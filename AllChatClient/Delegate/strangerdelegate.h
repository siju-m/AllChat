#ifndef STRANGERDELEGATE_H
#define STRANGERDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>

class StrangerDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    enum ButtonState{
        Normal,
        Hover,
        Pressed,
        Released
    };
    explicit StrangerDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void applyClicked(const QString &id,const int &row);
    void sendClicked(const QString &id);

private:
    void draw_avatar(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void draw_userName(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void draw_addBtn(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void draw_sendBtn(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    mutable QMap<QPersistentModelIndex, ButtonState> m_buttonState;
};

#endif // STRANGERDELEGATE_H
