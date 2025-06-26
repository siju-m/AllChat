#include "contactmanager.h"
#include "Model/messagepacket.h"
#include "chathistorymanager.h"

ContactManager *ContactManager::m_instance;
std::mutex ContactManager::m_mutex;

ContactManager::ContactManager(QObject *parent) :
    QObject(parent),
    m_historyManager(nullptr)
{}

void ContactManager::updateUserList(const QMap<QString, QString> &newUserList,const QMap<QString,QByteArray> &new_idAvatar) {

    QSet<QString> newUserSet;
    const auto &newKeys = newUserList.keys();
    for(auto &it:newKeys)
        newUserSet<<it;
    QSet<QString> currentUserSet;
    const auto &ctKeys = m_friendList.keys();
    for(auto &it:ctKeys)
        currentUserSet<<it;
    // 计算需要删除的用户
    QSet<QString> usersToRemove = currentUserSet - newUserSet;
    // 计算新增的用户
    QSet<QString> usersToAdd = newUserSet - currentUserSet;

    // 删除用户
    for (const QString &userId : usersToRemove) {
        removeFriend(userId);
    }

    // 添加新用户
    for (const QString &userId : usersToAdd) {
        addFriend(userId, User(newUserList[userId], userId));
    }

    if(m_historyManager == nullptr)
        m_historyManager = ChatHistoryManager::getInstance();

    emit clearModel();
    const auto &keys = m_friendList.keys();
    for(auto &id:keys){
        User frd = m_friendList.value(id);
        QString avatarPath = new_idAvatar[id].size()?m_historyManager->storeAvatar(frd.getUserName(),new_idAvatar[id]):"";
        setFriendAvatar(id, avatarPath);

        emit addFriendModel(frd.getUserName(),
                            id,
                            StateEnum::onlineState_type(frd.getOnlineState()?StateEnum::ONLINE:StateEnum::OFFLINE),
                            avatarPath);

        QPair<QString,QString> lastMessage = m_historyManager->getLastMessage(id);
        emit addChatModel(frd.getUserName(),
                          id,
                          lastMessage.first,
                          lastMessage.second,
                          0,
                          avatarPath);
    }
}

bool ContactManager::isFriend(const QString& userId) const {
    return m_friendList.contains(userId);
}

bool ContactManager::isStranger(const QString& userId) const {
    return m_strangerList.contains(userId);
}

bool ContactManager::isInGroup(const QString& groupId) const {
    return m_groupIdList.contains(groupId);
}

User ContactManager::friendById(const QString &userId) const
{
    if(m_friendList.contains(userId))
        return m_friendList.value(userId);
    else
        return User();
}

void ContactManager::addFriend(const QString &userId, const User &user)
{
    if(m_friendList.contains(userId))
        m_friendList[userId] = user;
    else
        m_friendList.insert(userId, user);
}

void ContactManager::removeFriend(const QString &userId)
{
    if(m_friendList.contains(userId))
        m_friendList.remove(userId);
}

void ContactManager::setFriendState(const QString& userId, bool state)
{
    if(m_friendList.contains(userId))
        m_friendList[userId].setOnlineState(state);
}

void ContactManager::setFriendAvatar(const QString &userId, const QString &avatar)
{
    if(m_friendList.contains(userId))
        m_friendList[userId].setAvatarPath(avatar);
}

User ContactManager::strangerById(const QString &userId) const
{
    if(m_strangerList.contains(userId))
        return m_strangerList.value(userId);
    else
        return User();
}

void ContactManager::addStranger(const QString &userId, const User &user)
{
    if(!m_strangerList.contains(userId))
        m_strangerList.insert(userId, user);
}

Group ContactManager::groupById(const QString &groupId) const
{
    if(m_groupList.contains(groupId))
        return m_groupList.value(groupId);
    else
        return Group();
}

void ContactManager::addGroupId(const QString &groupId)
{
    if(!m_groupIdList.contains(groupId))
        m_groupIdList.insert(groupId);
}

void ContactManager::addGroup(const QString &groupId, const Group &group)
{
    if(!m_groupList.contains(groupId))
        m_groupList.insert(groupId, group);
}

const QMap<QString, User> &ContactManager::friendList() const
{
    return m_friendList;
}

const QMap<QString, User> &ContactManager::strangerList() const
{
    return m_strangerList;
}

const QSet<QString> &ContactManager::groupIdList() const
{
    return m_groupIdList;
}

const QMap<QString, Group> &ContactManager::groupList() const
{
    return m_groupList;
}
