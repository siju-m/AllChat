#ifndef LOGIN_FORM_H
#define LOGIN_FORM_H

#include <QWidget>
#include <QPainter>
#include <QIntValidator>
#include <QRegularExpressionValidator>
#include <QPropertyAnimation>
#include "input_box.h"
#include "login_button.h"

class Login_form : public QWidget
{
    Q_OBJECT
public:
    explicit Login_form(QWidget *parent = nullptr);
    void crop_corner();
    void draw_text();
    void animations();

    QPropertyAnimation* animation;
    int zoom_rate = 20;
signals:
    void login(const QString &username, const QString &password);
public slots:
    void execute_animation(Login_button::AnimationState State);

protected:
    void paintEvent(QPaintEvent *event);

public:
    Input_box* username;
    Input_box* password;
    Login_button* login_button;
};

#endif // LOGIN_FORM_H
