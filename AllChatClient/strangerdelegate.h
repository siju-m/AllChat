#ifndef STRANGERDELEGATE_H
#define STRANGERDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>

class StrangerDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit StrangerDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
signals:
    void applyClicked(const QString &id,const int &row);
protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

#endif // STRANGERDELEGATE_H
