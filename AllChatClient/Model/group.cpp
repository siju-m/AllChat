#include "group.h"

Group::Group(QString groupId, QString groupName, int memberCount, QString avatarPath):
    m_groupId(groupId),
    m_groupName(groupName),
    m_memberCount(memberCount),
    m_avatarPath(avatarPath)
{

}

QString Group::groupId()
{
    return m_groupId;
}

QString Group::groupName()
{
    return m_groupName;
}

int Group::memberCount()
{
    return m_memberCount;
}
