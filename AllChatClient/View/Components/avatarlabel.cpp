#include "avatarlabel.h"

#include <QMouseEvent>

AvatarLabel::AvatarLabel(QWidget* parent) : QLabel(parent){}

void AvatarLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked(); // 发射点击信号
    }
    QLabel::mousePressEvent(event); // 保留原有行为
}
