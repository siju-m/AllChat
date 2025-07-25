#include "responsive_form.h"
#include "View/Components/tipsbox.h"

#include <QMessageBox>
#include <windows.h>

Responsive_form::Responsive_form(QWidget *parent)
    : QWidget{parent}
{
    this->setFixedSize(800, 520);
    this->setWindowFlags(Qt::FramelessWindowHint);

    transparent_transition_interface2 = new Transparent_transition_interface("欢迎!", "已经拥有账户?", "登录", this);
    transparent_transition_interface2->button->animation_status(false);
    transparent_transition_interface2->move(this->width(), 0);

    registration_form = new Registration_form(this);
    registration_form->move(width(), 0);
    // 转发注册请求
    connect(registration_form->login_button, &Login_button::clicked, this, [=](){
        QString userName = registration_form->username->text();
        QString pw1 = registration_form->password1->text();
        QString pw2 = registration_form->password2->text();

        if(pw1.isEmpty() || pw2.isEmpty()){
            TipsBox::showNotice("密码不能为空!", SA_TIPS, this);
        }else if(pw1 == pw2){
            emit regist(userName, pw1);
        }else{
            TipsBox::showNotice("密码不一致!", SA_WARNING, this);
        }

    });

    login_form = new Login_form(this);
    login_form->move(this->width() / 2, 0);
    // 转发登录请求
    connect(login_form->login_button, &Login_button::clicked, this, [=](){
        QString userName = login_form->username->text();
        QString passWord = login_form->password->text();
        if(userName.isEmpty() || passWord.isEmpty()){
            TipsBox::showNotice("用户名和密码不能为空!", SA_TIPS, this);
        }else
            emit login(userName, passWord);
    });

    scroll_bar = new Scroll_bar(this);
    scroll_bar->move(-width() * 1.55, 0);

    transparent_transition_interface = new Transparent_transition_interface("你好, 欢迎!", "没有账号?", "注册", this);
    transparent_transition_interface->move(0, 0);

    this->setRightShow();
    this->build_animation();

    connect(transparent_transition_interface->button, &Hollow_button::page_changed, this, &Responsive_form::execute_animation);
    connect(transparent_transition_interface2->button, &Hollow_button::page_changed, this, &Responsive_form::execute_animation);

    QPushButton *btnClose = new QPushButton("", this);
    btnClose->setIcon(QIcon(":/Icon/cross.png"));
    btnClose->setStyleSheet("QPushButton{"
                              "border:none;"
                              "padding:5px;"
                              "}"
                              "QPushButton::hover{	"
                              "color: #FFFFFF;"
                              "background-color: #d22d1e;"
                              "border-color: #2f3640;"
                              "}");
    btnClose->setFixedSize(30, 30);
    btnClose->move(this->width()-btnClose->width(), 0);
    connect(btnClose, &QPushButton::clicked, this, &Responsive_form::close);
}

void Responsive_form::setRightShow()
{
    transparent_transition_interface2->raise();
    registration_form->lower();
    transparent_transition_interface->raise();
    login_form->lower();
}

void Responsive_form::build_animation()
{
    animation2 = new QPropertyAnimation(this->scroll_bar, "pos");
    animation2->setDuration(m_animation_duration);
    animation2->setStartValue(this->scroll_bar->pos());
    animation2->setEndValue(QPoint(this->width() / 2*1.1, 0));
    animation2->setEasingCurve(QEasingCurve::Linear);

    animation3 = new QPropertyAnimation(this->transparent_transition_interface, "pos");
    animation3->setDuration(m_animation_duration);
    animation3->setStartValue(this->transparent_transition_interface->pos());
    animation3->setEndValue(QPoint(-this->width() / 2, 0));
    animation3->setEasingCurve(QEasingCurve::Linear);

    animation4 = new QPropertyAnimation(this->transparent_transition_interface2, "pos");
    animation4->setDuration(m_animation_duration);
    animation4->setStartValue(this->transparent_transition_interface2->pos());
    animation4->setEndValue(QPoint(this->width() / 2*1.1, 0));
    animation4->setEasingCurve(QEasingCurve::Linear);

    animation5 = new QPropertyAnimation(this->registration_form, "pos");
    animation5->setDuration(m_animation_duration);
    animation5->setStartValue(this->registration_form->pos());
    animation5->setEndValue(QPoint(0, 0));
    animation5->setEasingCurve(QEasingCurve::Linear);

    animation6 = new QPropertyAnimation(this->login_form, "pos");
    animation6->setDuration(m_animation_duration);
    animation6->setStartValue(this->login_form->pos());
    animation6->setEndValue(QPoint(-this->width() / 2, 0));
    animation6->setEasingCurve(QEasingCurve::Linear);

    connect(animation2, &QPropertyAnimation::valueChanged, this, [this] {
        if (scroll_bar->pos().x() > -this->width() / 2 && animation4->state() == QAbstractAnimation::Stopped && animation_execute_duration)
        {
            animation2->pause();
            animation3->setDirection(QAbstractAnimation::Forward);
            animation3->start();
            animation_execute_duration = false;
        }
        else if (scroll_bar->pos().x() < -this->width() / 10 && animation3->state() == QAbstractAnimation::Stopped && animation_restore_duration)
        {
            animation2->pause();
            animation4->setDirection(QAbstractAnimation::Backward);
            animation4->start();
            animation_restore_duration = false;
        }
    });

    connect(animation3, &QPropertyAnimation::finished, this, [this] {
        if (animation2->state() == QAbstractAnimation::Paused) {
            animation2->resume();
        }
    });

    connect(animation4, &QPropertyAnimation::finished, this, [this] {
        if (animation2->state() == QAbstractAnimation::Paused) {
            animation2->resume();
        }
    });

    connect(animation3, &QPropertyAnimation::finished, this, &Responsive_form::onAnimation3Finished);
    connect(animation4, &QPropertyAnimation::finished, this, &Responsive_form::onAnimation4Finished);
}

