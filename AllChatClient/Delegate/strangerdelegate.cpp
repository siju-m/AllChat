#include "strangerdelegate.h"

#include <Model/strangermodel.h>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTextDocument>

StrangerDelegate::StrangerDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

void StrangerDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);          // 抗锯齿
    QString userName = index.data(StrangerModel::UserNameRole).toString();

    QRect itemRect = option.rect;

    //绘制头像
    draw_avatar(painter,option,index);
    // QRect avatarRect =  QRect(itemRect.left() + 10, itemRect.top()+10, 40, 40);
    // painter->setBrush(QColor(200, 200, 200));
    // painter->setPen(Qt::NoPen);
    // painter->drawEllipse(avatarRect);


    // 绘制用户名
    QTextDocument doc;
    doc.setHtml(userName);
    doc.setTextWidth(itemRect.width()); // 设置最大宽度
    int nameWidth = doc.idealWidth(); // 获取文本的实际宽度
    QRect usernameRect = QRect(itemRect.left()+60, itemRect.top()+20, nameWidth, 20);
    painter->setPen(Qt::black);
    QFont font("微软雅黑", 14, QFont::Bold);
    painter->setFont(font);
    painter->drawText(usernameRect, Qt::AlignLeft | Qt::AlignTop, userName);

    //同意按钮
    QRect applyRect =  QRect(itemRect.right() -60, itemRect.top()+20, 50, 30);
    painter->setBrush(QColor(Qt::green));
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(applyRect,10,10);
    painter->setPen(Qt::black);
    painter->setFont(QFont("微软雅黑", 12, QFont::Bold));
    painter->drawText(applyRect.adjusted(9,3,0,0), Qt::AlignLeft | Qt::AlignTop, "添加");

    painter->restore();
}

QSize StrangerDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    (void)index;
    return QSize(option.rect.width(),60);
}

void StrangerDelegate::draw_avatar(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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

bool StrangerDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        // 获取鼠标点击位置
        QPoint clickPos = mouseEvent->pos();
        QRect itemRect = option.rect;
        QRect applyRect =  QRect(itemRect.right() -60, itemRect.top()+20, 50, 30);
        QString id = index.data(StrangerModel::IdRole).toString();
        if (applyRect.contains(clickPos)) {
            // 处理按钮点击逻辑
            emit applyClicked(id,index.row());
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
