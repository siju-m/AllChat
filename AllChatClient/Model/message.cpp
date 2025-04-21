#include "message.h"

#include <QJsonDocument>
#include <QJsonObject>

#include <Core/currentuser.h>


Message::Message(MessageType type, const QString &data, const QString &time, const User &sender, const QString &chatId):
    m_type(type),
    m_msgTime(time),
    m_sender(sender),
    m_chatId(chatId),
    m_plainText(QString())
{
    if(type == Text)
    {
        m_text = data;
        m_plainText = data;
    }
    else if(type == Image)
    {
        m_imagePath = data;
        m_plainText = "图片";
    }

    if(CurrentUser::getInstance()->getGroupsIdList().contains(chatId))
    {
        m_chatType = GROUP;
    }
    else
    {
        m_chatType = PRIVATE;
    }
}

Message::MessageType Message::getType() const
{
    return m_type;
}

QString Message::getText() const
{
    return m_text;
}

void Message::setText(const QString &text)
{
    m_text = text;
}

QString Message::getImage() const
{
    return m_imagePath;
}

void Message::setImage(const QString &path)
{
    m_imagePath = path;
}

QString Message::getTime() const
{
    return m_msgTime;
}

void Message::setTime(const QString &time)
{
    m_msgTime = time;
}

QByteArray Message::jsonData() const
{
    QJsonObject obj;
    QJsonObject format;
    obj["name"] = m_sender.getUserName();
    obj["id"] = m_sender.getUserId();
    obj["time"] = m_msgTime;

    if(m_type == Text){
        obj["message"] = m_text;
        format["kinds"] = "text";
    }
    else if(m_type == Image){
        obj["imagePath"] = m_imagePath;
        format["kinds"] = "image";
    }

    format["data"] = obj;
    QJsonDocument doc(format);
    return doc.toJson(QJsonDocument::Compact);
}

QString Message::getChatId() const
{
    return m_chatId;
}

void Message::setChatId(const QString &id)
{
    m_chatId = id;
}

Message::ChatType Message::getChatType()
{
    return m_chatType;
}

QString Message::getAvatarPath() const
{
    return m_sender.getAvatarPath();
}

QString Message::getSenderId() const
{
    return m_sender.getUserId();
}

QString Message::getSenderName() const
{
    return m_sender.getUserName();
}

QString Message::getPlainText() const
{
    return m_plainText;
}
