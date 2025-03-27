#include "friendsmodel.h"


FriendsModel::FriendsModel(QObject *parent): QAbstractListModel(parent)
{

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
    case AvatarRole: return msg.avatarPath;
    default: return QVariant();
    }
}

void FriendsModel::addFriends_ToList(const QString &userName, const QString &id, const StateEnum::onlineState_type &onlineState, const QString &avatarPath)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_friends.append({userName, id, onlineState,avatarPath});
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
