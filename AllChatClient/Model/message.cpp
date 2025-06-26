#include "message.h"

#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>

#include <Core/currentuser.h>
#include <Core/contactmanager.h>


Message::Message()
{}

Message::Message(MessageType type, const QString &data, const QString &time, const User &sender, const QString &chatId):
    m_type(type),
    m_content(data),
    m_msgTime(time),
    m_sender(sender),
    m_chatId(chatId),
    m_plainText(QString())
{
    switch (type) {
        case Text:
            m_plainText = data;
            break;
        case Image:
            m_plainText = "图片";
            break;
        case File:
        {
            QFileInfo fileInfo(data);
            m_plainText = fileInfo.fileName();
            break;
        }
        default:
            break;
    }

    if(ContactManager::getInstance()->isInGroup(chatId))
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

QString Message::getTypeString() const
{
    switch (m_type) {
    case Text:
        return "text";
    case Image:
        return "image";
    case File:
        return "file";
    default:
        return "";
    }
}

QString Message::getContent() const
{
    return m_content;
}

QString Message::getTime() const
{
    return m_msgTime;
}

QDateTime Message::getDateTime() const
{
    return QDateTime::fromString(m_msgTime,"yyyy-MM-dd hh:mm:ss");
}

void Message::setTime(const QString &time)
{
    m_msgTime = time;
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
