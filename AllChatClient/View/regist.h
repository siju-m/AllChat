#ifndef REGIST_H
#define REGIST_H

#include <QDialog>
#include <QMessageBox>
#include "Utils/CustomTypes.h"

namespace Ui {
class Regist;
}

class Regist : public QDialog
{
    Q_OBJECT

public:
    explicit Regist(QWidget *parent = nullptr);
    ~Regist();
public slots:
    void closeWindow(CommonEnum::message_type result);
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void on_btnRegist_clicked();
signals:
    void regist(const QString &username, const QString &password);
private:
    Ui::Regist *ui;
};

#endif // REGIST_H
