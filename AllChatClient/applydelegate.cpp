#include "applydelegate.h"

#include "friendsmodel.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTextDocument>

ApplyDelegate::ApplyDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

void ApplyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);          // 抗锯齿
    QString userName = index.data(FriendsModel::UserNameRole).toString();
    QString lastMessage = index.data(FriendsModel::LastMessageRole).toString();

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
    QRect usernameRect = QRect(itemRect.left()+60, itemRect.top()+10, nameWidth, 20);
    painter->setPen(Qt::black);
    QFont font("Arial", 12, QFont::Bold); // 字体为 Arial，大小为 20，加粗
    painter->setFont(font);
    painter->drawText(usernameRect, Qt::AlignLeft | Qt::AlignTop, userName);

    //绘制最新消息
    lastMessage = (lastMessage.size()>10? lastMessage.last(10)+"...":lastMessage);
    doc.setHtml(lastMessage);//只显示消息前面部分内容
    doc.setTextWidth(itemRect.width()); // 设置最大宽度
    int messageWidth = doc.idealWidth(); // 获取文本的实际宽度

    QRect messageRect = QRect(itemRect.left()+60, itemRect.top()+35, messageWidth, 20);
    painter->setPen(Qt::gray);
    font = QFont("Arial", 10, QFont::Medium);
    painter->setFont(font);
    painter->drawText(messageRect, Qt::AlignLeft | Qt::AlignTop, lastMessage);

    //同意按钮
    QRect agreeRect =  QRect(itemRect.right() -55, itemRect.top()+20, 40, 20);
    painter->setBrush(QColor(Qt::green));
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(agreeRect,10,10);
    painter->setPen(Qt::black);
    painter->drawText(agreeRect.adjusted(6,3,0,0), Qt::AlignLeft | Qt::AlignTop, "同意");

    painter->restore();
}

QSize ApplyDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    (void)index;
    return QSize(option.rect.width(),60);
}

bool ApplyDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {
        // MessageType type = static_cast<MessageType>(index.data(MessageModel::TypeRole).toInt());
        // if (type != MessageType::Image) return false;

        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        // 获取鼠标点击位置
        QPoint clickPos = mouseEvent->pos();
        QRect itemRect = option.rect;
        QRect agreeRect =  QRect(itemRect.right() -55, itemRect.top()+20, 40, 20);
        // 判断点击位置是否在按钮区域内
        QString id = index.data(FriendsModel::IdRole).toString();
        if (agreeRect.contains(clickPos)) {
            // 处理按钮点击逻辑
            // qDebug()<<"yes";
            emit applyResult(id,index.row());
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
