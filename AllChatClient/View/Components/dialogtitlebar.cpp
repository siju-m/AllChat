#include "dialogtitlebar.h"
#include "qwindow.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <windows.h>

DialogTitleBar::DialogTitleBar(QWidget *parent)
    : QWidget{parent}
{
    m_btnClose = new QPushButton("", this);
    m_btnClose->setIcon(QIcon(":/Icon/cross.png"));
    m_btnClose->setStyleSheet("QPushButton{"
                              "border:none;"
                              "padding:5px;"
                              "}"
                              "QPushButton::hover{	"
                              "color: #FFFFFF;"
                              "background-color: #d22d1e;"
                              "border-color: #2f3640;"
                              "}");
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addStretch();
    layout->addWidget(m_btnClose);
    layout->setContentsMargins(0, 0, 0, 0);

    connect(m_btnClose, &QPushButton::clicked, this, &DialogTitleBar::closeWindow);
}

void DialogTitleBar::mousePressEvent(QMouseEvent *event) {
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
