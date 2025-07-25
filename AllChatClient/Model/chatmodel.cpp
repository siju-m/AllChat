#include "chatmodel.h"
#include "Core/avatarmanager.h"

#include <QDateTime>
#include <qtconcurrentrun.h>

ChatModel::ChatModel(QObject *parent)
    : QAbstractListModel{parent}
{
    // CommonUtil::bindModelToAvatarUpdates(this, [=](int row){return m_ids[row];}, AvatarRole);
}

int ChatModel::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    return m_id_chats.size();
}

QVariant ChatModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_id_chats.size())
        return QVariant();
    const Chats &msg = m_id_chats[m_ids[index.row()]];
    switch (role) {
    case UserNameRole: return msg.userName;
    case IdRole: return m_ids[index.row()];
    case LastMessageRole: return msg.lastMessage;
    case LastMessageTimeRole: return msg.lastMessageTime;
    case UnreadMsgNumRole: return msg.unreadMsgNum;
    case AvatarRole:{
        QPixmap *pix = AvatarManager::getInstance()->getAvatar(m_ids[index.row()]);
        if (pix != nullptr) {
            return *pix;
        } else {
            // 异步请求头像，加载完后刷新当前项
            AvatarManager::getInstance()->getAvatar(m_ids[index.row()], [=](const QPixmap &pix) {
                ChatModel* that = const_cast<ChatModel*>(this);
                QMetaObject::invokeMethod(that, [that, index]() {
                    emit that->dataChanged(index, index, {AvatarRole});
                }, Qt::QueuedConnection);
            });
            return QPixmap(); // 返回默认图或空图
        }
    }
    // case AvatarRole: return avatarCache.contains(msg.avatarPath)?avatarCache.value(msg.avatarPath):QPixmap();
    default: return QVariant();
    }
}

void ChatModel::addChat_toList(const QString &userName, const QString &id, const QString &lastMessage, const QString &lastMessageTime,const int &unreadMsgNum)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    // loadImage(avatarPath, rowCount());
    // loadAvatar(id, rowCount());

    m_ids.append(id);
    m_id_chats[id] = {userName, lastMessage,lastMessageTime,unreadMsgNum};
    sortListByTime();

    endInsertRows();
}

void ChatModel::updateLastMessage(const QString targetId, const QString &message, const QString &lastMessageTime)
{
    if(!m_id_chats.contains(targetId)) return;
    int targetRow  = m_ids.indexOf(targetId);
    if(targetRow==-1) return;
    emit layoutAboutToBeChanged();//父类信号，通知内部数据的顺序改变

    m_id_chats[targetId].lastMessage = message;
    m_id_chats[targetId].lastMessageTime = lastMessageTime;

    //只需要把这个对象移到最前面就行了
    sortToFirst(targetRow);
    emit sortEnd();

    emit layoutChanged();
}

void ChatModel::removeItem(const QString &targetId)
{
    if(!m_id_chats.contains(targetId)) return;
    int row = m_ids.indexOf(targetId);
    beginRemoveRows(QModelIndex(), row, row); // 通知视图删除开始
    m_id_chats.remove(targetId);
    m_ids.remove(row);
    endRemoveRows(); // 通知视图删除完成

}

void ChatModel::clear()
{
    if (m_id_chats.isEmpty()) return;

    // avatarCache.clear();
    beginRemoveRows(QModelIndex(), 0, m_id_chats.size()-1); // 通知视图删除开始
    m_id_chats.clear();
    m_ids.clear();
    endRemoveRows(); // 通知视图删除完成
}

void ChatModel::sortToFirst(const int &row)
{
    QString tmp;
    for(int i=row;i>0;--i){
        tmp = m_ids[i];
        m_ids[i]=m_ids[i-1];
        m_ids[i-1]=tmp;
    }
}

QString ChatModel::getLastMsgTime(const QString &targetId)
{
    if(!m_id_chats.contains(targetId)) return QString();
    return m_id_chats[targetId].lastMessageTime;
}

void ChatModel::sortListByTime()
{
    std::sort(m_ids.begin(),m_ids.end(),[this](const QString &a, const QString &b) {
        return this->compareByTime(a, b);
    });
    emit sortEnd();//通知选择排序后的第一个
}

bool ChatModel::compareByTime(const QString &a, const QString &b)
{
    if(m_id_chats[a].lastMessageTime.isEmpty())
        return false;
    if(m_id_chats[b].lastMessageTime.isEmpty())
        return true;
    QDateTime aDateTime = QDateTime::fromString(m_id_chats[a].lastMessageTime,"yyyy-MM-dd hh:mm:ss");
    QDateTime bDateTime = QDateTime::fromString(m_id_chats[b].lastMessageTime,"yyyy-MM-dd hh:mm:ss");
    return aDateTime >bDateTime;
}

int ChatModel::get_currentChatRow()
{
    return m_ids.indexOf(m_current_chatId);
}

int ChatModel::get_rowById(const QString &targetId) const
{
    return m_ids.indexOf(targetId);
}

void ChatModel::set_currentChatId(const QString &id)
{
    m_current_chatId = id;
}

void ChatModel::add_unreadMsgNum(const QString &id)
{
    if(!m_id_chats.contains(id)) return;
    ++m_id_chats[id].unreadMsgNum;
    int row = m_ids.indexOf(id);
    QModelIndex index = this->index(row,0);
    emit dataChanged(index,index,{row});
}

void ChatModel::clear_unreadMsgNum(const QString &id)
{
    if(!m_id_chats.contains(id)) return;
    m_id_chats[id].unreadMsgNum = 0;
    int row = m_ids.indexOf(id);
    QModelIndex index = this->index(row,0);
    emit dataChanged(index,index,{row});
}


// void ChatModel::loadAvatar(const QString &userId, int index)
// {
//     AvatarManager::getInstance()->getAvatar(userId, [=](const QPixmap &pix){
//         avatarCache.insert(userId, pix);
//         emit this->dataChanged(this->index(index, 0), this->index(index, 0));
//     });
// }
