#include "onlinestatelabel.h"

#include <QPainter>

OnlineStateLabel::OnlineStateLabel(QWidget *parent):QLabel(parent) {}

void OnlineStateLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event); // 先调用基类的绘制事件

    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing); // 开启抗锯齿
    painter.setPen(Qt::NoPen);//无边框

    // 绘制一个红色圆形
    switch(m_onlineState){
    case StateEnum::ONLINE:painter.setBrush(Qt::green);break;
    case StateEnum::OFFLINE:painter.setBrush(Qt::gray);break;
    case StateEnum::BUSY:painter.setBrush(Qt::yellow);break;
    default:break;
    }
    painter.drawEllipse(5, 5, 20, 20);
    painter.restore();
}

void OnlineStateLabel::setOnlineState(StateEnum::onlineState_type state)
{
    if (m_onlineState != state) { // 避免重复更新
        m_onlineState = state;
        update(); // 通知 Qt 重新绘制控件
    }
}
