#include "server.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <Core/configmanager.h>

Server::Server(QObject *parent) :
    QTcpServer(parent) ,dataBase(new DataBase(this)),
    m_dataTransfer(new DataTransfer(this)),
    m_redisClient(new RedisClient(this)),
    m_friend_apply_cache(m_redisClient->friendApplyCache())
{
    dataBase->initDatabase();

    connect(m_dataTransfer,&DataTransfer::handleData,this,&Server::handleData);
}

Server::~Server()
{
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
    if(!success){//避免重复登录
        QByteArray packet = getPacket(LOGIN_FAILED);
        sendData(senderSocket,packet);
        return;
    }

    QByteArray avatar = dataBase->getAvatar(m_clients_userId[senderSocket]);
    QByteArray packet = getPacket(LOGIN_SUCCESS,m_clients_userId[senderSocket],avatar);
    sendData(m_clients_userId[senderSocket],packet);
    //更新联系列表
    updateContactList(m_clients_userId[senderSocket]);

    // 更新并广播在线用户列表
    // broadcast_userOnlineList();
    sendOnlineState(m_clients_userId.value(senderSocket), true);
    sendFrdOnlineList(m_clients_userId.value(senderSocket));

    //发送陌生人的信息
    updateGroup_strangerList(m_clients_userId[senderSocket]);

    //判断是否有待转发的消息
    sendForwardContents(m_clients_userId[senderSocket]);

    sendApplyList(m_clients_userId[senderSocket]);
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
    in.setVersion(ConfigManager::dataStreamVersion());

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
    case message_type::DELETE_FRIEND:{
        handle_deleteFriend(in,senderSocket);
    }break;
    case message_type::CREATE_GROUP:{
        handle_createGroup(in,senderSocket);
    }break;
    case message_type::GROUP_CHAT:{
        handle_groupChat(in,senderSocket);
    }break;
    case message_type::PRIVATE_FILE:{
        handle_privateFile(in,senderSocket);
    }break;
    default :  qDebug() << "接收到未知消息类型!";break;
    }
}

void Server::privateMessage(QDataStream &in,QTcpSocket *senderSocket){

    QString textMessage,id;
    in>>id>>textMessage;
    QByteArray packet = getPacket(CHAT, m_clients_userId[senderSocket], textMessage, getCurrentTime());
    sendData(id,packet);
}


void Server::privateImage(QDataStream &in,QTcpSocket *senderSocket)
{
    QByteArray imageData;
    QString id;
    qDebug()<<"处理接收图片"<<id;
    in >> id>> imageData;
    qDebug()<<"dataBuffer.size():"<<imageData.size();
    QByteArray packet1 = getPacket(IMAGE, m_clients_userId[senderSocket], imageData, getCurrentTime());
    sendData(id,packet1);
}



void Server::handleAddFriend(QDataStream &in, QTcpSocket *senderSocket)
{
    QString id;
    in>>id;
    //需要先判断是否有好友再转发，避免重复添加
    QSet<QString> friendsId = dataBase->selectFriends(m_clients_userId[senderSocket]);

    //好友请求在对方在线时直接发送，不在线就等对方登录再发过去
    //不能使用之前的不在线转发方案，因为好友请求在客户端没有存储，需要每回登录都发送
    //先判断有没有好友，再判断是否重复添加
    if(!friendsId.contains(id) && !m_friend_apply_cache->hasAlreadyApplied(m_clients_userId[senderSocket],id)){
        //还需要判断对方是否在线
        m_friend_apply_cache->applyFriend(m_clients_userId[senderSocket],id);

        if(m_userIds_client.contains(id)){
            QByteArray avatar = dataBase->getAvatar(m_clients_userId[senderSocket]);
            QByteArray packet= getPacket(ADD_FRIEND,m_clients_name[senderSocket],m_clients_userId[senderSocket],avatar);
            sendData(m_userIds_client[id],packet);
        }
    }
}

void Server::handleAddFriend_Result(QDataStream &in, QTcpSocket *senderSocket)
{
    QString id;
    in>>id;

    dataBase->addFriends(m_clients_userId[senderSocket],id);
    dataBase->addFriends(id,m_clients_userId[senderSocket]);

    updateContactList(m_clients_userId[senderSocket]);
    updateContactList(id);
    // broadcast_userOnlineList();//更新在线状态
    sendOnlineState(m_clients_userId.value(senderSocket), true);
    sendOnlineState(id, m_userIds_client.contains(id));

    QByteArray packet = getPacket(messageType, m_clients_userId[senderSocket], id);
    sendData(id,packet);
    sendData(m_clients_userId[senderSocket],packet);

    m_friend_apply_cache->removeApply(id,m_clients_userId[senderSocket]);
}

