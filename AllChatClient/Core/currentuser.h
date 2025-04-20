#ifndef CURRENTUSER_H
#define CURRENTUSER_H

#include "Model/user.h"
#include <QMap>
#include <QSet>
#include <QString>


class CurrentUser
{
public:
    CurrentUser(const CurrentUser& obj) = delete;
    CurrentUser& operator=(const CurrentUser& obj) = delete;
    static CurrentUser* getInstance()
    {
        return m_userInfo;
    }
    void set_userName(QString name);
    QString get_userName();

    void set_userId(QString Id);
    QString get_userId();

    void set_avatarPath(QString path);
    QString get_avatarPath();

    User toUser();

    QMap<QString, User>& getFriendList();
    QMap<QString, User>& getStrangerList();
    QSet<QString>& getGroupsIdList();
private:
    CurrentUser() = default;
    static CurrentUser* m_userInfo;

    QString m_userId = QString();
    QString m_userName = QString();
    QString m_avatarPath = QString();
    QMap<QString, User> m_friendList;
    QMap<QString, User> m_strangerList;
    QSet<QString> m_groups_idList;
};

#endif // CURRENTUSER_H
