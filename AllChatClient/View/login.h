#ifndef LOGIN_H
#define LOGIN_H

#include "View/Login/responsive_form.h"
#include <QObject>
#include <Core/datatransfer.h>

class Login : public QObject
{
    Q_OBJECT
public:
    explicit Login(DataTransfer *dataTransfer, QObject *parent = nullptr);
    ~Login();

    void loginUser(const QString &username, const QString &password);
    void registerUser(const QString &username, const QString &password);
    void handle_loginResult(CommonEnum::message_type result);
    void handle_registResult(CommonEnum::message_type result);
signals:
    void login(const QString &username, const QString &password);
    void login_success();
private:
    Responsive_form* m_login_regist_view;
    DataTransfer* m_dataTransfer;
};

#endif // LOGIN_H
