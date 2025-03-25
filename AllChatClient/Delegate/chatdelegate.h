#ifndef CHATDELEGATE_H
#define CHATDELEGATE_H

#include <QStyledItemDelegate>

class ChatDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ChatDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QString caculate_time(const QString &lastMsgTime) const;
private:
    void draw_background(QPainter *painter, const QStyleOptionViewItem &option) const;
    void draw_avatar(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void draw_userName(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void draw_lastMsg(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void draw_lastMsgTime(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void draw_unreadMsgNum(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // CHATDELEGATE_H
