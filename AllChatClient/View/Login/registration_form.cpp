#include "registration_form.h"

Registration_form::Registration_form(QWidget *parent)
    : QWidget{parent}
{
    this->resize(parent->width()/2, parent->height());

    username = new Input_box(":/Icon/account.png", this);
    username->move((width()-username->width())/2, 140);
    username->setPlaceholderText("用户名");
    username->setMaxLength(11);
    username->setValidator(new QIntValidator(0, 2147483647, this));

    password1 = new Input_box(":/Icon/password.png", this);
    password1->move((width()-username->width())/2, 230);
    password1->setPlaceholderText("密码");
    password1->setMaxLength(16);
    // email->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}"), this));
    password1->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]+$"), this));

    password2 = new Input_box(":/Icon/password.png", this);
    password2->move((width()-username->width())/2, 320);
    password2->setPlaceholderText("确认密码");
    password2->setEchoMode(QLineEdit::Password);
    password2->setMaxLength(16);
    password2->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]+$"), this));

    login_button = new Login_button(this);
    login_button->setCenter_text("注册");
    login_button->move((width()-username->width())/2, 410);

    this->animations();
    connect(login_button, &Login_button::execute_animation_signal, this, &Registration_form::execute_animation);
}

void Registration_form::animations()
{
    animation = new QPropertyAnimation(this->login_button, "geometry");
    animation->setDuration(250);
    animation->setStartValue(this->login_button->geometry());
    animation->setEndValue(QRect(this->login_button->pos().x() + zoom_rate, this->login_button->pos().y() + zoom_rate / 2, login_button->width() - zoom_rate * 2, login_button->height() - zoom_rate));
    animation->setEasingCurve(QEasingCurve::Linear);
}

void Registration_form::execute_animation(Login_button::AnimationState State)
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

void Registration_form::paintEvent(QPaintEvent* event)
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

void Registration_form::crop_corner()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    QBrush Brush(QColor(255, 255, 255, 255));
    painter.setBrush(Brush);
    painter.drawRect(0, 0, width(), height());
}

void Registration_form::draw_text()
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

    QRect actualRect = painter.boundingRect(rect1, Qt::AlignCenter, "注册");
    rect1.setHeight(actualRect.height());
    rect1.setWidth(actualRect.width());
    rect1.moveCenter(QPoint(width() / 2, height() / 8));
    painter.drawText(rect1, "注册");
}
