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
    QString storeImage(QString imageName,const QByteArray &imageData);

    void loadChatHistoryFromFile(QString targetId);//从文件中加载聊天记录
    QPair<QString,QString> getLastMessage(const QString &targetId);
private:
    CurrentUser *m_user;
signals:
    // void addMessage_toList(const QString &text,const QString &chatId,const QString &senderId,const QString &time);//添加消息到聊天界面
    // void addImage_toList(const QString &imagePath,const QString &chatId,const QString &senderId,const QString &time);//添加图片到聊天界面
    void addMessage_toList(const Message &message);
};

#endif // CHATHISTORYMANAGER_H
