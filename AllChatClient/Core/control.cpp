#include "control.h"

Control::Control(QObject *parent)
    : QObject{parent}
{

    m_dataTransfer = new DataTransfer(this);
    m_window = new MainWindow(m_dataTransfer);
    m_login = new Login(m_dataTransfer);

    QObject::connect(m_login, &Login::login_success, this, [=](){
        m_login->deleteLater();
        m_window->show();
    });
}

Control::~Control()
{
    m_window->deleteLater();
}

