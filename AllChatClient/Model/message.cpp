#include "message.h"

#include <QJsonDocument>
#include <QJsonObject>


Message::Message(MessageType type, const QString &data, const QString &time, const User &sender, const QString &chatId):
    m_type(type),
    m_msgTime(time),
    m_sender(sender),
    m_chatId(chatId)
{
    if(type == Text){
        m_text = data;
    }else if(type == Image){
        m_imagePath = data;
    }
}

QString Message::getText()
{
    return m_text;
}

void Message::setText(QString text)
{
    m_text = text;
}

QString Message::getImage()
{
    return m_imagePath;
}

void Message::setImage(QString path)
{
    m_imagePath = path;
}

QString Message::getTime()
{
    return m_msgTime;
}

void Message::setTime(QString time)
{
    m_msgTime = time;
}

QByteArray Message::jsonData()
{
    QJsonObject obj;
    QJsonObject format;
    obj["name"] = m_sender.getUserName();
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

QString Message::getChatId()
{
    return m_chatId;
}

void Message::setChatId(QString id)
{
    m_chatId = id;
}
