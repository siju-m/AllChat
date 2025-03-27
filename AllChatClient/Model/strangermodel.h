#ifndef STRANGERMODEL_H
#define STRANGERMODEL_H

#include <QAbstractListModel>

struct Strangers{
    QString userName;
    QString id;
    QString applyMessage;
    QString avatarPath;
};

class StrangerModel : public QAbstractListModel
{
public:
    explicit StrangerModel(QObject *parent = nullptr);

    enum Roles {
        UserNameRole = Qt::UserRole + 1,
        IdRole,
        ApplyMessageRole,
        AvatarRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void addFriends_ToList(const QString &userName,const QString &id,const QString &applyMessage="", const QString &avatarPath="");

    void removeItem(int row);
    void clear();
private:
    QVector<Strangers> m_strangers;
};

#endif // STRANGERMODEL_H
