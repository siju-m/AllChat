#ifndef DATABASE_H
#define DATABASE_H

#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QUuid>
#include <QSet>
#include <QCryptographicHash>

class DataBase : public QObject
{
public:
    DataBase(QObject *parent = nullptr);


    // 初始化数据库
    void initDatabase();
    // 注册登录
    bool registerUser(const QString &username, const QString &password);
    // 登陆验证，如果验证成功就返回用户id，否则就返回空值
    QString loginUser(const QString &username, const QString &password);

    // 好友
    bool addFriends(const QString &userId,const QString &friendId);
    QSet<QString> selectFriends(const QString &userId);
    bool deleteFriend(const QString &userId,const QString &friendId);

    QString selectNameById(const QString &userId);
    // 好友列表
    QMap<QString,QString> selectFriendsId_name(const QString &userId);
    // 根据用户名模糊查询用户列表
    QMap<QString,QString> selectUser_byName(const QString &name);

    // 头像
    bool updateAvatar(const QString &userId, const QByteArray &imageData);//更新头像
    QByteArray getAvatar(const QString &userId);//获取头像
    QMap<QString,QByteArray> getFriendsAvatar(const QString &userId);
    QMap<QString,QByteArray> selectAvatar_byName(const QString &name);//根据用户名模糊查询用户头像列表


    // 创建群聊
    bool createGroup(const QString &groupId, const QString &groupName, const QString &creatorId);
    // 添加群成员
    bool insertGroupMember(const QVector<QString> &ids, const QString &groupId);
    // 查询用户的群聊列表
    QMap<QString, QString> selectGroupsByUserId(const QString &id);
    // 查询群成员列表
    QVector<QString> selectUsersByGroupId(const QString &groupId);
    // 查询所有群里不是好友的用户列表
    QByteArray selectGroupStrangers(const QString &userId);
};

#endif // DATABASE_H
