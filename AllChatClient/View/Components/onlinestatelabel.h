#ifndef ONLINESTATELABEL_H
#define ONLINESTATELABEL_H

#include <QLabel>
#include <QRect>
#include "Utils/CustomTypes.h"

class OnlineStateLabel : public QLabel
{
public:
    OnlineStateLabel(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void setOnlineState(StateEnum::onlineState_type state,const QRect &position={5, 5, 20, 20});
private:
    StateEnum::onlineState_type m_onlineState = StateEnum::NONE;
    QRect m_position;
};

#endif // ONLINESTATELABEL_H
