#ifndef CONFIRMBOX_H
#define CONFIRMBOX_H

#include <QDialog>

class ConfirmBox : public QDialog
{
    Q_OBJECT
public:
    ConfirmBox(QWidget *parent = nullptr, QString title_s="", QString subTitle_s="", QString confirm_s="", QString cancel_s="");

    static bool question(QWidget *parent = nullptr, QString title_s="", QString subTitle_s="", QString confirm_s="", QString cancel_s="");
signals:
};

#endif // CONFIRMBOX_H