// bool Server::isOnline(const QString &id)
// {
//     return m_userIds_client.contains(id);
// }

void Server::sendForwardContents(const QString &userId)
{
    QByteArray forwardContent = m_redisClient->getForwardMessages(userId);
    if(!forwardContent.isEmpty()){
        m_userIds_client[userId]->write(forwardContent);
        m_redisClient->deleteForwardMessages(userId);
    }

    // m_userIds_client[userId]->write(m_forward_contents[userId]);
    // m_forward_contents.remove(userId);
}

void Server::sendApplyList(const QString &userId)
{
    QVector<QString> list = m_friend_apply_cache->getReceivedList(userId);
    for(const auto &it : list){
        QString senderName = dataBase->selectNameById(it);
        QByteArray avatar = dataBase->getAvatar(it);
        QByteArray packet= getPacket(ADD_FRIEND,senderName,it,avatar);
        sendData(userId,packet);
    }
}

void Server::updateContactList(const QString &userId)
{
    updateFriendsList(userId);
    updateGroupsList(userId);
}

void Server::updateFriendsList(const QString &userId)
{
    QMap<QString,QString> id_name = dataBase->selectFriendsId_name(userId);
    QMap<QString,QByteArray> id_avatar = dataBase->getFriendsAvatar(userId);
    QByteArray packet = getPacket(USER_LIST,id_name,id_avatar);
    sendData(userId,packet);
}

void Server::updateGroupsList(const QString &userId)
{
    QByteArray groups = dataBase->selectGroupsByUserId(userId);
    if(groups.isEmpty())
        return;
    QByteArray packet = getPacket(GROUP_LIST, groups);
    sendData(userId,packet);
}

void Server::updateGroup_strangerList(const QString &userId)
{
    QByteArray strangerList = dataBase->selectGroupStrangers(userId);
    QByteArray packet = getPacket(GROUP_STRANGER_LIST, strangerList);
    sendData(userId,packet);
}

void Server::handle_slelectByName(QDataStream &in, QTcpSocket *senderSocket)
{
    QString name;
    in>>name;
    QMap<QString,QString> id_name = dataBase->selectUser_byName(name);
    QMap<QString,QByteArray> id_avatar = dataBase->selectAvatar_byName(name);
    if(id_name.contains(m_clients_userId[senderSocket]))
        id_name.remove(m_clients_userId[senderSocket]);
    QByteArray result = getPacket(NEW_FRIEND_REULT,id_name,id_avatar);
    // senderSocket->write(result);
    sendData(m_clients_userId[senderSocket],result);
}

void Server::sendData(const QString &targetId, QByteArray &packet)
{
    QByteArray outData;
    QDataStream out(&outData,QIODevice::WriteOnly);
    out.setVersion(ConfigManager::dataStreamVersion());
    out<<static_cast<qint32>(packet.size());
    out.writeRawData(packet.constData(),packet.size());
    if(m_userIds_client.contains(targetId)){
        // qDebug()<<"向 "+targetId+" 发送数据";
        m_userIds_client[targetId]->write(outData);
        m_userIds_client[targetId]->flush();
    }
    else storeForwardContents(outData,targetId);
}

void Server::sendData(QTcpSocket *senderSocket, QByteArray &packet)
{
    QByteArray outData;
    QDataStream out(&outData,QIODevice::WriteOnly);
    out.setVersion(ConfigManager::dataStreamVersion());
    out<<static_cast<qint32>(packet.size());
    out.writeRawData(packet.constData(),packet.size());
    senderSocket->write(outData);
    senderSocket->flush();
}

void Server::handle_updateAvatar(QDataStream &in, QTcpSocket *senderSocket)
{
    QByteArray avatar;
    in>>avatar;
    bool success = dataBase->updateAvatar(m_clients_userId[senderSocket],avatar);
    QByteArray packet = getPacket(UPDATE_AVATAR_RESULT,success);
    sendData(senderSocket,packet);
}

void Server::handle_deleteFriend(QDataStream &in, QTcpSocket *senderSocket)
{
    QString friendId;
    in>>friendId;
    QString userId = m_clients_userId[senderSocket];
    bool success = dataBase->deleteFriend(userId,friendId);
    QByteArray packet = getPacket(DELETE_FRIEND,success,friendId);
    sendData(senderSocket,packet);
    //todo 通知被删除者的方式需要优化
    // updateFriendsList(friendId);
    updateContactList(friendId);
}

