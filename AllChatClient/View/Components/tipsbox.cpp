#include "tipsbox.h"

#include <QPainter>
#include <QDebug>
#include <QResizeEvent>
#include <QHBoxLayout>
#include <QGuiApplication>
TipsBox::TipsBox(const QString &text, const MSG_TIP_TYPE &type, const quint16 &duration, QWidget *parent) :
    QWidget(parent),
    m_nTipType(type),
    count(duration),
    m_text(text)
{
    // this->setFixedSize(350,80);

    QGridLayout *layout1 = new QGridLayout(this);
    m_frame = new QFrame(this);
    layout1->addWidget(m_frame);
    this->setLayout(layout1);
    QHBoxLayout *layout2 = new QHBoxLayout(m_frame);
    m_textLabel = new QLabel(this);
    m_iconLabel = new QLabel(this);
    m_iconLabel->setFixedWidth(40);

    layout2->addWidget(m_iconLabel);
    layout2->addWidget(m_textLabel);
    m_frame->setLayout(layout2);

    ShadowEffectInit();             //设置提示框的阴影效果

    initAnimation(parent);

    // 设置窗体关闭时自动释放内存
    this->setAttribute(Qt::WA_DeleteOnClose);
}

TipsBox::~TipsBox()
{
}

void TipsBox::showNotice(const QString &text, const MSG_TIP_TYPE &type, QWidget *parent, const quint16 &duration)
{
    TipsBox *box = new TipsBox(text, type, duration, parent);
    box->show();
}

void TipsBox::ShadowEffectInit()
{
    time = new QTimer(this);
    connect(time,&QTimer::timeout,this,[=](){

        count--;
        // qDebug()<<"duration"<<count;
        if(count <= 0)
        {
            // 当计数结束时，开始动画回退
            animation.setDirection(QAbstractAnimation::Backward);
            animation.start();

            QTimer::singleShot(200, this, [=](){
                fadeAnimation->start();

                connect(fadeAnimation,&QPropertyAnimation::finished,this,[=](){
                    this->deleteLater();
                });
            });
        }
    });

    time->start(1000);
    QString iconStyle = "QLabel{"
                        "image: url(:/Icon/%1.png);"
                        "background:transparent;"
                        "}";
    QString framStyle = ".QFrame{"
                        "background-color: %1 ;"
                        "border: 1px solid %2 ;"
                        "border-radius:8px;"
                        "}";
    QString textStyle = "QLabel{"
                        "font:24px;"
                        "color: %1 ;"
                        "background:transparent;"
                        "Font-weight:bold;"
                        "}";
    if(MSG_TIP_TYPE::SA_WARNING == m_nTipType){

        m_iconLabel->setStyleSheet(iconStyle.arg("warning"));
        m_frame->setStyleSheet(framStyle.arg("rgb(254, 253, 236)","rgb(250, 236, 216)"));
        m_textLabel->setStyleSheet(textStyle.arg("rgb(231, 165, 65)"));

    }else if(MSG_TIP_TYPE::SA_FAILED == m_nTipType){

        m_iconLabel->setStyleSheet(iconStyle.arg("error"));
        m_frame->setStyleSheet(framStyle.arg("rgb(254, 240, 240)","rgb(253, 226, 226)"));
        m_textLabel->setStyleSheet(textStyle.arg("rgb(245, 115, 123)"));

    }
    else if(MSG_TIP_TYPE::SA_SUCCESS == m_nTipType){

        m_iconLabel->setStyleSheet(iconStyle.arg("success"));
        m_frame->setStyleSheet(framStyle.arg("rgb(240, 249, 235)","rgb(225, 243, 216)"));
        m_textLabel->setStyleSheet(textStyle.arg("rgb(108, 195, 66)"));

    }
    else if(MSG_TIP_TYPE::SA_TIPS == m_nTipType){

        m_iconLabel->setStyleSheet(iconStyle.arg("tips"));
        m_frame->setStyleSheet(framStyle.arg("rgb(237, 242, 252)","rgb(237, 240, 246)"));
        m_textLabel->setStyleSheet(textStyle.arg("rgb(157, 160, 167)"));

    }

    m_textLabel->setText(m_text);

    //设置窗体透明
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    //设置无边框
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // 在类的初始化或构造函数中添加
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);                // 阴影偏移量，可以设置为(0, 0)贴紧控件，或正负值产生偏移
    shadow->setColor(QColor(0, 0, 0,20)); // 阴影颜色及透明度
    //设置阴影圆角
    shadow->setBlurRadius(8);
    m_frame->setGraphicsEffect(shadow);           //必须对frame设置阴影 否则报错
    //给垂直布局器设置边距(此步很重要, 设置宽度为阴影的宽度)
    this->setContentsMargins(2,2,2,2);
}

void TipsBox::initAnimation(QWidget *parent)
{
    // 初始化动画
    animation.setTargetObject(this);
    animation.setPropertyName("pos");
    animation.setDuration(800); // 动画持续时间为500毫秒

    // 自动居中 parent 或屏幕
    QRect refRect;
    if (parent) {
        refRect = parent->geometry();
    } else {
        QScreen *screen = QGuiApplication::primaryScreen();
        refRect = screen->availableGeometry();
    }

    int x = (refRect.width() - this->sizeHint().width()) / 2 + refRect.x();
    int y = (refRect.height() - this->sizeHint().height()) / 2 + refRect.y();
    animation.setStartValue(QPoint(x , y - (this->sizeHint().height()/2)));               // 开始位置在窗口上方
    animation.setEndValue(QPoint(x , y));       // 结束位置在窗口中央

    // 创建淡出动画
    fadeAnimation = new QPropertyAnimation(this, "windowOpacity");

    fadeAnimation->setDuration(300);    // 淡出动画持续时间
    fadeAnimation->setStartValue(1.0);  // 开始时完全不透明
    fadeAnimation->setEndValue(0.5);    // 结束时完全透明

    animation.setEasingCurve(QEasingCurve::InOutQuad);
    animation.start();
}
