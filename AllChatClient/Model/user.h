#ifndef USER_H
#define USER_H

#include <QString>


class User
{
public:
    User();
    User(QString userName, QString id, bool onlineState = false);

    void setUserName(const QString &name);
    QString getUserName() const;

    void setUserId(const QString &id);
    QString getUserId() const;

    void setOnlineState(const bool &state);
    bool getOnlineState() const;

private:
    QString m_userName;
    QString m_id;
    bool m_onlineState;
};

// using UserList = QVector<User>;

#endif // USER_H
