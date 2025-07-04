#include "scroll_bar.h"

Scroll_bar::Scroll_bar(QWidget *parent)
    : QWidget{parent}
{
    this->resize(parent->width()*2, parent->height());
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

void Scroll_bar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setViewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());

    this->crop_corner();
}

void Scroll_bar::crop_corner()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.addRoundedRect(0, 0, width(), height(), height() / 7, height() / 7);
    painter.setClipPath(path);
    painter.setPen(Qt::NoPen);

    QBrush Brush(QColor(53, 93, 144, 255));
    painter.setBrush(Brush);
    painter.drawRect(0, 0, width(), height());
}
