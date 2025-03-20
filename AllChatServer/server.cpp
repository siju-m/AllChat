#include "server.h"
#include <QDateTime>
#include <QDebug>
#include <QFile>

Server::Server(QObject *parent)
    : QTcpServer(parent) ,dataBase(new DataBase(this)){
    dataBase->initDatabase();
    // QFile file("D:/msiju/Downloads/1.jpg");
    // if (!file.open(QIODevice::ReadOnly)) {
    //     qDebug() << "Failed to open file" ;
    // }else{
    //     QByteArray imageData = file.readAll();
    //     dataBase->updateAvatar("429491ac-7e22-4e76-b3fa-bb6b856e515f",imageData);
    // }

    // qDebug()<<dataBase->getAvatar("bebfa97f-9e55-4666-8c00-16c77fa8ecf8").isEmpty();
}

bool Server::loginUser(QTcpSocket *socket,const QString &username, const QString &password) {
    QString userId = dataBase->loginUser(username,password);

    if(!userId.isNull()){
        m_clients_name[socket] = username;
        m_userIds_client[userId] = socket;
        m_clients_userId[socket] = userId;
        qDebug() << "User logged in: " << username << " with ID: " << userId;
    }
    return !userId.isNull();
}

void Server::handleLogin(QDataStream &in, QTcpSocket *senderSocket)
{
    QString username, password;
    in >> username >> password;
    bool success = loginUser(senderSocket,username, password);

    QByteArray avatar = dataBase->getAvatar(m_clients_userId[senderSocket]);
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << (success ? LOGIN_SUCCESS : LOGIN_FAILED);
    qDebug()<<"用户登录"<<avatar.size();
    if(success){
        //发送头像
            // out.writeRawData(avatar.constData(),avatar.size());
        out<<avatar;
    }
    QByteArray outData;
    QDataStream out1(&outData,QIODevice::WriteOnly);
    out1.setVersion(QDataStream::Qt_5_15);
    out1<<static_cast<qint32>(packet.size());
    out1.writeRawData(packet.constData(),packet.size());
    senderSocket->write(outData);
    if(success){
        //向登录用户发送他的最新好友列表
        updateFriendsList(m_clients_userId[senderSocket]);

        // 更新并广播在线用户列表
        broadcast_userOnlineList();

        //判断是否有待转发的消息
        if(m_forward_contents.contains(m_clients_userId[senderSocket]))
            send_forwardContents(m_clients_userId[senderSocket]);
    }

}

void Server::handleRegist(QDataStream &in, QTcpSocket *senderSocket)
{
    QString username, password;
    in >> username >> password;
    bool success = dataBase->registerUser(username, password);
    QByteArray packet = getPacket(success ? REGISTER_SUCCESS : REGISTER_FAILED);
    qDebug()<<"用户注册";
    //todo 后面看能不能封装
    QByteArray outData;
    QDataStream out(&outData,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out<<static_cast<qint32>(packet.size());
    out.writeRawData(packet.constData(),packet.size());
    senderSocket->write(outData);
    // senderSocket->write(packet);
}


void Server::incomingConnection(qintptr socketDescriptor) {
    auto *clientSocket = new QTcpSocket(this);
    if (clientSocket->setSocketDescriptor(socketDescriptor)) {
        connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);

        m_clients_name[clientSocket]="";
        qDebug() << "新连接成功建立.";
    } else {
        delete clientSocket;
        qWarning() << "接受连接失败.";
    }
}

void Server::onReadyRead() {
    auto *senderSocket = qobject_cast<QTcpSocket *>(sender());
    if (!senderSocket) return;


    QDataStream in(senderSocket);
    in.setVersion(QDataStream::Qt_5_15);

    while (!in.atEnd()) {
        if(currentReceivingState == WaitingForHeader)
            in >> messageType;
        switch(messageType){
        case message_type::IMAGE:{
            receiveImage(in,senderSocket);
        }break;
        case message_type::LOGIN:{
            handleLogin(in,senderSocket);
        }break;
        case message_type::REGISTER:{
            handleRegist(in,senderSocket);
        }break;
        case message_type::CHAT:{
            privateMessage(senderSocket,in);
        }break;
        case message_type::ADD_FRIEND:{
            handleAddFriend(in,senderSocket);
        }break;
        case message_type::AGREE_FRIEND:{
            handleAddFriend_Result(in,senderSocket);
        }break;
        case message_type::FIND_NEW_FRIEND:{
            handle_slelectByName(in,senderSocket);
        }break;
            default :  qDebug() << "Unknown message type received!";break;
        }
    }
}
void Server::resetState() {
    currentReceivingState = WaitingForHeader;
    currentDataLength = 0;
    receivedBytes = 0;
    dataBuffer.clear();
}

void Server::privateMessage(QTcpSocket *socket, QDataStream &stream){

    QString textMessage,id;
    stream>>id>>textMessage;
    QByteArray packet = getPacket(CHAT , m_clients_userId[socket] , textMessage);
    sendData(id,packet);
}

void Server::privateImage(QTcpSocket *socket,QString id)
{
    // 转发图片数据给其他用户
    QByteArray packet1 = getPacket(IMAGE,m_clients_userId[socket] , dataBuffer);
    sendData(id,packet1);
}

