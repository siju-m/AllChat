#include "user.h"

User::User() {}

User::User(QString userName, bool onlineState, QString avatarPath)
    : m_userName(userName)
    , m_onlineState(onlineState)
    , m_avatarPath(avatarPath)
{}

void User::setUserName(const QString &name)
{
    m_userName = name;
}

QString User::getUserName() const
{
    return m_userName;
}

void User::setOnlineState(const bool &state)
{
    m_onlineState = state;
}

bool User::getOnlineState() const
{
    return m_onlineState;
}

void User::setAvatarPath(const QString &path)
{
    m_avatarPath = path;
}

QString User::getAvatarPath() const
{
    return m_avatarPath;
}
