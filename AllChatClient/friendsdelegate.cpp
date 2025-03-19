#include "friendsdelegate.h"
#include "friendsmodel.h"
#include <QPainter>
#include <QPainterPath>
#include <QTextDocument>

FriendsDelegate::FriendsDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

void FriendsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);          // 抗锯齿
    QString userName = index.data(FriendsModel::UserNameRole).toString();
    QString lastMessage = index.data(FriendsModel::LastMessageRole).toString();
    const QString avatarPath = index.data(FriendsModel::AvatarRole).toString();

    // 绘制选中状态背景
    if (option.state & QStyle::State_Selected) {
        QPainterPath path;
        path.addRoundedRect(option.rect.adjusted(2, 2, -2, -2), 10, 10); // 圆角矩形

        // 渐变填充
        QLinearGradient gradient(option.rect.topLeft(), option.rect.bottomRight());
        gradient.setColorAt(0.0, QColor(50, 150, 255, 180));
        gradient.setColorAt(1.0, QColor(20, 100, 220, 100));
        painter->fillPath(path, gradient);

        // 添加边框
        QPen pen(QColor(30, 100, 220));
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawPath(path);
    }

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
            qDebug() << "Failed to load avatar:" << avatarPath;
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
    if (option.state & QStyle::State_Selected)
        painter->setPen(Qt::white);
    else
        painter->setPen(Qt::gray);
    font = QFont("Arial", 10, QFont::Bold);
    painter->setFont(font);
    painter->drawText(messageRect, Qt::AlignLeft | Qt::AlignTop, lastMessage);

    // QPen linePen(Qt::gray, 1); // 设置分割线的颜色和宽度
    // painter->setPen(linePen);
    // painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());



    painter->restore();
}

QSize FriendsDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    (void)index;
    return QSize(option.rect.width(),60);
}
