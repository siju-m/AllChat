#ifndef CONTACTMANAGER_H
#define CONTACTMANAGER_H

#include "Model/group.h"
#include "Model/user.h"
#include "Utils/CustomTypes.h"
#include <mutex>
#include <QString>
#include <QMap>
#include <QSet>
#include <QObject>

class ChatHistoryManager;

class ContactManager: public QObject
{
    Q_OBJECT
public:
    ContactManager(const ContactManager& obj) = delete;
    ContactManager& operator=(const ContactManager& obj) = delete;
    static ContactManager* getInstance(){
        if(m_instance == nullptr){
            std::lock_guard<std::mutex> locker(m_mutex);
            if(m_instance == nullptr){
                m_instance = new ContactManager();
            }
        }
        return m_instance;
    }

    void updateUserList(const QMap<QString, QString> &newUserList,const QMap<QString,QByteArray> &new_idAvatar);

    bool isFriend(const QString& userId) const;
    bool isStranger(const QString& userId) const;
    bool isInGroup(const QString& groupId) const;

    User friendById(const QString& userId) const;
    void addFriend(const QString& userId, const User &user);
    void removeFriend(const QString& userId);
    void setFriendState(const QString& userId, bool state);
    void setFriendAvatar(const QString& userId, const QString& avatar);

    User strangerById(const QString& userId) const;
    void addStranger(const QString& userId, const User &user);

    Group groupById(const QString& groupId) const;
    void addGroupId(const QString& groupId);
    void addGroup(const QString& groupId, const Group& group);

    const QMap<QString, User>& friendList() const;
    const QMap<QString, User>& strangerList() const;
    const QSet<QString>& groupIdList() const;
    const QMap<QString, Group>& groupList() const;

signals:
    void clearModel();
    void addChatModel(const QString &userName,const QString &id,const QString &lastMessage,const QString &lastMessageTime,const int &unreadMsgNum, const QString &avatarPath="");
    void addFriendModel(const QString &userName,const QString &id,const StateEnum::onlineState_type &onlineState, const QString &avatarPath="");

private:
    ContactManager(QObject *parent = nullptr);

private:
    static ContactManager *m_instance;
    static std::mutex m_mutex;

    QMap<QString, User> m_friendList;
    QMap<QString, User> m_strangerList;
    QSet<QString> m_groupIdList;
    QMap<QString, Group> m_groupList;
    ChatHistoryManager *m_historyManager;
};

#endif // CONTACTMANAGER_H
