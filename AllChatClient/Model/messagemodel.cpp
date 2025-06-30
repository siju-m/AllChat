#include "messagemodel.h"
#include "Core/currentuser.h"

#include <QtConcurrent>

#include <Core/avatarmanager.h>

MessageModel::MessageModel(QObject *parent) : QAbstractListModel(parent) {
    // CommonUtil::bindModelToAvatarUpdates(this, [=](int row){return m_messages[row].senderId;}, AvatarRole);
}

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
    case AvatarRole:{
        QPixmap *pix = AvatarManager::getInstance()->getAvatar(msg.senderId);
        if (pix != nullptr) {
            return *pix;
        } else {
            // 异步请求头像，加载完后刷新当前项
            AvatarManager::getInstance()->getAvatar(msg.senderId, [=](const QPixmap &pix) {
                MessageModel* that = const_cast<MessageModel*>(this);
                QMetaObject::invokeMethod(that, [that, index]() {
                    emit that->dataChanged(index, index, {AvatarRole});
                }, Qt::QueuedConnection);
            });
            return QPixmap(); // 返回默认图或空图
        }
    };
    case TimeRole: return msg.time;
    default: return QVariant();
    }
}

void MessageModel::addMessage(const Message &msg)
{
    const QString senderId = msg.getSenderId();
    bool isOutgoing = (senderId == CurrentUser::getInstance()->get_userId());
    const QString senderName = msg.getSenderName();
    const QString time = msg.getTime();
    QString content = msg.getContent();

    MessageType type = getType(msg.getType());

    // loadAvatar(senderId, rowCount());

    if(type==Image){
        loadImage(content, rowCount());
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_messages.append({type, content, isOutgoing, senderName, senderId, time});
    endInsertRows();
}

void MessageModel::addOlderMessage(const Message &message)
{
    const QString senderId = message.getSenderId();
    bool isOutgoing = (senderId == CurrentUser::getInstance()->get_userId());
    const QString senderName = message.getSenderName();
    const QString time = message.getTime();

    MessageType type = getType(message.getType());
    const QString content = message.getContent();

    // loadAvatar(senderId, 0);

    if(type==Image){
        loadImage(content, 0);
    }

    beginInsertRows(QModelIndex(), 0, 0);
    m_messages.prepend({type, content, isOutgoing, senderName, senderId, time});
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
    // avatarCache.clear();
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

// void MessageModel::loadAvatar(const QString &userId, int index)
// {
//     AvatarManager::getInstance()->getAvatar(userId, [=](const QPixmap &pix){
//         avatarCache.insert(userId, pix);
//         emit this->dataChanged(this->index(index, 0), this->index(index, 0));
//     });
// }

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