void Server::handle_createGroup(QDataStream &in, QTcpSocket *senderSocket)
{
    QVector<QString> ids;
    QString groupName;
    in >> ids >> groupName;
    ids << m_clients_userId[senderSocket];
    QString groupId = generateUniqueId();

    if(!dataBase->createGroup(groupId, groupName, m_clients_userId[senderSocket]))
        return;

    bool result = dataBase->insertGroupMember(ids, groupId);
    if(result){
        QByteArray packet = getPacket(CREATE_GROUP, groupId, groupName, (int)ids.size());
        for(const auto &id : ids){
            sendData(id, packet);
        }
    }
}

void Server::handle_groupChat(QDataStream &in, QTcpSocket *senderSocket)
{
    QString groupId, type;
    in >> groupId >> type;

    QString senderId = m_clients_userId[senderSocket];
    QByteArray data;
    // qDebug()<<type;
    if(type == "TEXT")
    {
        QString text;
        in >> text;
        data = getPacket(GROUP_CHAT, groupId, senderId, type, text, getCurrentTime());
    }
    else if(type == "IMAGE")
    {
        QByteArray image;
        in >> image;
        data = getPacket(GROUP_CHAT, groupId, senderId, type, image, getCurrentTime());
    }

    QVector<QString> ids = dataBase->selectUsersByGroupId(groupId);
    // qDebug()<<ids;
    // qDebug()<<"sender: "<<senderId;
    for(const QString &id : ids){
        if(id != senderId){
            // qDebug()<<"给"+m_clients_name[m_userIds_client[id]]+"发送群聊消息"<< text;
            sendData(id, data);
        }
    }
}

void Server::handle_privateFile(QDataStream &in, QTcpSocket *senderSocket)
{
    QString chatId, fileName;
    QByteArray data;
    in >> chatId >> fileName >> data;

    QByteArray packet = getPacket(PRIVATE_FILE, m_clients_userId[senderSocket], fileName, data, getCurrentTime());
    sendData(chatId, packet);
}

QString Server::getCurrentTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}


void Server::storeForwardContents(const QByteArray &content,const QString &userId)
{
    QByteArray forwardContent = m_redisClient->getForwardMessages(userId);
    if(!forwardContent.isEmpty()){
        m_redisClient->addForwardMessage(userId, forwardContent.append(content));
    }
    else{
        m_redisClient->addForwardMessage(userId, content);
    }
    // if(m_forward_contents.contains(userId)){
    //     m_forward_contents[userId].append(content);
    // }else{
    //     m_forward_contents[userId]=content;
    // }
}

// void Server::broadcast_userOnlineList() {
//     //在线状态是广播的，有点问题
//     QSet<QString> userId;
//     for (auto it = m_userIds_client.begin(); it != m_userIds_client.end(); ++it) {
//         userId.insert(it.key());
//     }
//     QByteArray packet = getPacket(ONLINE_LIST,userId);
//     for (auto it = m_userIds_client.begin(); it != m_userIds_client.end(); ++it) {
//         // qDebug()<<"更新"+it.key()+"的在线列表";
//         sendData(it.key(),packet);
//     }
// }

void Server::sendOnlineState(const QString &id, const bool state)
{
    QByteArray packet = getPacket(ONLINE_STATE, id, state);
    QSet<QString> friends = dataBase->selectFriends(id);
    for(auto it = friends.begin(); it != friends.end(); ++it){
        if(m_userIds_client.contains(*it)){
            sendData(*it, packet);
        }
    }
}

void Server::sendFrdOnlineList(const QString &id)
{
    QSet<QString> friends = dataBase->selectFriends(id);
    QSet<QString> onlineFrd;
    for(auto it = friends.begin(); it != friends.end(); ++it){
        if(m_userIds_client.contains(*it))
            onlineFrd.insert(*it);
    }
    QByteArray packet = getPacket(ONLINE_LIST, onlineFrd);
    sendData(id, packet);
}

void Server::onClientDisconnected() {
    auto *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (!clientSocket) return;

    QString userName = m_clients_name[clientSocket];

    //发送用户下线消息
    sendOnlineState(m_clients_userId.value(clientSocket), false);
    // 移除用户数据
    m_clients_name.remove(clientSocket);
    m_userIds_client.remove(m_userIds_client.key(clientSocket));
    m_clients_userId.remove(clientSocket);

    clientSocket->deleteLater();
    qDebug() << "用户断开连接:" << userName;

    // 广播用户下线消息
    // broadcast_userOnlineList();
}

QString Server::generateUniqueId() {
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

template<typename... Args>
QByteArray Server::getPacket(Args... args)
{
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.setVersion(ConfigManager::dataStreamVersion());
    (void)(stream<<...<< args);
    return packet;
}
