#include "control.h"

Control::Control(QObject *parent)
    : QObject{parent}
{
    initLogin();
    w = new MainWindow();
    QObject::connect(r, &Responsive_form::login, w, &MainWindow::loginUser);//处理登录按钮
    QObject::connect(r, &Responsive_form::login_success, this, [=](){
        r->close();
        r->deleteLater();
        w->show();
    });
    QObject::connect(w,&MainWindow::loginResult,r,&Responsive_form::closeWindow);
}

Control::~Control()
{
    w->deleteLater();
}

void Control::initLogin()
{
    r = new Responsive_form();
    r->show();
}
