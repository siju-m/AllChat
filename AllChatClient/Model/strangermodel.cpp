#include "strangermodel.h"

StrangerModel::StrangerModel(QObject *parent)
    : QAbstractListModel{parent}
{}

int StrangerModel::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    return m_strangers.size();
}

QVariant StrangerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_strangers.size())
        return QVariant();

    const Strangers &msg = m_strangers[index.row()];
    switch (role) {
    case UserNameRole: return msg.userName;
    case IdRole: return msg.id;
    case IsFriendRole: return msg.isFriend;
    case AvatarRole: return msg.avatarPath;
    default: return QVariant();
    }
}

void StrangerModel::addFriends_ToList(const QString &userName, const QString &id, bool isFriend, const QString &avatarPath)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_strangers.append({userName,id,isFriend,avatarPath});
    endInsertRows();
}

void StrangerModel::removeItem(int row)
{

    if(row<0 || row>=m_strangers.size())
        return;
    beginRemoveRows(QModelIndex(),row,row);
    m_strangers.remove(row);
    endRemoveRows();
}

void StrangerModel::clear()
{
    if(!m_strangers.size()) return;

    beginRemoveRows(QModelIndex(),0,m_strangers.size()-1);
    m_strangers.clear();
    endRemoveRows();
}

bool StrangerModel::isEmpty()
{
    return m_strangers.isEmpty();
}
