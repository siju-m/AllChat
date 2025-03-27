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


    void initDatabase();//初始化数据库
    bool registerUser(const QString &username, const QString &password);
    QString loginUser(const QString &username, const QString &password);//如果验证成功就返回用户id，否则就返回空值

    bool addFriends(const QString &userId,const QString &friendId);//添加好友
    QSet<QString> selectFriends(const QString &userId);//查询好友
    QString selectNameById(const QString &userId);
    QMap<QString,QString> selectFriendsId_name(const QString &userId);
    QMap<QString,QString> selectUser_byName(const QString &name);//根据用户名模糊查询用户列表
    QMap<QString,QByteArray> selectAvatar_byName(const QString &name);//根据用户名模糊查询用户头像列表

    bool updateAvatar(const QString &userId, const QByteArray &imageData);//更新头像
    QByteArray getAvatar(const QString &userId);//获取头像
    QMap<QString,QByteArray> getFriendsAvatar(const QString &userId);

    bool deleteFriend(const QString &userId,const QString &friendId);
};

#endif // DATABASE_H
