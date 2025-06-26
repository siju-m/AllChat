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

void CurrentUser::set_currentChatId(QString chatId)
{
    m_currentChatId = chatId;
}

QString CurrentUser::get_currentChatId()
{
    return m_currentChatId;
}

User CurrentUser::toUser()
{
    return User(m_userName, m_userId, m_avatarPath, true);
}

// QMap<QString, User> &CurrentUser::getFriendList()
// {
//     return m_friendList;
// }

// QMap<QString, User> &CurrentUser::getStrangerList()
// {
//     return m_strangerList;
// }

// QSet<QString> &CurrentUser::getGroupsIdList()
// {
//     return m_groups_idList;
// }

// QMap<QString, Group> &CurrentUser::getGroupList()
// {
//     return m_groups;
// }


