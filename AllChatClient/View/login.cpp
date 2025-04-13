#include "login.h"
#include "View/Components/tipsbox.h"

#include <Core/currentuser.h>


Login::Login(DataTransfer *dataTransfer, QObject *parent)
    : QObject{parent}
    , m_dataTransfer(dataTransfer)
{
    m_login_regist_view = new Responsive_form();
    m_login_regist_view->show();

    QObject::connect(m_dataTransfer,&DataTransfer::loginResult,m_login_regist_view,&Responsive_form::closeWindow);
    QObject::connect(m_dataTransfer,&DataTransfer::loginResult,this,&Login::handle_loginResult);
    QObject::connect(m_dataTransfer,&DataTransfer::registResult,this,&Login::handle_registResult);
    QObject::connect(m_login_regist_view, &Responsive_form::login, this, &Login::loginUser);
    QObject::connect(m_login_regist_view, &Responsive_form::regist, this, &Login::registerUser);
}

Login::~Login()
{
    m_login_regist_view->deleteLater();
}

void Login::loginUser(const QString &username, const QString &password) {
    // qDebug()<<username<<password;

    CurrentUser::getInstance()->set_userName(username);
    Packet data(CommonEnum::message_type::LOGIN,username,password);
    m_dataTransfer->sendData(data);
}

void Login::registerUser(const QString &username, const QString &password) {
    if (username.isEmpty() || password.isEmpty()) return;
    Packet data(CommonEnum::message_type::REGISTER,username,password);
    m_dataTransfer->sendData(data);
}

void Login::handle_loginResult(CommonEnum::message_type result)
{
    if(result == CommonEnum::message_type::LOGIN_SUCCESS){
        emit login_success();
    }

}

void Login::handle_registResult(CommonEnum::message_type result)
{
    switch(result){
    case CommonEnum::REGISTER_SUCCESS:{
        qDebug()<<"注册成功";
        // 切换到登录界面
        TipsBox::showNotice("注册成功", SA_SUCCESS, m_login_regist_view);
        m_login_regist_view->goToLoginPage();
    }break;
    case CommonEnum::REGISTER_FAILED:{
        qDebug()<<"注册失败";
        TipsBox::showNotice("用户名已存在", SA_FAILED, m_login_regist_view);
    }break;
    default:break;
    }
}
