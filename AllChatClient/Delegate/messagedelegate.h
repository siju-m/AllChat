#ifndef MESSAGEDELEGATE_H
#define MESSAGEDELEGATE_H

#include <QStyledItemDelegate>
#include <QTextDocument>
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QPainterPath>

class MessageDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit MessageDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
protected:

signals:
    void imageClicked(const QPixmap &image); // 点击图片时触发信号
    void fileClicked(const QString &file);

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


    QRect calculateImageRect(const QStyleOptionViewItem &option,
                             bool isOutgoing,
                             const QSize &imageSize) const;

    QRect getAvatarRect(const QStyleOptionViewItem &option, bool isOutgoing) const;
    QSize getImageSize(const QModelIndex &index) const;
    QRect getFileRect(const QStyleOptionViewItem &option, bool isOutgoing) const;

    QString formatFileSize(qint64 size) const;
    QString caculate_time(const QString &lastMsgTime) const;

    bool isClickOnImage(const QPoint &pos, const QModelIndex &index, const QStyleOptionViewItem &option) const;
    bool isClickOnFile(const QPoint &pos, const QModelIndex &index, const QStyleOptionViewItem &option) const;

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

};
#endif // MESSAGEDELEGATE_H
