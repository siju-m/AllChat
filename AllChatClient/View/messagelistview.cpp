#include "messagelistview.h"

#include "imageviewer.h"

#include <QDesktopServices>


MessageListView::MessageListView(QWidget *parent) :
    QListView(parent)
{
    initMessageList();

    initScorllBar();
}

void MessageListView::initMessageList()
{
    m_message_model = new MessageModel(this);
    m_message_delegate = new MessageDelegate(this);

    /*使用listView在消息比较多的情况下内存低、可以流畅滚动
    用widget的话消息一多就比较占用内存*/
    setModel(m_message_model);
    setItemDelegate(m_message_delegate);
    setVerticalScrollMode(QListView::ScrollPerPixel); // 平滑滚动
    setResizeMode(QListView::Adjust);                 // 自动调整项大小

    // 设置ListView属性
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//始终隐藏水平滚动条
    setSelectionMode(QAbstractItemView::NoSelection);

    QScrollBar *verticalScrollBar = this->verticalScrollBar();
    verticalScrollBar->setSingleStep(10); // 垂直滚动步长
    verticalScrollBar->setPageStep(100);  // 垂直滚动一页的距离

    connect(m_message_delegate, &MessageDelegate::imageClicked, this, [=](const QPixmap &image){
        ImageViewer *viewer = new ImageViewer(image);
        viewer->resize(800, 600);
        viewer->exec(); // 或 show() 非模态显示
    });

    connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, &MessageListView::checkScrollTop);

    // 打开聊天记录的文件
    connect(m_message_delegate, &MessageDelegate::fileClicked, this, [=](const QString &filePath){
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    });
}

void MessageListView::initScorllBar()
{
    // 初始隐藏滚动条
    verticalScrollBar()->setVisible(false);
    horizontalScrollBar()->setVisible(false);

    // 创建定时器
    hideTimer = new QTimer(this);
    hideTimer->setInterval(3000); // 1秒后隐藏
    connect(hideTimer, &QTimer::timeout, this, &MessageListView::hideScrollBars);

    // 监听滚动条滚动，触发显示逻辑
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &MessageListView::showAndHideScrollBars);
    connect(horizontalScrollBar(), &QScrollBar::valueChanged, this, &MessageListView::showAndHideScrollBars);
}

void MessageListView::clear()
{
    m_message_model->clear();
    m_isFirstLoad = true;
}

void MessageListView::addTime_toList(const QString &time)
{
    QString lastMsgTime = m_message_model->get_lastTime();
    if(lastMsgTime.isEmpty()){
        m_message_model->addTimeMessage(time);
    }else{
        bool isTooLong = compareTime(lastMsgTime,time);
        if(isTooLong)
            m_message_model->addTimeMessage(time);
    }
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
    const QString time = message.getTime();

    addTime_toList(time);
    m_message_model->addMessage(message);

    this->scrollToBottom();// 自动滚动到底部
}

void MessageListView::addOlderMessage(const QVector<Message> &messages)
{
    // 插入前记录当前滚动位置
    QScrollBar* vScroll = this->verticalScrollBar();
    const int oldScrollPos = vScroll->value();

    QString olderMsgTime;
    int rowSize = 0;
    for(auto &message : messages)
    {
        const QString time = message.getTime();
        olderMsgTime = m_message_model->get_olderTime();

        if(!olderMsgTime.isEmpty())
        {
            bool isTooLong = compareTime(olderMsgTime,time);
            if(isTooLong)
            {
                ++rowSize;
                m_message_model->addOlderTimeMessage(olderMsgTime);
            }
        }
        ++rowSize;
        m_message_model->addOlderMessage(message);
    }

    if(messages.size() < 15)
    {
        // 历史记录的数量不够时说明已经到顶了
        olderMsgTime = m_message_model->get_olderTime();
        if(!olderMsgTime.isEmpty())
            m_message_model->addOlderTimeMessage(olderMsgTime);
    }

    if(m_isFirstLoad)
    {        
        this->scrollToBottom();// 自动滚动到底部
        m_isFirstLoad = false;
    }
    else
    {
        QStyleOptionViewItem option;
        option.initFrom(this); // 初始化样式
        //todo 计算新加入项的高度需要精确计算
        QTimer::singleShot(0, this, [=] {
            int newHeight = 0;
            for (int row = 0; row < rowSize; ++row) {
                QModelIndex index = m_message_model->index(row, 0);
                QSize size = m_message_delegate->sizeHint(option, index); // 传入 QStyleOptionViewItem

                newHeight += size.height();
            }
            vScroll->setValue(oldScrollPos + newHeight);
        });
    }
}

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

void MessageListView::checkScrollTop(int value)
{
    if (!m_isFirstLoad && value == 0)
        emit scrolledToTop();
}
