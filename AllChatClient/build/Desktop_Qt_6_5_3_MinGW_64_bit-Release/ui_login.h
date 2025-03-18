/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEditUserName;
    QLineEdit *lineEditPassword;
    QPushButton *btnLogin;
    QCommandLinkButton *btnRegist;

    void setupUi(QDialog *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName("Login");
        Login->resize(237, 268);
        verticalLayout = new QVBoxLayout(Login);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(30, 20, 30, 30);
        lineEditUserName = new QLineEdit(Login);
        lineEditUserName->setObjectName("lineEditUserName");
        lineEditUserName->setMinimumSize(QSize(0, 30));
        lineEditUserName->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lineEditUserName);

        lineEditPassword = new QLineEdit(Login);
        lineEditPassword->setObjectName("lineEditPassword");
        lineEditPassword->setMinimumSize(QSize(0, 30));
        lineEditPassword->setEchoMode(QLineEdit::PasswordEchoOnEdit);
        lineEditPassword->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lineEditPassword);

        btnLogin = new QPushButton(Login);
        btnLogin->setObjectName("btnLogin");
        btnLogin->setMinimumSize(QSize(0, 30));

        verticalLayout->addWidget(btnLogin);

        btnRegist = new QCommandLinkButton(Login);
        btnRegist->setObjectName("btnRegist");
        btnRegist->setMaximumSize(QSize(16777215, 50));
        btnRegist->setStyleSheet(QString::fromUtf8(""));

        verticalLayout->addWidget(btnRegist);


        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QDialog *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "Dialog", nullptr));
        lineEditUserName->setPlaceholderText(QCoreApplication::translate("Login", "\350\276\223\345\205\245\347\224\250\346\210\267\345\220\215", nullptr));
        lineEditPassword->setPlaceholderText(QCoreApplication::translate("Login", "\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        btnLogin->setText(QCoreApplication::translate("Login", "\347\231\273\345\275\225", nullptr));
        btnRegist->setText(QCoreApplication::translate("Login", "\346\262\241\346\234\211\350\264\246\345\217\267\357\274\237\345\216\273\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
