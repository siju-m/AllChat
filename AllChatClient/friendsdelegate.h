#ifndef FRIENDSDELEGATE_H
#define FRIENDSDELEGATE_H

#include <QStyledItemDelegate>

class FriendsDelegate : public QStyledItemDelegate
{
public:
    explicit FriendsDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // FRIENDSDELEGATE_H
