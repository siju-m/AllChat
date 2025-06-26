#ifndef CONTROL_H
#define CONTROL_H

#include "MainWindow.h"
#include "View/login.h"
#include <QObject>


class Control : public QObject
{
    Q_OBJECT
public:
    explicit Control(QObject *parent = nullptr);
    ~Control();
signals:

private:
    ChatHistoryManager* m_historyManager;
    MainWindow *m_window;
    Login* m_login;
};

#endif // CONTROL_H
