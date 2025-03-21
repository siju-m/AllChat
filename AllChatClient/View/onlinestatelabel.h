#ifndef ONLINESTATELABEL_H
#define ONLINESTATELABEL_H

#include <QLabel>
#include "Utils/CustomTypes.h"

class OnlineStateLabel : public QLabel
{
public:
    OnlineStateLabel(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void setOnlineState(StateEnum::onlineState_type state);
private:
    StateEnum::onlineState_type m_onlineState = StateEnum::OFFLINE;
};

#endif // ONLINESTATELABEL_H
