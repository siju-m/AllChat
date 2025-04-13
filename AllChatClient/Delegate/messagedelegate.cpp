#include "messagedelegate.h"

#include <QListView>



MessageDelegate::MessageDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

// MessageDelegate.cpp
void MessageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);          // 抗锯齿
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true); // 平滑缩放

    // 公共参数
    MessageType type = static_cast<MessageType>(index.data(MessageModel::TypeRole).toInt());
    if(type == MessageType::Time){
        drawTime(painter,option,index);
        painter->restore();
        return;
    }
    const bool isOutgoing = index.data(MessageModel::IsOutgoingRole).toBool();
    const QString userName = index.data(MessageModel::UserNameRole).toString();
    const QString avatarPath = index.data(MessageModel::AvatarRole).toString();

    // --- 头像绘制 ---
    QRect itemRect = option.rect;

    // 绘制头像（固定在左侧/右侧）
    QRect avatarRect =getAvatarRect(option,isOutgoing);
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



    // 绘制用户名
    QTextDocument doc;
    doc.setHtml(userName);
    doc.setTextWidth(250); // 设置最大宽度
    int nameWidth = doc.idealWidth(); // 获取文本的实际宽度
    QRect usernameRect = isOutgoing
                             ? QRect(itemRect.right()-nameWidth-55, itemRect.top(), 250, 20)
                             : QRect(itemRect.left()+65, itemRect.top(), 20, 20);
    painter->setPen(Qt::black);
    painter->drawText(usernameRect, Qt::AlignLeft | Qt::AlignTop, userName);

    // --- 根据消息类型绘制内容 ---
    if (type == MessageType::Text) {
        drawTextMessage(painter, option, index, isOutgoing);
    } else if (type == MessageType::Image) {
        drawImageMessage(painter, option, index, isOutgoing, avatarRect);
    }

    painter->restore();
}

void MessageDelegate::drawTextMessage(QPainter *painter, const QStyleOptionViewItem &option,
                                      const QModelIndex &index, bool isOutgoing) const {
    QString text = index.data(MessageModel::TextRole).toString();

    // 将纯文本中的换行符转换为HTML格式（可选步骤）

    // 计算气泡区域
    int maxBubbleWidth = option.rect.width() - 120;

    QTextDocument doc;
    // 如果text是纯文本应改用：
    doc.setPlainText(text);

    doc.setTextWidth(maxBubbleWidth);
    int textWidth = qMin((int)doc.idealWidth(), maxBubbleWidth);

    // 动态设置气泡尺寸
    int bubbleWidth = textWidth + 16; // 增加内边距
    int textHeight = doc.size().height();
    QRect bubbleRect = isOutgoing
                           ? QRect(option.rect.right() - bubbleWidth - 60, option.rect.top() + 15, bubbleWidth, textHeight + 10)
                           : QRect(option.rect.left() + 60, option.rect.top() + 15, bubbleWidth, textHeight + 10);

    // 绘制气泡背景
    painter->setBrush(isOutgoing ? QColor(149, 236, 105) : Qt::white);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(bubbleRect, 8, 8);

    // 绘制文本
    painter->save();
    painter->translate(bubbleRect.topLeft() + QPoint(8, 5));
    QRect textRect(0, 0, bubbleWidth - 16, option.rect.height());
    doc.drawContents(painter, textRect);
    painter->restore();
}

void MessageDelegate::drawTime(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString time = index.data(MessageModel::TimeRole).toString();
    time = caculate_time(time);
    QRect itemRect = option.rect;
    QTextDocument doc;
    doc.setHtml(time);
    doc.setTextWidth(250); // 设置最大宽度
    int timeWidth = doc.idealWidth(); // 获取文本的实际宽度
    QRect timeRect = QRect(itemRect.right()/2-(timeWidth/2), itemRect.top()+10, timeWidth, 20);
    //绘制背景气泡
    QColor bubbleColor = Qt::white;
    painter->setBrush(bubbleColor);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(timeRect.adjusted(-10,-5,0,0), 8, 8);
    //绘制文字
    painter->setPen(Qt::black);
    painter->drawText(timeRect, Qt::AlignLeft | Qt::AlignTop, time);
}

QString MessageDelegate::caculate_time(const QString &lastMsgTime) const
{
    QDateTime time = QDateTime::fromString(lastMsgTime,"yyyy-MM-dd hh:mm:ss");
    QDateTime time_startDay = QDate::currentDate().startOfDay();
    if(time>time_startDay){
        return time.time().toString("hh:mm");
    }else if(time_startDay.date().year()<=time.date().year()){
        return time.toString("MM-dd hh:mm");
    }else return time.toString("yyyy-MM-dd hh:mm");
}

