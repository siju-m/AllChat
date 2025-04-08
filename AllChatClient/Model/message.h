#ifndef MESSAGE_H
#define MESSAGE_H

#include "Model/user.h"
#include <QString>


class Message
{
public:
    enum MessageType{Text, Image};
    Message(MessageType type, const QString &data, const QString &time, const User &sender, const QString &chatId);

    // 文本消息
    QString getText();
    void setText(QString text);

    // 图片消息路径
    QString getImage();
    void setImage(QString path);

    QString getTime();
    void setTime(QString time);

    // 聊天记录的json二进制
    QByteArray jsonData();

    // 消息所属的聊天id
    QString getChatId();
    void setChatId(QString id);
private:
    MessageType m_type;
    QString m_text;
    QString m_imagePath;
    QString m_msgTime;
    User m_sender;
    // QString m_sender;
    QString m_chatId;
};

#endif // MESSAGE_H
