#include "chatdelegate.h"

#include <QDateTime>
#include <QPainter>
#include <QPainterPath>
#include <QTextDocument>

#include <Model/chatmodel.h>

ChatDelegate::ChatDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

void ChatDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);          // 抗锯齿

    draw_background(painter,option);
    draw_avatar(painter,option,index);
    draw_userName(painter,option,index);
    draw_lastMsg(painter,option,index);
    draw_lastMsgTime(painter,option,index);
    draw_unreadMsgNum(painter,option,index);

    painter->restore();
}

QSize ChatDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    (void)index;
    return QSize(option.rect.width(),60);
}

void ChatDelegate::draw_background(QPainter *painter, const QStyleOptionViewItem &option) const
{
    // 绘制选中状态背景
    if (option.state & QStyle::State_Selected)
    {
        QPainterPath path;
        path.addRoundedRect(option.rect.adjusted(0, 1, 0, -1), 0, 0); // 圆角矩形

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
    else if (option.state & QStyle::State_MouseOver)
    {
        painter->fillRect(option.rect, QColor(222,229,237));
    }
}

void ChatDelegate::draw_avatar(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QPixmap avatar = index.data(ChatModel::AvatarRole).value<QPixmap>();
    QRect itemRect = option.rect;
    //绘制头像
    QRect avatarRect =  QRect(itemRect.left() + 10, itemRect.top()+10, 40, 40);
    if(avatar.isNull()){
        painter->setBrush(QColor(200, 200, 200));
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(avatarRect);
    }else{
        // 获取设备像素比（处理高DPI屏幕）
        qreal dpr = painter->device()->devicePixelRatio();
        QSize targetSize = avatarRect.size() * dpr;

        // 使用高质量缩放并扩展至目标区域
        QPixmap scaled = avatar.scaled(
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

void ChatDelegate::draw_userName(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect itemRect = option.rect;
    QString userName = index.data(ChatModel::UserNameRole).toString();

    // 绘制用户名
    QFont font("Arial", 12, QFont::Bold); // 字体为 Arial，大小为 20，加粗
    QFontMetrics metrics(font);
    int maxWidth = 100; // 最多显示宽度（像素）
    userName = metrics.elidedText(userName, Qt::ElideRight, maxWidth);//截取过长文字
    int nameWidth = metrics.horizontalAdvance(userName); // 获取文本宽度
    int nameHeight = metrics.height(); // 获取文本高度
    QRect usernameRect = QRect(itemRect.left()+60, itemRect.top()+10, nameWidth, nameHeight);

    painter->setPen(Qt::black);
    painter->setFont(font);
    painter->drawText(usernameRect, Qt::AlignLeft | Qt::AlignTop, userName);
}

void ChatDelegate::draw_lastMsg(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString lastMessage = index.data(ChatModel::LastMessageRole).toString();
    QRect itemRect = option.rect;

    //绘制最新消息
    lastMessage = (lastMessage.size()>6? lastMessage.left(6)+"...":lastMessage);//截取过长消息
    QFont font("Arial", 10, QFont::Bold);
    QFontMetrics metrics(font);
    int msgWidth = metrics.horizontalAdvance(lastMessage); // 获取文本宽度
    int msgHeight = metrics.height(); // 获取文本高度

    QRect messageRect = QRect(itemRect.left()+60, itemRect.top()+35, msgWidth, msgHeight);
    if (option.state & QStyle::State_Selected)
        painter->setPen(Qt::white);
    else
        painter->setPen(Qt::gray);

    painter->setFont(font);
    painter->drawText(messageRect, Qt::AlignLeft | Qt::AlignTop, lastMessage);
}

void ChatDelegate::draw_lastMsgTime(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect itemRect = option.rect;
    QString lastMsgTime = index.data(ChatModel::LastMessageTimeRole).toString();
    lastMsgTime = caculate_time(lastMsgTime);

    QFont font("Arial", 8, QFont::Bold);
    QFontMetrics metrics(font);
    int timeWidth = metrics.horizontalAdvance(lastMsgTime); // 获取文本宽度
    int timeHeight = metrics.height(); // 获取文本高度
    QRect timeRect = QRect(itemRect.right()-timeWidth-15,itemRect.top()+12,timeWidth,timeHeight);

    if (option.state & QStyle::State_Selected)
        painter->setPen(Qt::white);
    else
        painter->setPen(Qt::gray);
    painter->setFont(font);
    painter->drawText(timeRect, Qt::AlignLeft | Qt::AlignTop, lastMsgTime);
}

QString ChatDelegate::caculate_time(const QString &lastMsgTime) const
{
    QDateTime time = QDateTime::fromString(lastMsgTime,"yyyy-MM-dd hh:mm:ss");
    QDateTime time_startDay = QDate::currentDate().startOfDay();
    if(time>time_startDay){
        return time.time().toString("hh:mm");
    }else if(time_startDay.date().year()<=time.date().year()){
        return time.toString("MM-dd");
    }else return time.toString("yyyy-MM-dd");
}

void ChatDelegate::draw_unreadMsgNum(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect itemRect = option.rect;
    const int unreadMsgNum = index.data(ChatModel::UnreadMsgNumRole).toInt();
    if(!unreadMsgNum) return;

    //绘制未读消息数
    QString unreadNum;
    if(unreadMsgNum>99) unreadNum = "99+";
    else unreadNum = QString::number(unreadMsgNum);
    QFont font("Arial", 8, QFont::Bold);
    QFontMetrics metrics(font);
    int numWidth = metrics.horizontalAdvance(unreadNum); // 获取文本宽度
    int numeHeight = metrics.height(); // 获取文本高度
    QRect numRect = QRect(itemRect.right()-numWidth-15,itemRect.top()+35,numWidth<numeHeight?numeHeight:numWidth,numeHeight);

    //气泡
    QColor bubbleColor = Qt::red;
    painter->setBrush(bubbleColor);
    painter->setPen(Qt::NoPen);
    int spacing = 4;
    QPoint numPoint = QPoint(numRect.left()+numWidth/2,numRect.top()+numeHeight/2);
    int w = qMax(numWidth, numeHeight) + spacing;  // 宽度
    int h = numeHeight+spacing;   // 高度
    QRect bubbleRect = QRect(numPoint.rx()-w/2,numPoint.ry()-h/2,w,h);
    painter->drawRoundedRect(bubbleRect, h/2,h/2);
    //数字
    painter->setFont(font);
    painter->setPen(Qt::white);
    painter->drawText(numRect, Qt::AlignLeft | Qt::AlignTop, unreadNum);
}
