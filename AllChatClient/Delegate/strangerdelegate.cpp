#include "strangerdelegate.h"

#include <Model/strangermodel.h>
#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTextDocument>

StrangerDelegate::StrangerDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

void StrangerDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);          // 抗锯齿


    //绘制头像
    draw_avatar(painter,option,index);

    // 绘制用户名
    draw_userName(painter,option,index);

    bool isFriend = index.data(StrangerModel::IsFriendRole).toBool();
    if(isFriend){
        // 跳转发送消息
        draw_sendBtn(painter, option, index);
    }else{
        //添加按钮
        draw_addBtn(painter, option, index);
    }


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

void StrangerDelegate::draw_userName(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString userName = index.data(StrangerModel::UserNameRole).toString();

    QRect itemRect = option.rect;
    QTextDocument doc;
    doc.setHtml(userName);
    doc.setTextWidth(itemRect.width()); // 设置最大宽度
    int nameWidth = doc.idealWidth(); // 获取文本的实际宽度
    QRect usernameRect = QRect(itemRect.left()+60, itemRect.top()+20, nameWidth, 20);
    painter->setPen(Qt::black);
    QFont font("微软雅黑", 14, QFont::Bold);
    painter->setFont(font);
    painter->drawText(usernameRect, Qt::AlignLeft | Qt::AlignTop, userName);
}

void StrangerDelegate::draw_addBtn(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect itemRect = option.rect;
    QRect applyRect =  QRect(itemRect.right() -60, itemRect.top()+20, 50, 30);
    switch (m_buttonState[index]) {
    case Normal:
        painter->setBrush(QColor(53, 93, 144, 255));
        break;
    case Hover:
        painter->setBrush(QColor(93, 125, 166, 255));
        break;
    default:
        break;
    }

    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(applyRect,10,10);
    painter->setPen(Qt::white);
    painter->setFont(QFont("微软雅黑", 10, QFont::Bold));
    painter->drawText(applyRect.adjusted(12,6,0,0), Qt::AlignLeft | Qt::AlignTop, "添加");
}

void StrangerDelegate::draw_sendBtn(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect itemRect = option.rect;
    QRect applyRect =  QRect(itemRect.right() -65, itemRect.top()+20, 60, 30);
    switch (m_buttonState[index]) {
    case Normal:
        painter->setBrush(QColor(53, 93, 144, 255));
        break;
    case Hover:
        painter->setBrush(QColor(93, 125, 166, 255));
        break;
    default:
        break;
    }

    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(applyRect,10,10);
    painter->setPen(Qt::white);
    painter->setFont(QFont("微软雅黑", 10, QFont::Bold));
    painter->drawText(applyRect.adjusted(12,6,0,0), Qt::AlignLeft | Qt::AlignTop, "发消息");
}

bool StrangerDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseMove) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        // 获取鼠标点击位置
        QPoint clickPos = mouseEvent->pos();
        QRect itemRect = option.rect;
        QRect applyRect =  QRect(itemRect.right() -60, itemRect.top()+20, 50, 30);
        QString id = index.data(StrangerModel::IdRole).toString();
        if (applyRect.contains(clickPos)) {
            if(event->type() == QEvent::MouseButtonPress){
                // 处理按钮点击逻辑
                bool isFriend = index.data(StrangerModel::IsFriendRole).toBool();
                if(isFriend){
                    emit sendClicked(id);
                }else{
                    emit applyClicked(id,index.row());
                }
                return true;
            }else if(event->type() == QEvent::MouseMove){
                m_buttonState[index] = Hover;
            }
        }else{
            m_buttonState[index] = Normal;
        }
        // 触发视图更新按钮
        QWidget *widget = qobject_cast<QWidget *>(parent());
        if (widget)
            widget->update();
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
