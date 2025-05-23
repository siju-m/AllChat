#include "control.h"

Control::Control(QObject *parent)
    : QObject{parent}
{
    ChatHistoryManager *historyManager = new ChatHistoryManager(this);
    m_window = new MainWindow(historyManager);
    m_login = new Login(historyManager);

    QObject::connect(m_login, &Login::login_success, this, [=](){
        m_login->deleteLater();
        historyManager->initDatabase();
        m_window->show();
    });
}

Control::~Control()
{
    m_window->deleteLater();
}

