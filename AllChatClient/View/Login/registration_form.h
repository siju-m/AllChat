#ifndef REGISTRATION_FORM_H
#define REGISTRATION_FORM_H

#include <QWidget>
#include <QPainter>
#include <QIntValidator>
#include <QRegularExpressionValidator>
#include <QPropertyAnimation>
#include "input_box.h"
#include "login_button.h"

class Registration_form : public QWidget
{
    Q_OBJECT
public:
    explicit Registration_form(QWidget *parent = nullptr);
    void animations();

    QPropertyAnimation* animation;
    int zoom_rate = 20;

public:
    Input_box* username;
    Input_box* email;
    Input_box* password;
    Login_button* login_button;

    void crop_corner();
    void draw_text();

protected:
    void paintEvent(QPaintEvent* event);

public slots:
    void execute_animation(Login_button::AnimationState State);
};

#endif // REGISTRATION_FORM_H
