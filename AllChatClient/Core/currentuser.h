#ifndef CURRENTUSER_H
#define CURRENTUSER_H

// #include "Model/group.h"
#include "Model/user.h"
// #include <QMap>
// #include <QSet>
#include <QString>


class CurrentUser
{
public:
    CurrentUser(const CurrentUser& obj) = delete;
    CurrentUser& operator=(const CurrentUser& obj) = delete;
    static CurrentUser *getInstance()
    {
        return m_userInfo;
    }
    void set_userName(QString name);
    QString get_userName();

    void set_userId(QString Id);
    QString get_userId();

    void set_currentChatId(QString chatId);
    QString get_currentChatId();

    User toUser();
private:
    CurrentUser() = default;
    static CurrentUser *m_userInfo;

    QString m_userId = QString();
    QString m_userName = QString();
    QString m_currentChatId = QString();
};

#endif // CURRENTUSER_H
