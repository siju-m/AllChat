#ifndef CONFIRMBOX_H
#define CONFIRMBOX_H

#include <QDialog>

class ConfirmBox : public QDialog
{
    Q_OBJECT
public:
    explicit ConfirmBox(QWidget *parent = nullptr);

    static bool question(QWidget *parent = nullptr);
signals:
};

#endif // CONFIRMBOX_H
