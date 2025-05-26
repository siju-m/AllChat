#ifndef CONTROL_H
#define CONTROL_H

#include "MainWindow.h"
#include "View/login.h"
#include <QObject>

#include "ElaWidget.h"

class Control : public QObject
{
    Q_OBJECT
public:
    explicit Control(QObject *parent = nullptr);
    ~Control();
signals:

private:
    MainWindow *m_window;
    Login* m_login;
    ElaWidget* ela;
};

#endif // CONTROL_H
