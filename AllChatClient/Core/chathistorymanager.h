#ifndef CHATHISTORYMANAGER_H
#define CHATHISTORYMANAGER_H

#include "Core/currentuser.h"
#include "Model/message.h"
#include <QObject>

class ChatHistoryManager : public QObject
{
    Q_OBJECT
public:
    explicit ChatHistoryManager(QObject *parent = nullptr);

    // 获取聊天记录在文件中的路径
    QString getChatHistoryFilePath();
    // 获取聊天文件夹路径
    QString getChatFilePath(QString chatId);
    void addHistoryToFile(Message &msg);

    QString storeImage(QString imageName, const QByteArray &imageData);
    QString storeFile(QString fileName, const QByteArray &fileData);

    void loadChatHistoryFromFile(QString targetId);//从文件中加载聊天记录
    QPair<QString,QString> getLastMessage(const QString &targetId);
    void initDatabase();

private:

private:
    CurrentUser *m_user;
    QPair<QString, qulonglong> m_last_messageId;

signals:
    void addMessage_toList(const QVector<Message> &messages);
};

#endif // CHATHISTORYMANAGER_H
