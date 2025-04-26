#ifndef MESSAGEDELEGATE_H
#define MESSAGEDELEGATE_H

#include <QStyledItemDelegate>
#include <QTextDocument>
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QPainterPath>
#include "Model/messagemodel.h"

class MessageDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit MessageDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool isClickOnImage(const QPoint &pos, const QModelIndex &index, const QStyleOptionViewItem &option) const;

    qint32 getListViewSpacing();
protected:

signals:
    void imageClicked(const QPixmap &image); // 点击图片时触发信号
private:
    void drawAvatar(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawUserName(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawImageMessage(QPainter *painter,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index, bool isOutgoing) const;
    void drawTextMessage(QPainter *painter,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index, bool isOutgoing) const;
    void drawFileMessage(QPainter *painter,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index, bool isOutgoing) const;
    void drawTime(QPainter *painter,
                  const QStyleOptionViewItem &option,
                  const QModelIndex &index) const;

    QString caculate_time(const QString &lastMsgTime) const;
    QRect calculateImageRect(const QStyleOptionViewItem &option,
                             bool isOutgoing,
                             const QSize &imageSize) const;

    QRect getAvatarRect(const QStyleOptionViewItem &option, bool isOutgoing) const;
    QSize getImageSize(const QModelIndex &index) const;

    QString formatFileSize(qint64 size) const;

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    //将间距值存在类里面方便调整option.rect之间的差异
    qint32 m_listView_Spacing=10;
};
#endif // MESSAGEDELEGATE_H
