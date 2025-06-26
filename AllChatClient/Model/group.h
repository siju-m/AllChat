#ifndef GROUP_H
#define GROUP_H

#include <QString>


class Group
{
public:
    Group(QString groupId="", QString groupName="", int memberCount=0, QString avatarPath = "");

    QString groupId();
    QString groupName();
    int memberCount();

private:
    QString m_groupId;
    QString m_groupName;
    int m_memberCount;
    QString m_avatarPath;
};

#endif // GROUP_H
