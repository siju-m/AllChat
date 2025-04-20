#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>
#include <QUuid>
#include <Core/datatransfer.h>
#include "Core/database.h"

enum message_type{
    IMAGE,
    LOGIN,
    REGISTER,
    CHAT,
    LOGIN_SUCCESS,LOGIN_FAILED,
    REGISTER_SUCCESS,REGISTER_FAILED,
    USER_LIST,
    ADD_FRIEND,
    AGREE_FRIEND,
    FIND_NEW_FRIEND,
    NEW_FRIEND_REULT,
    ONLINE_LIST,
    UPDATE_AVATAR,
    UPDATE_AVATAR_RESULT,
    DELETE_FRIEND,
    CREATE_GROUP,
    GROUP_LIST,
    GROUP_STRANGER_LIST,
    GROUP_CHAT
};

class Server : public QTcpServer {
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QString generateUniqueId();              // 生成唯一 ID
    void broadcast_userOnlineList();                // 广播在线用户列表
    void broadcastMessage(const QString &sender, const QString &message); // 广播消息

    void privateMessage(QDataStream &in,QTcpSocket *senderSocket);//转发私聊信息
    void privateImage(QDataStream &in,QTcpSocket *senderSocket);//转发私聊图片

    void handleLogin(QDataStream &in,QTcpSocket *senderSocket);//处理登录请求
    bool loginUser(QTcpSocket *socket,const QString &username, const QString &password);//确认是否可以登录
    void handleRegist(QDataStream &in,QTcpSocket *senderSocket);//处理注册请求

    void handleAddFriend(QDataStream &in,QTcpSocket *senderSocket);//处理添加好友请求
    void handleAddFriend_Result(QDataStream &in,QTcpSocket *senderSocket);//处理添加好友请求的结果

    //确认是否在线
    bool confirm_isOnline(const QString &id);

    //存储待转发消息
    void store_forwardContents(const QByteArray &content,const QString &userId);
    //转发存储的消息
    void send_forwardContents(const QString &userId);
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
    QSet<QPair<QString,QString>> m_alreadyApply;        //确保每个好友申请只发送一次
    // QHash<QString,QSet<QString>> m_groups;              // 群聊号对应的用户列表

    //todo 可以把东西全都存在数据库 //如果过大就存起来
    QHash<QString,QByteArray> m_forward_contents;       //转发消息

    DataBase *dataBase;
    DataTransfer *m_dataTransfer;

    message_type messageType;
};

#endif // SERVER_H
