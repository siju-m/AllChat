#include "customtitlebar.h"

#include <QHBoxLayout>
#include <QMouseEvent>

CustomTitleBar::CustomTitleBar(QWidget *parent) : QWidget(parent) {
    setFixedHeight(40);
    // setStyleSheet("background-color: #333; color: white;");

    m_titleLabel = new QLabel("AllChat", this);

    m_minimizeButton = new QPushButton("", this);
    m_minimizeButton->setIcon(QIcon(":/Icon/minus.png"));
    m_minimizeButton->setFlat(true);
    m_maximizeButton = new QPushButton("", this);
    m_maximizeButton->setIcon(QIcon(":/Icon/square.png"));
    m_maximizeButton->setFlat(true);
    m_closeButton = new QPushButton("", this);
    m_closeButton->setIcon(QIcon(":/Icon/cross.png"));
    m_closeButton->setFlat(true);

    m_minimizeButton->setFixedSize(30, 30);
    m_maximizeButton->setFixedSize(30, 30);
    m_closeButton->setFixedSize(30, 30);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_titleLabel);
    layout->addStretch();
    layout->addWidget(m_minimizeButton);
    layout->addWidget(m_maximizeButton);
    layout->addWidget(m_closeButton);
    layout->setContentsMargins(10, 0, 10, 0);
    setLayout(layout);

    connect(m_minimizeButton, &QPushButton::clicked, this, &CustomTitleBar::minimizeClicked);
    connect(m_maximizeButton, &QPushButton::clicked, this, &CustomTitleBar::maximizeClicked);
    connect(m_closeButton, &QPushButton::clicked, this, &CustomTitleBar::closeClicked);
}


// 处理鼠标按下事件（用于拖动窗口）
void CustomTitleBar::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_offset = event->globalPosition().toPoint() - parentWidget()->window()->frameGeometry().topLeft();
        event->accept();
    }
}

// 处理鼠标移动事件（拖动窗口）
void CustomTitleBar::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        parentWidget()->window()->move(event->globalPosition().toPoint() - m_offset);
        event->accept();
    }
}
