#ifndef USER_H
#define USER_H

#include <QString>


class User
{
public:
    User();
    User(QString userName, QString id, bool onlineState = false, QString avatarPath = QString());

    void setUserName(const QString &name);
    QString getUserName() const;

    void setUserId(const QString &id);
    QString getUserId();

    void setOnlineState(const bool &state);
    bool getOnlineState() const;

    void setAvatarPath(const QString &path);
    QString getAvatarPath() const;
private:
    QString m_userName;
    QString m_id;
    bool m_onlineState;
    QString m_avatarPath;
};

// using UserList = QVector<User>;

#endif // USER_H
