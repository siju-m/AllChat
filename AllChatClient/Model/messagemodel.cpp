#include "messagemodel.h"


MessageModel::MessageModel(QObject *parent) : QAbstractListModel(parent) {}

int MessageModel::rowCount(const QModelIndex &parent) const{
    (void)parent;
    return m_messages.size();
}

QVariant MessageModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid() || index.row() >= m_messages.size())
        return QVariant();

    const Message &msg = m_messages[index.row()];
    switch (role) {
    case TypeRole: return msg.type;
    case TextRole: return msg.text;
    case ImageRole: return msg.imagePath;
    case IsOutgoingRole: return msg.isOutgoing;
    case UserNameRole: return msg.userName;
    case AvatarRole: return msg.avatarPath;
    case TimeRole: return msg.time;
    default: return QVariant();
    }
}

void MessageModel::addTextMessage(const QString &text, bool isOutgoing, const QString &userName, const QString &avatarPath, const QString &time) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_messages.append({MessageType::Text, text, "", isOutgoing, userName,avatarPath,time});
    endInsertRows();
}

void MessageModel::addImageMessage(const QString &imagePath, bool isOutgoing, const QString &userName, const QString &avatarPath, const QString &time) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_messages.append({MessageType::Image, "", imagePath, isOutgoing, userName,avatarPath,time});
    endInsertRows();
}

void MessageModel::addTimeMessage(const QString &time)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_messages.append({MessageType::Time, "", "", true, "","",time});
    endInsertRows();
}

void MessageModel::clear() {
    beginResetModel();  // 通知视图即将更新
    m_messages.clear();
    endResetModel();    // 通知视图更新完成
}

void MessageModel::update_lastTempTime(const QString &targetId, const QString &lastMessageTime)
{
    m_temp_lastMsgTime[targetId]=lastMessageTime;
}

QString MessageModel::get_lastTempTime(const QString &targetId)
{
    if(!m_temp_lastMsgTime.contains(targetId)) return QString();
    return m_temp_lastMsgTime[targetId];
}


