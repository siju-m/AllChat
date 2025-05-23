#include "login_form.h"

Login_form::Login_form(QWidget *parent)
    : QWidget{parent}
{
    this->resize(parent->width()/2, parent->height());

    username = new Input_box(":/Icon/account.png", this);
    username->move((width()-username->width())/2, 171);
    username->setPlaceholderText("用户名");
    username->setMaxLength(11);
    username->setValidator(new QIntValidator(0, 2147483647, this));

    password = new Input_box(":/Icon/password.png", this);
    password->move((width()-password->width())/2, 263);
    password->setPlaceholderText("密码");
    password->setEchoMode(QLineEdit::Password);
    password->setMaxLength(16);
    password->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]+$"), this));

    login_button = new Login_button(this);
    login_button->setCenter_text("登录");
    login_button->move((width()-login_button->width())/2, 381);

    this->animations();
    connect(login_button, &Login_button::execute_animation_signal, this, &Login_form::execute_animation);
}

void Login_form::animations()
{
    animation = new QPropertyAnimation(this->login_button, "geometry");
    animation->setDuration(250);
    animation->setStartValue(this->login_button->geometry());
    animation->setEndValue(QRect(this->login_button->pos().x() + zoom_rate, this->login_button->pos().y() + zoom_rate / 2, login_button->width() - zoom_rate * 2, login_button->height() - zoom_rate));
    animation->setEasingCurve(QEasingCurve::Linear);
}

void Login_form::execute_animation(Login_button::AnimationState State)
{
    if (State == Login_button::Execute)
    {
        animation->setDirection(QAbstractAnimation::Forward);
        animation->start();
    }
    else if (State == Login_button::Restore)
    {
        animation->setDirection(QAbstractAnimation::Backward);
        animation->start();
    }
}

void Login_form::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setViewport(0, 0, 477, 620);
    painter.setWindow(0, 0, 477, 620);

    this->crop_corner();
    this->draw_text();
}

void Login_form::crop_corner()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    QBrush Brush(QColor(255, 255, 255, 255));
    painter.setBrush(Brush);
    painter.drawRect(0, 0, width(), height());
}

void Login_form::draw_text()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QRect rect1(0, 0, 0, 0);
    QFont font1;
    font1.setPointSize(30);
    font1.setBold(true);
    font1.setWordSpacing(1);
    painter.setFont(font1);

    QColor semiTransparent(0, 0, 0, 255);
    painter.setPen(semiTransparent);

    QRect actualRect = painter.boundingRect(rect1, Qt::AlignCenter, "登录");
    rect1.setHeight(actualRect.height());
    rect1.setWidth(actualRect.width());
    rect1.moveCenter(QPoint(width() / 2, height() / 6));
    painter.drawText(rect1, "登录");
}