void Responsive_form::onAnimation3Finished()
{
    if (currentSequence == 1)
    {
        animation4->setDirection(QAbstractAnimation::Forward);
        animation4->start();
        animation5->setDirection(QAbstractAnimation::Forward);
        animation5->start();
    }
}
void Responsive_form::onAnimation4Finished()
{
    if (currentSequence == 1) return;

    else if (currentSequence == 2)
    {
        animation3->setDirection(QAbstractAnimation::Backward);
        animation3->start();

        animation6->setDirection(QAbstractAnimation::Backward);
        animation6->start();
    }
}

void Responsive_form::execute_animation(Hollow_button::AnimationState status)
{
    if (status == Hollow_button::AnimationState::ANIMATION_STATE_EXECUTING)
    {
        animation_execute_duration = true;
        currentSequence = 1;

        animation2->setDirection(QAbstractAnimation::Forward);
        animation2->start();

        animation6->setDirection(QAbstractAnimation::Forward);
        animation6->start();
    }
    else if (status == Hollow_button::AnimationState::ANIMATION_STATE_RESET)
    {
        animation_restore_duration = true;
        currentSequence = 2;

        animation2->setDirection(QAbstractAnimation::Backward);
        animation2->start();
    }
}

void Responsive_form::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setViewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());

    this->crop_corner();
}

void Responsive_form::crop_corner()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPainterPath path;
    path.addRoundedRect(0, 0, width(), height(), 0, 0);
    painter.setClipPath(path);
    painter.setPen(Qt::NoPen);

    QBrush Brush(QColor(255, 255, 255, 255));
    painter.setBrush(Brush);
    painter.drawRect(0, 0, width(), height());
}

void Responsive_form::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return) {

        if(currentSequence == 2){
            // 登录
            emit login_form->login_button->clicked();
        }else if(currentSequence == 1){
            // 注册
            emit registration_form->login_button->clicked();
        }
    }
}

void Responsive_form::createRoundPath(QPainterPath& path)
{
    path.addRoundedRect(0, 0, width(), height(), 35, 35);
    path.closeSubpath();
}

void Responsive_form::mousePressEvent(QMouseEvent* event)
{

    if (event->button() == Qt::LeftButton) {
        QWidget *w = childAt(event->pos());
        // 如果不是按钮，就可以拖动
        if (!qobject_cast<QAbstractButton*>(w)) {
#ifdef Q_OS_WIN
            ReleaseCapture();
            SendMessage(HWND(window()->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
#else
            window()->windowHandle()->startSystemMove();
#endif
        }
    }
    QWidget::mousePressEvent(event);
}

int Responsive_form::animation_duration() const
{
    return m_animation_duration;
}

void Responsive_form::setAnimation_duration(int newAnimation_duration)
{
    m_animation_duration = newAnimation_duration;
}

void Responsive_form::closeWindow(CommonEnum::message_type result){
    switch(result){
    case CommonEnum::message_type::LOGIN_SUCCESS:{
        this->close();
    }break;
    case CommonEnum::message_type::LOGIN_FAILED:{
        TipsBox::showNotice("密码错误或用户不存在!", SA_FAILED, this);
    }break;
    default:break;
    }

}

void Responsive_form::goToLoginPage()
{
    execute_animation(Hollow_button::ANIMATION_STATE_RESET);
}
