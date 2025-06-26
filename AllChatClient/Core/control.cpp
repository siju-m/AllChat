#include "control.h"


Control::Control(QObject *parent)
    : QObject{parent},
    m_historyManager(ChatHistoryManager::getInstance())
{
    m_window = new MainWindow();
    m_login = new Login();

    QObject::connect(m_login, &Login::login_success, this, [=](){
        m_login->deleteLater();
        m_historyManager->initDatabase();
        m_window->show();
    });

}

Control::~Control()
{
    m_window->deleteLater();
}