void Server::receiveImage(QDataStream &in,QTcpSocket *senderSocket)
{
    if (currentReceivingState == WaitingForHeader) {
        if (senderSocket->bytesAvailable() < static_cast<qint64>(sizeof(qint32) * 2)) return; // 数据不足，等待下次读取
        in >> currentDataLength;
        qDebug()<<messageType << currentDataLength;
        if (currentDataLength <= 0 || currentDataLength > 10 * 1024 * 1024) {
            qWarning() << "Invalid data length:" << currentDataLength;
            resetState();
            return;
        }
        dataBuffer.clear();
        dataBuffer.reserve(currentDataLength);
        currentReceivingState = ReceivingData;
    }
    if (currentReceivingState == ReceivingData) {
        int bytesToRead = qMin(senderSocket->bytesAvailable(), currentDataLength - receivedBytes);
        QByteArray chunk = senderSocket->read(bytesToRead);
        dataBuffer.append(chunk);
        receivedBytes += chunk.size();
        if (receivedBytes == currentDataLength) {
            QDataStream stream(dataBuffer);
            QByteArray imageData;
            QString id;
            qDebug()<<"处理接收图片"<<id;
            stream >> id>> imageData;
            dataBuffer = imageData;
            qDebug()<<"messageType:"<<messageType;
            qDebug()<<"dataBuffer.size():"<<dataBuffer.size();
            privateImage(senderSocket,id);
            resetState();
        }
    }
}



void Server::handleAddFriend(QDataStream &in, QTcpSocket *senderSocket)
{
    QString id;
    in>>id;
    //需要先判断是否有好友再转发，避免重复添加
    QSet<QString> friendsId = dataBase->selectFriends(m_clients_userId[senderSocket]);

    //先判断有没有好友，再判断是否重复添加
    if(!friendsId.contains(id) && !m_alreadyApply.contains(qMakePair(m_clients_userId[senderSocket],id))){
        //还需要判断对方是否在线
        m_alreadyApply.insert(qMakePair(m_clients_userId[senderSocket],id));
        QByteArray packet= getPacket(ADD_FRIEND,m_clients_name[senderSocket],m_clients_userId[senderSocket]);
        sendData(id,packet);
    }

}

void Server::handleAddFriend_Result(QDataStream &in, QTcpSocket *senderSocket)
{
    QString id;
    in>>id;
    QByteArray packet = getPacket(messageType,m_clients_name[senderSocket],id);
    //todo 转发和存储的判断可以封装一下
    sendData(id,packet);
    dataBase->addFriends(m_clients_userId[senderSocket],id);
    dataBase->addFriends(id,m_clients_userId[senderSocket]);
    updateFriendsList(m_clients_userId[senderSocket]);//更新同意者的好友列表
    updateFriendsList(id);//更新请求者的好友列表
    broadcast_userOnlineList();//更新在线状态
    m_alreadyApply.remove(qMakePair(id,m_clients_userId[senderSocket]));
}

bool Server::confirm_isOnline(const QString &id)
{
    return m_userIds_client.contains(id);
}

void Server::send_forwardContents(const QString &userId)
{
    m_userIds_client[userId]->write(m_forward_contents[userId]);
    m_forward_contents.remove(userId);
}

void Server::updateFriendsList(const QString &userId)
{
    QMap<QString,QString> id_name = dataBase->selectFriendsId_name(userId);
    QMap<QString,QByteArray> id_avatar = dataBase->getFriendsAvatar(userId);
    QByteArray packet1 = getPacket(USER_LIST,id_name,id_avatar);
    sendData(userId,packet1);
}

void Server::handle_slelectByName(QDataStream &in, QTcpSocket *senderSocket)
{
    QString name;
    in>>name;
    QMap<QString,QString> id_name = dataBase->selectUser_byName(name);
    if(id_name.contains(m_clients_userId[senderSocket]))
        id_name.remove(m_clients_userId[senderSocket]);
    QByteArray result = getPacket(NEW_FRIEND_REULT,id_name);
    // senderSocket->write(result);
    sendData(m_clients_userId[senderSocket],result);
}

void Server::sendData(const QString &targetId, QByteArray &packet)
{
    QByteArray outData;
    QDataStream out(&outData,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out<<static_cast<qint32>(packet.size());
    out.writeRawData(packet.constData(),packet.size());
    if(m_userIds_client.contains(targetId)){
        // qDebug()<<"向 "+targetId+" 发送数据";
        m_userIds_client[targetId]->write(outData);
    }
    else store_forwardContents(outData,targetId);
}

void Server::store_forwardContents(const QByteArray &content,const QString &userId)
{
    if(m_forward_contents.contains(userId)){
        m_forward_contents[userId].append(content);
    }else{
        m_forward_contents[userId]=content;
    }
}

void Server::broadcast_userOnlineList() {

    QSet<QString> userId;
    for (auto it = m_userIds_client.begin(); it != m_userIds_client.end(); ++it) {
        userId.insert(it.key());
    }
    QByteArray packet = getPacket(ONLINE_LIST,userId);
    for (auto it = m_userIds_client.begin(); it != m_userIds_client.end(); ++it) {
        qDebug()<<"更新"+it.key()+"的在线列表";
        sendData(it.key(),packet);
    }

    qDebug() << "Broadcasted user list.";
}

void Server::onClientDisconnected() {
    auto *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (!clientSocket) return;

    QString userName = m_clients_name[clientSocket];

    // 移除用户数据
    m_clients_name.remove(clientSocket);
    m_userIds_client.remove(m_userIds_client.key(clientSocket));
    m_clients_userId.remove(clientSocket);

    clientSocket->deleteLater();
    qDebug() << "用户断开连接:" << userName;

    // 广播用户下线消息
    broadcast_userOnlineList();
}

QString Server::generateUniqueId() {
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

template<typename... Args>
QByteArray Server::getPacket(Args... args)
{
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_15);
    (stream<<...<< args);
    return packet;
}
