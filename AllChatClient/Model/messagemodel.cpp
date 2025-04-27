#include "messagemodel.h"

#include <Core/currentuser.h>


MessageModel::MessageModel(QObject *parent) : QAbstractListModel(parent) {}

int MessageModel::rowCount(const QModelIndex &parent) const{
    (void)parent;
    return m_messages.size();
}

QVariant MessageModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid() || index.row() >= m_messages.size())
        return QVariant();

    const ModelMessage &msg = m_messages[index.row()];
    switch (role) {
    case TypeRole: return msg.type;
    case ContentRole: return msg.content;
    case IsOutgoingRole: return msg.isOutgoing;
    case UserNameRole: return msg.userName;
    case AvatarRole: return msg.avatarPath;
    case TimeRole: return msg.time;
    default: return QVariant();
    }
}

void MessageModel::addMessage(const Message &msg)
{
    bool isOutgoing = (msg.getSenderId() == CurrentUser::getInstance()->get_userId());
    const QString senderName = msg.getSenderName();
    const QString avatarPath = msg.getAvatarPath();
    const QString time = msg.getTime();
    QString content = msg.getContent();

    MessageType type = getType(msg.getType());

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_messages.append({type, content, isOutgoing, senderName,avatarPath,time});
    endInsertRows();
}

// void MessageModel::addTextMessage(const QString &text, bool isOutgoing, const QString &userName, const QString &avatarPath, const QString &time) {
//     beginInsertRows(QModelIndex(), rowCount(), rowCount());
//     m_messages.append({MessageType::Text, text, "", isOutgoing, userName,avatarPath,time});
//     endInsertRows();
// }

// void MessageModel::addImageMessage(const QString &imagePath, bool isOutgoing, const QString &userName, const QString &avatarPath, const QString &time) {
//     beginInsertRows(QModelIndex(), rowCount(), rowCount());
//     m_messages.append({MessageType::Image, "", imagePath, isOutgoing, userName,avatarPath,time});
//     endInsertRows();
// }

void MessageModel::addOlderMessage(const Message &message)
{

    bool isOutgoing = (message.getSenderId() == CurrentUser::getInstance()->get_userId());
    const QString senderName = message.getSenderName();
    const QString avatarPath = message.getAvatarPath();
    const QString time = message.getTime();

    MessageType type = getType(message.getType());
    const QString content = message.getContent();

    beginInsertRows(QModelIndex(), 0, 0);
    m_messages.prepend({type, content, isOutgoing, senderName, avatarPath, time});
    endInsertRows();
}

// void MessageModel::addOlderTextMessage(const QString &text, bool isOutgoing, const QString &userName, const QString &avatarPath, const QString &time)
// {
//     beginInsertRows(QModelIndex(), 0, 0);
//     m_messages.prepend({MessageType::Text, text, "", isOutgoing, userName,avatarPath,time});
//     endInsertRows();
// }

// void MessageModel::addOlderImageMessage(const QString &imagePath, bool isOutgoing, const QString &userName, const QString &avatarPath, const QString &time)
// {
//     beginInsertRows(QModelIndex(), 0, 0); // 行号范围是 [0, 0]
//     m_messages.prepend({MessageType::Image, "", imagePath, isOutgoing, userName, avatarPath, time});
//     endInsertRows();
// }

void MessageModel::addTimeMessage(const QString &time)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_messages.append({MessageType::Time, "", true, "","",time});
    endInsertRows();
}

void MessageModel::addOlderTimeMessage(const QString &time)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_messages.prepend({MessageType::Time, "", true, "","",time});
    endInsertRows();
}

void MessageModel::clear() {
    beginResetModel();  // 通知视图即将更新
    m_messages.clear();
    endResetModel();    // 通知视图更新完成
}

QString MessageModel::get_olderTime()
{
    if(m_messages.isEmpty() || m_messages.first().type == Time)
    {
        return QString();
    }
    else
    {
        return m_messages.first().time;
    }
}

QString MessageModel::get_lastTime()
{
    if(m_messages.isEmpty() || m_messages.last().type == Time)
    {
        return QString();
    }
    else
    {
        return m_messages.last().time;
    }
}

MessageType MessageModel::getType(Message::MessageType type)
{
    switch (type) {
    case Message::Text:
        return MessageType::Text;
    case Message::Image:
        return MessageType::Image;
    case Message::File:
        return MessageType::File;
    }
    return MessageType::NONE;
}


