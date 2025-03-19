#ifndef FRIENDSMODEL_H
#define FRIENDSMODEL_H

#include <QAbstractListModel>

struct Friends{
    QString userName;
    QString id;
    QString lastMessage;
    QString avatarPath;
};

class FriendsModel: public QAbstractListModel {
    Q_OBJECT
public:
    explicit FriendsModel(QObject *parent = nullptr);

    enum Roles {
        UserNameRole = Qt::UserRole + 1,
        IdRole,
        LastMessageRole,
        AvatarRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    void addFriends_ToList(const QString &userName,const QString &id,const QString &lastMessage, const QString &avatarPath="");

    void removeItem(int row);

    void clear();

    Qt::ItemFlags flags(const QModelIndex &index) const override;


private:
    QVector<Friends> m_friends;
};

#endif // FRIENDSMODEL_H
