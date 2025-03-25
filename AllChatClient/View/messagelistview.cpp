#include "messagelistview.h"

#include <Delegate/messagedelegate.h>

MessageListView::MessageListView(QWidget *parent) : QListView(parent) {
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
