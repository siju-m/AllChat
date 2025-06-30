#include "friendsmodel.h"
#include <QPixmap>
#include <qtconcurrentrun.h>
#include <Core/avatarmanager.h>
#include <Utils/commonutil.h>


FriendsModel::FriendsModel(QObject *parent): QAbstractListModel(parent)
{
    // CommonUtil::bindModelToAvatarUpdates(this, [=](int row){return m_friends[row].id;}, AvatarRole);
}

int FriendsModel::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    return m_friends.size();
}

QVariant FriendsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_friends.size())
        return QVariant();

    const Friends &msg = m_friends[index.row()];
    switch (role) {
    case UserNameRole: return msg.userName;
    case IdRole: return msg.id;
    case OnlineRole: return msg.onlineState;
    // case AvatarRole: return avatarCache.contains(msg.avatarPath)?avatarCache.value(msg.avatarPath):QPixmap();
    case AvatarRole: {
        QPixmap *pix = AvatarManager::getInstance()->getAvatar(msg.id);
        if (pix != nullptr) {
            return *pix;
        } else {
            // 异步请求头像，加载完后刷新当前项
            AvatarManager::getInstance()->getAvatar(msg.id, [=](const QPixmap &pix) {
                FriendsModel* that = const_cast<FriendsModel*>(this);
                QMetaObject::invokeMethod(that, [that, index]() {
                    emit that->dataChanged(index, index, {AvatarRole});
                }, Qt::QueuedConnection);
            });
            return QPixmap(); // 返回默认图或空图
        }
    };
    default: return QVariant();
    }
}

void FriendsModel::addFriends_ToList(const QString &userName, const QString &id, const StateEnum::onlineState_type &onlineState)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    // loadImage(avatarPath, rowCount());
    // loadAvatar(id, rowCount());
    m_friends.append({userName, id, onlineState});
    endInsertRows();
}

void FriendsModel::updateOnlineState(QString targetId,const StateEnum::onlineState_type &onlineState)
{
    for(int i=0;i<m_friends.size();++i){
        if(m_friends[i].id == targetId){
            m_friends[i].onlineState = onlineState;
            QModelIndex index = this->index(i,0);
            emit dataChanged(index,index);
        }
    }
}

void FriendsModel::removeItem(int row)
{
    if (row < 0 || row >= m_friends.size()) return;

    beginRemoveRows(QModelIndex(), row, row); // 通知视图删除开始
    m_friends.removeAt(row);  // 移除数据
    endRemoveRows(); // 通知视图删除完成
}

void FriendsModel::removeItem(const QString &id)
{
    int row=-1;
    for(int i=0;i<m_friends.size();++i){
        if(m_friends[i].id==id){
            row = i;
            break;
        }
    }
    if(row!=-1){
        beginRemoveColumns(QModelIndex(), row, row);
        m_friends.remove(row);
        endRemoveRows();
    }
}

void FriendsModel::clear()
{
    if (m_friends.isEmpty()) return;

    // avatarCache.clear();
    beginRemoveRows(QModelIndex(), 0, m_friends.size()-1); // 通知视图删除开始
    m_friends.clear();
    endRemoveRows(); // 通知视图删除完成
}

Qt::ItemFlags FriendsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

// void FriendsModel::loadAvatar(const QString &userId, int index)
// {
//     AvatarManager::getInstance()->getAvatar(userId, [=](const QPixmap &pix){
//         avatarCache.insert(userId, pix);
//         emit this->dataChanged(this->index(index, 0), this->index(index, 0));
//     });
// }
