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
    case LastMessageRole: return msg.lastMessage;
    default: return QVariant();
    }
}

void FriendsModel::addFriends_ToList( const QString &userName,const QString &id,const QString &lastMessage)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_friends.append({userName, id, lastMessage});
    endInsertRows();
}

void FriendsModel::removeItem(int row)
{
    if (row < 0 || row >= m_friends.size()) return;

    beginRemoveRows(QModelIndex(), row, row); // 通知视图删除开始
    m_friends.removeAt(row);  // 移除数据
    endRemoveRows(); // 通知视图删除完成
}

void FriendsModel::clear()
{
    m_friends.clear();
}

Qt::ItemFlags FriendsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
