#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>
#include <QUuid>
#include <Core/datatransfer.h>
#include "Core/database.h"
#include "Core/httpservermanager.h"
#include <Core/redisclient.h>

enum message_type{
    IMAGE,//图片消息
    LOGIN,//登录请求
    REGISTER,//注册请求
    CHAT,//聊天消息
    LOGIN_SUCCESS,LOGIN_FAILED,//登陆结果
    REGISTER_SUCCESS,REGISTER_FAILED,//注册结果
    USER_LIST,//用户列表消息
    ADD_FRIEND,//添加好友消息
    AGREE_FRIEND,//同意好友请求
    FIND_NEW_FRIEND,//查询用户请求
    NEW_FRIEND_REULT,//查询用户结果
    ONLINE_STATE,//在线用户状态
    ONLINE_LIST,
    UPDATE_AVATAR,
    UPDATE_AVATAR_RESULT,
    REFRESH_AVATAR,
    DELETE_FRIEND,
    CREATE_GROUP,
    GROUP_LIST,
    GROUP_STRANGER_LIST,
    GROUP_CHAT,
    PRIVATE_FILE
};

class Server : public QTcpServer {
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QString generateUniqueId();              // 生成唯一 ID
    void sendOnlineState(const QString &id, const bool state);
    void sendFrdOnlineList(const QString &id);                // 发送在线用户列表

    void privateMessage(QDataStream &in,QTcpSocket *senderSocket);//转发私聊信息
    void privateImage(QDataStream &in,QTcpSocket *senderSocket);//转发私聊图片

    void handleLogin(QDataStream &in,QTcpSocket *senderSocket);//处理登录请求
    bool loginUser(QTcpSocket *socket,const QString &username, const QString &password);//确认是否可以登录
    void handleRegist(QDataStream &in,QTcpSocket *senderSocket);//处理注册请求

    void handleAddFriend(QDataStream &in,QTcpSocket *senderSocket);//处理添加好友请求
    void handleAddFriend_Result(QDataStream &in,QTcpSocket *senderSocket);//处理添加好友请求的结果

    //确认是否在线
    // bool isOnline(const QString &id);

    //存储待转发消息
    void storeForwardContents(const QByteArray &content,const QString &userId);
    //转发存储的消息
    void sendForwardContents(const QString &userId);
    //发送好友申请列表
    void sendApplyList(const QString &userId);
    //更新群聊和好友列表
    void updateContactList(const QString &userId);
    //更新用户好友列表
    void updateFriendsList(const QString &userId);
    //更新用户群聊列表
    void updateGroupsList(const QString &userId);
    // 更新群聊中没有好友的用户信息
    void updateGroup_strangerList(const QString &userId);

    void handle_slelectByName(QDataStream &in,QTcpSocket *senderSocket);//处理模糊查询用户列表请求

    template <typename... Args>
    QByteArray getPacket(Args... args);
    void sendData(const QString &targetId,QByteArray &packet);//用于发送可以等待对方上线的消息
    void sendData(QTcpSocket *senderSocket,QByteArray &packet);//用于发送实时信息

    void handle_updateAvatar(QDataStream &in,QTcpSocket *senderSocket);

    void handle_deleteFriend(QDataStream &in,QTcpSocket *senderSocket);

    void handle_createGroup(QDataStream &in,QTcpSocket *senderSocket);
    void handle_groupChat(QDataStream &in,QTcpSocket *senderSocket);

    void handle_privateFile(QDataStream &in,QTcpSocket *senderSocket);

    QString getCurrentTime();

private slots:
    void onReadyRead();
    void handleData(QByteArray data,QTcpSocket *senderSocket);
    void onClientDisconnected();

private:
    QMap<QTcpSocket *, QString> m_clients_name;         // 客户端对应的用户名
    QMap<QString, QTcpSocket *> m_userIds_client;       // 用户 ID对应的客户端
    QMap<QTcpSocket *,QString> m_clients_userId;        // 客户端对应的用户 ID
    QHash<QString,QSet<QString>> m_friends;             // 每个用户对应的好友申请列表
    // QSet<QPair<QString,QString>> m_alreadyApply;        //确保每个好友申请只发送一次
    // QHash<QString,QSet<QString>> m_groups;              // 群聊号对应的用户列表

    QHash<QString,QByteArray> m_forward_contents;       //转发消息

    DataBase *dataBase;
    DataTransfer *m_dataTransfer;
    RedisClient *m_redisClient;
    FriendApplyCache *m_friend_apply_cache;

    message_type messageType;
    HttpServerManager *m_http_manager;
};

#endif // SERVER_H
