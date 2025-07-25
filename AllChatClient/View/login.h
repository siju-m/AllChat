#ifndef LOGIN_H
#define LOGIN_H

#include "Core/chathistorymanager.h"
#include "View/Login/responsive_form.h"
#include <QObject>
#include <Core/datatransfer.h>

class Login : public QObject
{
    Q_OBJECT
public:
    explicit Login(QObject *parent = nullptr);
    ~Login();

    void loginUser(const QString &username, const QString &password);
    void registerUser(const QString &username, const QString &password);
    void handle_loginResult(CommonEnum::message_type result, QByteArray data);
    void handle_registResult(CommonEnum::message_type result);

private:
    void updateUserInfo(QByteArray &data);

signals:
    void login(const QString &username, const QString &password);
    void login_success();

private:
    Responsive_form *m_login_regist_view;
    DataTransfer *m_dataTransfer;
    ChatHistoryManager *m_historyManager;
};

#endif // LOGIN_H
