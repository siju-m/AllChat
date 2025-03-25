#include "currentuser.h"

//饿汉模式
CurrentUser* CurrentUser::m_userInfo = new CurrentUser;

void CurrentUser::set_userName(QString name)
{
    m_userName = name;
}

QString CurrentUser::get_userName()
{
    return m_userName;
}

void CurrentUser::set_userId(QString Id)
{
    m_userId = Id;
}

QString CurrentUser::get_userId()
{
    return m_userId;
}

void CurrentUser::set_avatarPath(QString path)
{
    m_avatarPath = path;
}

QString CurrentUser::get_avatarPath()
{
    return m_avatarPath;
}


