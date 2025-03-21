#include "strangerdelegate.h"

#include <Model\FriendsModel.h>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QTextDocument>

StrangerDelegate::StrangerDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

void StrangerDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);          // 抗锯齿
    QString userName = index.data(FriendsModel::UserNameRole).toString();
    // QString lastMessage = index.data(FriendsModel::LastMessageRole).toString();

    QRect itemRect = option.rect;

    //绘制头像
    QRect avatarRect =  QRect(itemRect.left() + 10, itemRect.top()+10, 40, 40);
    painter->setBrush(QColor(200, 200, 200));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(avatarRect);


    // 绘制用户名
    QTextDocument doc;
    doc.setHtml(userName);
    doc.setTextWidth(itemRect.width()); // 设置最大宽度
    int nameWidth = doc.idealWidth(); // 获取文本的实际宽度
    QRect usernameRect = QRect(itemRect.left()+60, itemRect.top()+20, nameWidth, 20);
    painter->setPen(Qt::black);
    QFont font("Arial", 14, QFont::Bold); // 字体为 Arial，大小为 20，加粗
    painter->setFont(font);
    painter->drawText(usernameRect, Qt::AlignLeft | Qt::AlignTop, userName);

    //同意按钮
    QRect applyRect =  QRect(itemRect.right() -60, itemRect.top()+20, 50, 30);
    painter->setBrush(QColor(Qt::green));
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(applyRect,10,10);
    painter->setPen(Qt::black);
    painter->drawText(applyRect.adjusted(6,3,0,0), Qt::AlignLeft | Qt::AlignTop, "添加");

    painter->restore();
}

QSize StrangerDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    (void)index;
    return QSize(option.rect.width(),60);
}

bool StrangerDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        // 获取鼠标点击位置
        QPoint clickPos = mouseEvent->pos();
        QRect itemRect = option.rect;
        QRect applyRect =  QRect(itemRect.right() -60, itemRect.top()+20, 50, 30);
        QString id = index.data(FriendsModel::IdRole).toString();
        if (applyRect.contains(clickPos)) {
            // 处理按钮点击逻辑
            emit applyClicked(id,index.row());
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
