#include "user.h"

User::User() {}

User::User(QString userName, QString id, bool onlineState)
    : m_userName(userName)
    , m_id(id)
    , m_onlineState(onlineState)
{}

void User::setUserName(const QString &name)
{
    m_userName = name;
}

QString User::getUserName() const
{
    return m_userName;
}

void User::setUserId(const QString &id)
{
    m_id = id;
}

QString User::getUserId() const
{
    return m_id;
}

void User::setOnlineState(const bool &state)
{
    m_onlineState = state;
}

bool User::getOnlineState() const
{
    return m_onlineState;
}
