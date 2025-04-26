#ifndef MESSAGE_H
#define MESSAGE_H

#include "Model/user.h"
#include <QDateTime>
#include <QString>


class Message
{
public:
    enum MessageType{Text, Image, File};
    enum ChatType{PRIVATE, GROUP};
    Message();
    Message(MessageType type, const QString &data, const QString &time, const User &sender, const QString &chatId);

    MessageType getType() const;
    QString getType_string() const;

    QString getContent() const;
    // 文本消息
    // QString getText() const;
    // void setText(const QString &text);

    // 图片消息路径
    // QString getImage() const;
    // void setImage(const QString &path);

    QString getTime() const;
    QDateTime getDateTime() const;
    void setTime(const QString &time);

    // 聊天记录的json二进制
    // QByteArray jsonData() const;

    // 消息所属的聊天id
    QString getChatId() const;
    void setChatId(const QString &id);

    ChatType getChatType();

    QString getAvatarPath() const;
    QString getSenderId() const;
    QString getSenderName() const;
    QString getPlainText() const;
private:
    MessageType m_type;
    // QString m_text;
    // QString m_imagePath;
    QString m_content;
    QString m_msgTime;
    User m_sender;
    QString m_chatId;
    ChatType m_chatType;
    // 显示在未读消息的纯文本
    QString m_plainText;
};

#endif // MESSAGE_H
