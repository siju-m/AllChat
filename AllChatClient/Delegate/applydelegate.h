#ifndef APPLYDELEGATE_H
#define APPLYDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>

class ApplyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ApplyDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
signals:
    void applyResult(const QString &id,const int &row);
protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

#endif // APPLYDELEGATE_H
