#ifndef CHATHISTORYMANAGER_H
#define CHATHISTORYMANAGER_H

#include "Core/currentuser.h"
#include "Model/message.h"
#include <QObject>
#include <mutex>

class ContactManager;

class ChatHistoryManager : public QObject
{
    Q_OBJECT
public:
    static ChatHistoryManager *getInstance(){
        if(m_instance == nullptr){
            std::lock_guard<std::mutex> locker(m_mutex);
            if(m_instance == nullptr){
                m_instance = new ChatHistoryManager();
            }
        }
        return m_instance;
    }

    // 获取聊天记录在文件中的路径
    QString getChatHistoryFilePath();
    void addHistoryToFile(Message &msg);

    QString storeImage(const QByteArray &imageData);
    QString storeAvatar(QString userName, const QByteArray &imageData);
    QString storeFile(QString fileName, const QByteArray &fileData);

    void loadChatHistoryFromFile(QString targetId);//从文件中加载聊天记录
    QPair<QString,QString> getLastMessage(const QString &targetId);
    void initDatabase();

private:
    ChatHistoryManager(const ChatHistoryManager& obj) = delete;
    ChatHistoryManager& operator=(const ChatHistoryManager& obj) = delete;
    explicit ChatHistoryManager(QObject *parent = nullptr);
    QString saveImage(const QString &imageName, const QByteArray &imageData);

private:
    static ChatHistoryManager *m_instance;
    static std::mutex m_mutex;
    CurrentUser *m_user;
    ContactManager *m_contact_mg;
    QPair<QString, qulonglong> m_last_messageId;

signals:
    void addMessage_toList(const QVector<Message> &messages);
};

#endif // CHATHISTORYMANAGER_H
