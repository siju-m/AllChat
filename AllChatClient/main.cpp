#include "mainwindow.h"
#include "View\login.h"
#include "View\Regist.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login login;
    Regist regist;
    MainWindow w;
    //todo 主窗口更新ui，数据通信应该单独一个类来处理，然后数据处理一个类
    QObject::connect(&login,&Login::login,&w,&MainWindow::loginUser);//处理登录按钮
    QObject::connect(&regist,&Regist::regist,&w,&MainWindow::registerUser);//处理注册按钮
    QObject::connect(&w,&MainWindow::loginResult,&login,&Login::closeWindow);//根据登录结果决定是否关闭窗口
    QObject::connect(&w,&MainWindow::registResult,&regist,&Regist::closeWindow);
    QObject::connect(login.btnRegist,&QPushButton::clicked,&regist,&Regist::exec);//绑定跳转注册按钮
    w.ConnectServer();
    if(login.exec()==QDialog::Accepted){
        w.resize(1000, 700);
        w.show();
        return a.exec();
    }else return 0;


}
