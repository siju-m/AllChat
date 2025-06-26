#ifndef FRIENDSMODEL_H
#define FRIENDSMODEL_H

#include "Utils/CustomTypes.h"
#include <QAbstractListModel>

struct Friends{
    QString userName;
    QString id;
    StateEnum::onlineState_type onlineState = StateEnum::OFFLINE;
    QString avatarPath;
};

//好友数据
class FriendsModel: public QAbstractListModel {
    Q_OBJECT
public:
    explicit FriendsModel(QObject *parent = nullptr);

    enum Roles {
        UserNameRole = Qt::UserRole + 1,
        IdRole,
        OnlineRole,
        AvatarRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    void addFriends_ToList(const QString &userName,const QString &id,const StateEnum::onlineState_type &onlineState, const QString &avatarPath="");

    void updateOnlineState(QString targetId,const StateEnum::onlineState_type &onlineState);

    void removeItem(int row);
    void removeItem(const QString &id);

    void clear();

    Qt::ItemFlags flags(const QModelIndex &index) const override;
private:
    void loadImage(const QString &imagePath, int index);

private:
    QVector<Friends> m_friends;

    QHash<QString, QPixmap> avatarCache;
};

#endif // FRIENDSMODEL_H
