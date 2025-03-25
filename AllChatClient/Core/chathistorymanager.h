#ifndef CHATHISTORYMANAGER_H
#define CHATHISTORYMANAGER_H

#include "Model/currentuser.h"
#include <QObject>

class ChatHistoryManager : public QObject
{
    Q_OBJECT
public:
    explicit ChatHistoryManager(QObject *parent = nullptr);

    QString getChatHistoryFilePath();//获取聊天记录在文件中的路径
    void storeMessageToFile(const QString &targetId, const QString &sender, const QString &message, const QString &msgTime);//将聊天记录存在文件中
    QString storeImageToFile(const QString &targetId, const QString &sender, const QByteArray &imageData, const QString &msgTime);
    QString storeImage(QString imageName,const QByteArray &imageData);

    void loadChatHistoryFromFile(QString targetId);//从文件中加载聊天记录
    QPair<QString,QString> getLastMessage(const QString &targetId);
private:
    CurrentUser *m_user;
signals:
    void addMessage_toList(const QString &text,const QString &chatId,const QString &senderId,const QString &time);//添加消息到聊天界面
    void addImage_toList(const QString &imagePath,const QString &chatId,const QString &senderId,const QString &time);//添加图片到聊天界面
};

#endif // CHATHISTORYMANAGER_H
