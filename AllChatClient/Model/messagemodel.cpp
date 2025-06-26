#include "messagemodel.h"
#include "Core/currentuser.h"

#include <QtConcurrent>

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
    case ContentRole:{
        if(msg.type!=Image)
            return msg.content;
        else
            return imageCache.contains(msg.content)?imageCache.value(msg.content):QPixmap();
    }
    case IsOutgoingRole: return msg.isOutgoing;
    case UserNameRole: return msg.userName;
    case AvatarRole: return avatarCache.contains(msg.avatarPath)?avatarCache.value(msg.avatarPath):QPixmap();
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

    loadAvatar(avatarPath, rowCount());

    if(type==Image){
        loadImage(content, rowCount());
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_messages.append({type, content, isOutgoing, senderName,avatarPath,time});
    endInsertRows();
}

void MessageModel::addOlderMessage(const Message &message)
{

    bool isOutgoing = (message.getSenderId() == CurrentUser::getInstance()->get_userId());
    const QString senderName = message.getSenderName();
    const QString avatarPath = message.getAvatarPath();
    const QString time = message.getTime();

    MessageType type = getType(message.getType());
    const QString content = message.getContent();

    loadAvatar(avatarPath, 0);

    if(type==Image){
        loadImage(content, 0);
    }

    beginInsertRows(QModelIndex(), 0, 0);
    m_messages.prepend({type, content, isOutgoing, senderName, avatarPath, time});
    endInsertRows();
}

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
    avatarCache.clear();
    imageCache.clear();
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

void MessageModel::loadAvatar(const QString &avatarPath, int index)
{
    if(!avatarCache.contains(avatarPath)){
        (void)QtConcurrent::run([=]() {
            QPixmap asyncPixmap;
            asyncPixmap.load(avatarPath);
            if (!asyncPixmap.isNull()) {
                QMetaObject::invokeMethod(const_cast<MessageModel *>(this), [=]() {
                    avatarCache.insert(avatarPath, asyncPixmap);
                    emit this->dataChanged(this->index(index, 0), this->index(index, 0));
                }, Qt::QueuedConnection);
            }
        });
    }
}

void MessageModel::loadImage(const QString &imagePath, int index)
{
    if(!imageCache.contains(imagePath)){
        (void)QtConcurrent::run([=]() {
            QPixmap asyncPixmap;
            asyncPixmap.load(imagePath);
            if (!asyncPixmap.isNull()) {
                QMetaObject::invokeMethod(const_cast<MessageModel *>(this), [=]() {
                    imageCache.insert(imagePath, asyncPixmap);
                    emit this->dataChanged(this->index(index, 0), this->index(index, 0));
                }, Qt::QueuedConnection);
            }
        });
    }
}


