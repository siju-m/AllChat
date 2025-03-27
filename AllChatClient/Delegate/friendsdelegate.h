#ifndef FRIENDSDELEGATE_H
#define FRIENDSDELEGATE_H

#include <QStyledItemDelegate>

class FriendsDelegate : public QStyledItemDelegate
{
public:
    explicit FriendsDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
    void draw_back(QPainter *painter, const QStyleOptionViewItem &option) const;
    void draw_avatar(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void draw_name(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void draw_onlineState(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // FRIENDSDELEGATE_H
