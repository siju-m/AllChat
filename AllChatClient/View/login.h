#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QPushButton>
#include <QMessageBox>
#include "Utils/CustomTypes.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    QString userName;
    QPushButton *btnRegist;//跳转注册的按钮变量
public slots:
    void closeWindow(CommonEnum::message_type result);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private slots:
    void on_btnLogin_clicked();
signals:
    void login(const QString &username, const QString &password);
private:
    Ui::Login *ui;
    QPointF m_offset;
};

#endif // LOGIN_H
