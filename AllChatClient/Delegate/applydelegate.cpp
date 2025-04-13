#include "applydelegate.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTextDocument>

#include <Model/strangermodel.h>

ApplyDelegate::ApplyDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

void ApplyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);          // 抗锯齿

    //绘制头像
    draw_avatar(painter,option,index);
    // 绘制用户名
    draw_userName(painter,option,index);
    //绘制申请消息
    draw_applyMsg(painter,option,index);
    //同意按钮
    draw_btn(painter,option);

    painter->restore();
}

QSize ApplyDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    (void)index;
    return QSize(option.rect.width(),60);
}

void ApplyDelegate::draw_avatar(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QString avatarPath = index.data(StrangerModel::AvatarRole).toString();
    QRect itemRect = option.rect;
    //绘制头像
    QRect avatarRect =  QRect(itemRect.left() + 10, itemRect.top()+10, 40, 40);
    if(avatarPath.isEmpty()){
        painter->setBrush(QColor(200, 200, 200));
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(avatarRect);
    }else{
        QPixmap pixmap(avatarPath);
        if (pixmap.isNull()) {
            qDebug() << "加载头像失败:" << avatarPath;
        } else {
            // 获取设备像素比（处理高DPI屏幕）
            qreal dpr = painter->device()->devicePixelRatio();
            QSize targetSize = avatarRect.size() * dpr;

            // 使用高质量缩放并扩展至目标区域
            QPixmap scaled = pixmap.scaled(
                targetSize,
                Qt::KeepAspectRatioByExpanding, // 保持比例并扩展至目标尺寸
                Qt::SmoothTransformation       // 平滑处理
                );
            scaled.setDevicePixelRatio(dpr);

            // 设置圆形剪裁路径（确保头像显示为椭圆）
            QPainterPath clipPath;
            clipPath.addEllipse(avatarRect);
            painter->setClipPath(clipPath);

            // 计算居中绘制位置（避免缩放后图像偏移）
            QPoint drawPos = avatarRect.topLeft();
            if (scaled.width() > targetSize.width()) {
                drawPos.setX(drawPos.x() - (scaled.width() / dpr - avatarRect.width()) / 2);
            }
            if (scaled.height() > targetSize.height()) {
                drawPos.setY(drawPos.y() - (scaled.height() / dpr - avatarRect.height()) / 2);
            }

            painter->drawPixmap(drawPos, scaled);
            painter->setClipping(false); // 关闭剪裁以免影响后续绘制
        }
    }
}

void ApplyDelegate::draw_userName(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString userName = index.data(StrangerModel::UserNameRole).toString();
    QRect itemRect = option.rect;

    QTextDocument doc;
    doc.setHtml(userName);
    doc.setTextWidth(itemRect.width()); // 设置最大宽度
    int nameWidth = doc.idealWidth(); // 获取文本的实际宽度
    QRect usernameRect = QRect(itemRect.left()+60, itemRect.top()+10, nameWidth, 20);
    painter->setPen(Qt::black);
    QFont font("Arial", 12, QFont::Bold); // 字体为 Arial，大小为 20，加粗
    painter->setFont(font);
    painter->drawText(usernameRect, Qt::AlignLeft | Qt::AlignTop, userName);
}

void ApplyDelegate::draw_applyMsg(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString applyMessage = index.data(StrangerModel::ApplyMessageRole).toString();
    QRect itemRect = option.rect;

    QTextDocument doc;
    applyMessage = (applyMessage.size()>10? applyMessage.last(10)+"...":applyMessage);
    doc.setHtml(applyMessage);//只显示消息前面部分内容
    doc.setTextWidth(itemRect.width()); // 设置最大宽度
    int messageWidth = doc.idealWidth(); // 获取文本的实际宽度

    QRect messageRect = QRect(itemRect.left()+60, itemRect.top()+35, messageWidth, 20);
    painter->setPen(Qt::gray);
    QFont font = QFont("Arial", 10, QFont::Medium);
    painter->setFont(font);
    painter->drawText(messageRect, Qt::AlignLeft | Qt::AlignTop, applyMessage);
}

void ApplyDelegate::draw_btn(QPainter *painter, const QStyleOptionViewItem &option) const
{
    QRect itemRect = option.rect;
    QRect agreeRect =  QRect(itemRect.right() -55, itemRect.top()+20, 40, 20);
    painter->setBrush(QColor(Qt::green));
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(agreeRect,10,10);
    painter->setPen(Qt::black);
    painter->drawText(agreeRect.adjusted(6,3,0,0), Qt::AlignLeft | Qt::AlignTop, "同意");
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
        QString id = index.data(StrangerModel::IdRole).toString();
        if (agreeRect.contains(clickPos)) {
            // 处理按钮点击逻辑
            // qDebug()<<"yes";
            emit applyResult(id,index.row());
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
