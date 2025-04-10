#ifndef CONTROL_H
#define CONTROL_H

#include "MainWindow.h"
#include "View/Login/responsive_form.h"
#include <QObject>

class Control : public QObject
{
    Q_OBJECT
public:
    explicit Control(QObject *parent = nullptr);
    ~Control();
    void initLogin();
signals:

private:
    MainWindow *w;
    Responsive_form *r;
};

#endif // CONTROL_H
