#include "messagelistview.h"

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

void MessageListView:: showAndHideScrollBars() {
    verticalScrollBar()->setVisible(true);
    horizontalScrollBar()->setVisible(true);
    hideTimer->start(); // 重置计时器，1秒后隐藏
}

void MessageListView::hideScrollBars() {
    verticalScrollBar()->setVisible(false);
    horizontalScrollBar()->setVisible(false);
}
