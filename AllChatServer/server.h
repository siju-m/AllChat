#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>
#include <QUuid>
#include "database.h"

enum ReceivingState {
    WaitingForHeader, // 等待接收数据头
    ReceivingData     // 接收数据中
};
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
    ONLINE_LIST
};

class Server : public QTcpServer {
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onClientDisconnected();

private:
    QMap<QTcpSocket *, QString> m_clients_name;   // 客户端对应的用户名
    QMap<QString, QTcpSocket *> m_userIds_client;     // 用户 ID对应的客户端
    QMap<QTcpSocket *,QString> m_clients_userId;     // 客户端对应的用户 ID
    QHash<QString,QSet<QString>> m_friends;     // 每个用户对应的好友申请列表
    QSet<QPair<QString,QString>> m_alreadyApply; //确保每个好友申请只发送一次

    //todo 可以把东西全都存在数据库
    QHash<QString,QByteArray> m_forward_contents;//如果过大就存起来

    DataBase *dataBase;
    bool loginUser(QTcpSocket *socket,const QString &username, const QString &password);

    QString generateUniqueId();              // 生成唯一 ID
    //todo 改为广播在线状态
    void broadcast_userOnlineList();                // 广播在线用户列表
    void broadcastMessage(const QString &sender, const QString &message); // 广播消息
    void addClient(QTcpSocket *client, const QString &userName);

    message_type messageType;

    ReceivingState currentReceivingState = WaitingForHeader; // 当前状态初始化为等待数据头


    // QString messageType;
    qint32 currentDataLength = 0;    // 数据长度
    qint32 receivedBytes = 0;        // 已接收字节数
    QByteArray dataBuffer;           // 用于暂存接收到的数据
    void resetState();

    void privateMessage(QTcpSocket *socket, QDataStream &stream);//转发私聊信息
    void privateImage(QTcpSocket *socket, QString id);//转发私聊图片

    void receiveImage(QDataStream &in,QTcpSocket *senderSocket);//接受图片
    void handleLogin(QDataStream &in,QTcpSocket *senderSocket);//处理登录请求
    void handleRegist(QDataStream &in,QTcpSocket *senderSocket);//处理注册请求

    void handleAddFriend(QDataStream &in,QTcpSocket *senderSocket);//处理添加好友请求
    void handleAddFriend_Result(QDataStream &in,QTcpSocket *senderSocket);//处理添加好友请求的结果

    bool confirm_isOnline(const QString &id);//确认是否在线

    void store_forwardContents(const QByteArray &content,const QString &userId);//存储待转发消息
    void send_forwardContents(const QString &userId);//转发存储的消息
    void updateFriendsList(const QString &userId);//更新用户好友列表

    void handle_slelectByName(QDataStream &in,QTcpSocket *senderSocket);//处理模糊查询用户列表请求

    template <typename... Args>
    QByteArray getPacket(Args... args);
    void sendData(const QString &targetId,QByteArray &packet);
};

#endif // SERVER_H
