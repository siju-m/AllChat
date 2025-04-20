#include "messagelistview.h"

#include "Core/currentuser.h"
#include "imageviewer.h"

#include <Delegate/messagedelegate.h>

MessageListView::MessageListView(QWidget *parent) : QListView(parent) {
    initMessageList();

    initScorllBar();
}

void MessageListView::initMessageList()
{
    m_message_model = new MessageModel(this);
    MessageDelegate *message_delegate = new MessageDelegate(this);

    /*使用listView在消息比较多的情况下内存低、可以流畅滚动
    用widget的话消息一多就比较占用内存*/
    setModel(m_message_model);
    setItemDelegate(message_delegate);
    setVerticalScrollMode(QListView::ScrollPerPixel); // 平滑滚动
    setResizeMode(QListView::Adjust);                 // 自动调整项大小

    // 设置ListView属性
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//始终隐藏水平滚动条
    setSelectionMode(QAbstractItemView::NoSelection);

    QScrollBar *verticalScrollBar = this->verticalScrollBar();
    verticalScrollBar->setSingleStep(10); // 垂直滚动步长
    verticalScrollBar->setPageStep(100);  // 垂直滚动一页的距离

    connect(message_delegate,&MessageDelegate::imageClicked,this,[=](const QPixmap &image){
        ImageViewer *viewer = new ImageViewer(image);
        viewer->resize(800, 600);
        viewer->exec(); // 或 show() 非模态显示
    });
}

void MessageListView::initScorllBar()
{
    // 初始隐藏滚动条
    verticalScrollBar()->setVisible(false);
    horizontalScrollBar()->setVisible(false);

    // 创建定时器
    hideTimer = new QTimer(this);
    hideTimer->setInterval(1000); // 1秒后隐藏
    connect(hideTimer, &QTimer::timeout, this, &MessageListView::hideScrollBars);

    // 监听滚动条滚动，触发显示逻辑
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &MessageListView::showAndHideScrollBars);
    connect(horizontalScrollBar(), &QScrollBar::valueChanged, this, &MessageListView::showAndHideScrollBars);
}

void MessageListView::clear()
{
    m_message_model->clear();
}

void MessageListView::addTime_toList(const QString &chatId,const QString &time)
{
    QString lastMsgTime = m_message_model->get_lastTempTime(chatId);
    if(lastMsgTime.isEmpty()){
        m_message_model->addTimeMessage(time);
    }else{
        bool isTooLong = compareTime(lastMsgTime,time);
        if(isTooLong) m_message_model->addTimeMessage(time);
    }
    m_message_model->update_lastTempTime(chatId,time);
}

bool MessageListView::compareTime(const QString &pastTime, const QString &lastTime)
{
    QDateTime lastDateTime = QDateTime::fromString(lastTime,"yyyy-MM-dd hh:mm:ss");
    QDateTime pastDateTime = QDateTime::fromString(pastTime,"yyyy-MM-dd hh:mm:ss");
    if (lastDateTime.isValid() && pastDateTime.isValid()) {
        // 计算相差分钟数
        qint64 diffSeconds = lastDateTime.secsTo(pastDateTime); // time1 到 time2 的秒数（可能是负数）
        qint64 diffMinutes = diffSeconds / 60;    // 转换成分钟
        if(qAbs(diffMinutes)>2) return true;
        // qDebug() << "相差" << qAbs(diffMinutes) << "分钟"; // 取绝对值显示
    } else {
        qDebug() << "时间格式错误";
    }
    return false;
}

void MessageListView::addMessage(const Message &message)
{
    bool isOutgoing = (message.getSenderId() == CurrentUser::getInstance()->get_userId());
    const QString senderName = message.getSenderName();
    const QString avatarPath = message.getAvatarPath();
    const QString time = message.getTime();
    if(message.getType() == Message::Text)
    {
        const QString text = message.getText();
        m_message_model->addTextMessage(text,isOutgoing,senderName,avatarPath,time);
    }
    else if(message.getType() == Message::Image)
    {
        const QString imagePath = message.getImage();
        m_message_model->addImageMessage(imagePath,isOutgoing,senderName,avatarPath,time);
    }
    this->scrollToBottom();// 自动滚动到底部
}

// void MessageListView::addTextMessage(const QString &text, bool isOutgoing, const QString &userName, const QString &avatarPath, const QString &time)
// {
//     m_message_model->addTextMessage(text,isOutgoing,userName,avatarPath,time);
//     this->scrollToBottom();// 自动滚动到底部
// }

// void MessageListView::addImageMessage(const QString &imagePath, bool isOutgoing, const QString &userName, const QString &avatarPath, const QString &time)
// {
//     m_message_model->addImageMessage(imagePath,isOutgoing,userName,avatarPath,time);
//     this->scrollToBottom();
// }

void MessageListView:: wheelEvent(QWheelEvent *event)  {
    showAndHideScrollBars(); // 显示滚动条
    QListView::wheelEvent(event);
}

void MessageListView::resizeEvent(QResizeEvent *event)
{
    QListView::resizeEvent(event);

    //调整消息项大小
    // 获取模型
    QAbstractItemModel *model = this->model();
    if (!model) return;

    // 假设你知道 delegate 类型为 MessageDelegate，
    // 并且已经设置给这个视图
    MessageDelegate *msgDelegate = qobject_cast<MessageDelegate*>(this->itemDelegate());
    if (!msgDelegate) return;

    // 获取视口的可见区域（相对于视口坐标）
    QRect visibleRect = viewport()->rect();

    // 获取可见区域左上角的索引
    QModelIndex topIndex = indexAt(visibleRect.topLeft()+ QPoint(1, 1));
    if (!topIndex.isValid())
        return;

    // 获取可见区域左下角的索引
    QModelIndex bottomIndex = indexAt(visibleRect.bottomLeft()+ QPoint(1, -1));
    // 如果 bottomIndex 无效，则使用模型中最后一项的索引
    if (!bottomIndex.isValid())
        bottomIndex = model->index(model->rowCount() - 1, 0);

    int startRow = topIndex.row();
    int endRow = bottomIndex.row();
    for (int row = startRow; row <= endRow; ++row) {
        QModelIndex index = model->index(row, 0);

        emit msgDelegate->sizeHintChanged(index);
    }
}

void MessageListView:: showAndHideScrollBars() {
    verticalScrollBar()->setVisible(true);
    horizontalScrollBar()->setVisible(true);
    hideTimer->start(); // 重置计时器，1秒后隐藏
}

void MessageListView::hideScrollBars() {
    verticalScrollBar()->setVisible(false);
    horizontalScrollBar()->setVisible(false);
}
