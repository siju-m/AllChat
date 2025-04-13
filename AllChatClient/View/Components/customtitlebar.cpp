#include "customtitlebar.h"
#include "qwindow.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <windows.h>

CustomTitleBar::CustomTitleBar(QWidget *parent) : QWidget(parent) {
    setFixedHeight(40);
    // setStyleSheet("background-color: #333; color: white;");

    m_titleLabel = new QLabel("AllChat", this);
    m_titleLabel->setStyleSheet("font: 'Roboto';font-weight: bold;");

    m_borderWidget = new QWidget(this);
    m_borderWidget->setFixedSize(201,30);
    m_borderWidget->setStyleSheet("border-right: 1px solid gray;border-left: 1px solid gray;");

    QString btnStyle = "QPushButton{"
                       "border:none;"
                       "padding:5px;"
                       "}"
                       "QPushButton::hover{"
                       "color: #FFFFFF;"
                       "background-color: %1 ;"
                       "border-color: #2f3640;"
                       "}";
    m_minimizeButton = new QPushButton("", this);
    m_minimizeButton->setIcon(QIcon(":/Icon/minus.png"));
    m_minimizeButton->setStyleSheet(btnStyle.arg("#bdbdbd"));
    m_maximizeButton = new QPushButton("", this);
    m_maximizeButton->setIcon(QIcon(":/Icon/square.png"));
    m_maximizeButton->setStyleSheet(btnStyle.arg("#bdbdbd"));
    m_closeButton = new QPushButton("", this);
    m_closeButton->setIcon(QIcon(":/Icon/cross.png"));
    m_closeButton->setStyleSheet(btnStyle.arg("#d22d1e"));

    m_minimizeButton->setFixedSize(30, 30);
    m_maximizeButton->setFixedSize(30, 30);
    m_closeButton->setFixedSize(30, 30);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_borderWidget);
    layout->addStretch();
    layout->addWidget(m_minimizeButton);
    layout->addWidget(m_maximizeButton);
    layout->addWidget(m_closeButton);
    layout->setContentsMargins(3, 0, 5, 0);
    layout->setSpacing(2);
    setLayout(layout);

    connect(m_minimizeButton, &QPushButton::clicked, this, &CustomTitleBar::minimizeClicked);
    connect(m_maximizeButton, &QPushButton::clicked, this, &CustomTitleBar::maximizeClicked);
    connect(m_closeButton, &QPushButton::clicked, this, &CustomTitleBar::closeClicked);
}

void CustomTitleBar::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
#ifdef Q_OS_WIN
        ReleaseCapture();
        SendMessage(HWND(window()->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
#else
        window()->windowHandle()->startSystemMove();
#endif
        event->accept();
    } else {
        QWidget::mousePressEvent(event);
    }
}

//拦截双击事件做最大化/还原
void CustomTitleBar::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (window()->isMaximized())
            window()->showNormal();
        else
            window()->showMaximized();
        event->accept();
    } else {
        QWidget::mouseDoubleClickEvent(event);
    }
}
