#ifndef USER_H
#define USER_H

#include <QString>


class User
{
public:
    User();
    User(QString userName, bool onlineState = false, QString avatarPath = QString());

    void setUserName(const QString &name);
    QString getUserName() const;

    void setOnlineState(const bool &state);
    bool getOnlineState() const;

    void setAvatarPath(const QString &path);
    QString getAvatarPath() const;
private:
    QString m_userName;
    bool m_onlineState;
    QString m_avatarPath;
};

// using UserList = QVector<User>;

#endif // USER_H
