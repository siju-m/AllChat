#include "server.h"
#include <QDateTime>
#include <QDebug>
#include <QFile>

Server::Server(QObject *parent)
    : QTcpServer(parent) ,dataBase(new DataBase(this)),
    m_dataTransfer(new DataTransfer(this))
{
    dataBase->initDatabase();

    connect(m_dataTransfer,&DataTransfer::handleData,this,&Server::handleData);
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

    if(!userId.isEmpty() && !m_userIds_client.contains(userId)){
        m_clients_name[socket] = username;
        m_userIds_client[userId] = socket;
        m_clients_userId[socket] = userId;
        qDebug() << "User logged in: " << username << " with ID: " << userId;
        return true;
    }
    return false;
}

void Server::handleLogin(QDataStream &in, QTcpSocket *senderSocket)
{

    QString username, password;
    in >> username >> password;
    bool success = loginUser(senderSocket,username, password);
    qDebug()<<"用户登录"<<username<<success;
    if(!success){//避免重复登录
        QByteArray packet = getPacket(LOGIN_FAILED);
        sendData(senderSocket,packet);
        return;
    }

    QByteArray avatar = dataBase->getAvatar(m_clients_userId[senderSocket]);
    QByteArray packet = getPacket(LOGIN_SUCCESS,avatar);
    sendData(m_clients_userId[senderSocket],packet);
    //向登录用户发送他的最新好友列表
    updateFriendsList(m_clients_userId[senderSocket]);

    // 更新并广播在线用户列表
    broadcast_userOnlineList();

    //判断是否有待转发的消息
    if(m_forward_contents.contains(m_clients_userId[senderSocket]))
        send_forwardContents(m_clients_userId[senderSocket]);
}

void Server::handleRegist(QDataStream &in, QTcpSocket *senderSocket)
{
    QString username, password;
    in >> username >> password;
    bool success = dataBase->registerUser(username, password);
    QByteArray packet = getPacket(success ? REGISTER_SUCCESS : REGISTER_FAILED);
    qDebug()<<"用户注册";
    sendData(senderSocket,packet);
}


void Server::incomingConnection(qintptr socketDescriptor) {
    auto *clientSocket = new QTcpSocket(this);
    if (clientSocket->setSocketDescriptor(socketDescriptor)) {
        connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);

        // m_clients_name[clientSocket]="";
        qDebug() << "新连接成功建立.";
    } else {
        delete clientSocket;
        qWarning() << "接受连接失败.";
    }
}

void Server::onReadyRead() {
    auto *senderSocket = qobject_cast<QTcpSocket *>(sender());
    if (!senderSocket) return;
    m_dataTransfer->receiveData(senderSocket);
}


void Server::handleData(QByteArray data,QTcpSocket *senderSocket)
{
    QDataStream in(data);
    in.setVersion(QDataStream::Qt_5_15);

    in >> messageType;
    switch(messageType){
    case message_type::IMAGE:{
        privateImage(in,senderSocket);
    }break;
    case message_type::LOGIN:{
        handleLogin(in,senderSocket);
    }break;
    case message_type::REGISTER:{
        handleRegist(in,senderSocket);
    }break;
    case message_type::CHAT:{
        privateMessage(in,senderSocket);
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
    case message_type::UPDATE_AVATAR:{
        handle_updateAvatar(in,senderSocket);
    }break;
    default :  qDebug() << "接收到未知消息类型!";break;
    }
}

void Server::privateMessage(QDataStream &in,QTcpSocket *senderSocket){

    QString textMessage,id;
    in>>id>>textMessage;
    QByteArray packet = getPacket(CHAT , m_clients_userId[senderSocket] , textMessage);
    sendData(id,packet);
}


void Server::privateImage(QDataStream &in,QTcpSocket *senderSocket)
{
    QByteArray imageData;
    QString id;
    qDebug()<<"处理接收图片"<<id;
    in >> id>> imageData;
    qDebug()<<"dataBuffer.size():"<<imageData.size();
    QByteArray packet1 = getPacket(IMAGE,m_clients_userId[senderSocket] , imageData);
    sendData(id,packet1);
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

void Server::sendData(QTcpSocket *senderSocket, QByteArray &packet)
{
    QByteArray outData;
    QDataStream out(&outData,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out<<static_cast<qint32>(packet.size());
    out.writeRawData(packet.constData(),packet.size());
    senderSocket->write(outData);
}

void Server::handle_updateAvatar(QDataStream &in, QTcpSocket *senderSocket)
{
    QByteArray avatar;
    in>>avatar;
    bool success = dataBase->updateAvatar(m_clients_userId[senderSocket],avatar);
    QByteArray packet = getPacket(UPDATE_AVATAR_RESULT,success);
    sendData(senderSocket,packet);
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
