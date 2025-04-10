#include "Core/control.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Control c =  Control();
    // Regist regist;
    //todo 数据处理和iu线程分离
    // QObject::connect(&regist,&Regist::regist,&w,&MainWindow::registerUser);//处理注册按钮
    // QObject::connect(&w,&MainWindow::registResult,&regist,&Regist::closeWindow);
    // QObject::connect(login.btnRegist,&QPushButton::clicked,&regist,&Regist::exec);//绑定跳转注册按钮

    return a.exec();


}