// 绘制图片消息（关键改进：独立处理图片）
void MessageDelegate::drawImageMessage(QPainter *painter, const QStyleOptionViewItem &option,
                                       const QModelIndex &index, bool isOutgoing, const QRect &avatarRect) const {
    QPixmap image(index.data(MessageModel::ImageRole).toString());
    if(image.isNull())
        qDebug()<<"读取失败 "<<index.data(MessageModel::ImageRole).toString();
    // 计算图片显示区域
    qreal dpr = painter->device()->devicePixelRatio();//高dpi适配
    QSize scaledSize =getImageSize(index);
    QRect imageRect = calculateImageRect(option, isOutgoing, avatarRect, scaledSize);
    QPixmap scaled = image.scaled(
        scaledSize* dpr,
        Qt::KeepAspectRatioByExpanding, // 保持比例并扩展至目标尺寸
        Qt::SmoothTransformation       // 平滑处理
        );
    scaled.setDevicePixelRatio(dpr);
    // 绘制图片（带圆角裁剪）
    QPainterPath clipPath;
    painter->setPen(Qt::NoPen);
    clipPath.addRoundedRect(imageRect.adjusted(1,1,-1,-1), 8*dpr, 8*dpr);//不adjusted的话会有不连续的黑框影响美观
    painter->setClipPath(clipPath);
    painter->drawPixmap(imageRect, scaled);
    painter->setClipping(false);
}

// 实现calculateImageRect
QRect MessageDelegate::calculateImageRect(const QStyleOptionViewItem &option, bool isOutgoing,
                                          const QRect &avatarRect, const QSize &imageSize) const {
    const int spacing = 15;
    QPoint startPoint;

    if (isOutgoing) {
        startPoint = QPoint(option.rect.right() - avatarRect.width() - imageSize.width() - spacing,
                            avatarRect.top()+spacing);
    } else {
        startPoint = QPoint(avatarRect.right() + spacing,
                            avatarRect.top()+spacing);
    }

    return QRect(startPoint, imageSize);
}

bool MessageDelegate::isClickOnImage(const QPoint &pos, const QModelIndex &index, const QStyleOptionViewItem &option) const
{
    MessageType type = static_cast<MessageType>(index.data(MessageModel::TypeRole).toInt());
    if (type != MessageType::Image) return false;
    const bool isOutgoing = index.data(MessageModel::IsOutgoingRole).toBool();

    // 获取图片绘制区域
    QRect imageRect = calculateImageRect(option,
                                         isOutgoing,
                                         getAvatarRect(option,isOutgoing),
                                         getImageSize(index)
                                         );
    // 判断点击位置是否在区域内

    return imageRect.contains(pos);
}

qint32 MessageDelegate::getListViewSpacing()
{
    return m_listView_Spacing;
}

// 获取头像绘制区域（已实现）
QRect MessageDelegate::getAvatarRect(const QStyleOptionViewItem &option, bool isOutgoing) const {
    const qint16 avatarSize = 40;
    if (isOutgoing) {
        // 右侧头像
        return QRect(
            option.rect.right() - 50,
            option.rect.top() ,
            avatarSize,
            avatarSize
            );
    } else {
        // 左侧头像
        return QRect(
            option.rect.left() + 10,
            option.rect.top() ,
            avatarSize,
            avatarSize
            );
    }
}

// 获取图片显示尺寸（已实现）
QSize MessageDelegate::getImageSize(const QModelIndex &index) const {
    const int maxImageSize = 300;
    QPixmap image(index.data(MessageModel::ImageRole).toString());

    // 保持宽高比缩放
    return image.size().scaled(
        maxImageSize,
        maxImageSize,
        Qt::KeepAspectRatio
        );
}

bool MessageDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {
        // MessageType type = static_cast<MessageType>(index.data(MessageModel::TypeRole).toInt());
        // if (type != MessageType::Image) return false;

        QStyleOptionViewItem newOption = option;
        newOption.rect.adjust(0,0,-(m_listView_Spacing*2),0);
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        // 获取鼠标点击位置
        QPoint clickPos = mouseEvent->pos();

        // 判断点击位置是否在按钮区域内
        /*在发送方点击图片，检测区域会向右偏移一小段，点击接收方的图片就没有这个问题*/
        /*是listview设定的间距，paint中的option没有把间距算作宽度，但是editorEvent把这个算进去了*/
        if (isClickOnImage(clickPos,index,newOption)) {
            // 处理按钮点击逻辑
            // qDebug() << "Button clicked in item at row:" << index.row();
            QPixmap image(index.data(MessageModel::ImageRole).toString());
            emit imageClicked(image);
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QSize MessageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    MessageType type = static_cast<MessageType>(index.data(MessageModel::TypeRole).toInt());
    const int verticalSpacing = 20;
    const int bottumMargin = 15;
    switch(type){
    case MessageType::Text:{
        // 文本高度计算
        int maxWidth =option.rect.width() - 120;// 头像+边距
        QString text = index.data(MessageModel::TextRole).toString();
        QTextDocument doc;
        doc.setPlainText(text);
        doc.setTextWidth(maxWidth); // 与气泡宽度一致
        return QSize(option.rect.width(), doc.size().height() + verticalSpacing+bottumMargin);
    }
    case MessageType::Image:{
        // 图片高度计算
        QPixmap image(index.data(MessageModel::ImageRole).toString());
        QSize scaledSize = image.size().scaled(300, 300, Qt::KeepAspectRatio);
        return QSize(option.rect.width(), scaledSize.height() + verticalSpacing+bottumMargin);
    }
    case MessageType::Time:{
        return QSize(option.rect.width(), 20+bottumMargin);
    }
    default:return sizeHint(option,index);
    }

}
