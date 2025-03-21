/********************************************************************************
** Form generated from reading UI file 'regist.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGIST_H
#define UI_REGIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Regist
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *userName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *password;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *confirmPassword;
    QPushButton *btnRegist;

    void setupUi(QDialog *Regist)
    {
        if (Regist->objectName().isEmpty())
            Regist->setObjectName("Regist");
        Regist->resize(340, 274);
        verticalLayout = new QVBoxLayout(Regist);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(Regist);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);

        userName = new QLineEdit(Regist);
        userName->setObjectName("userName");

        horizontalLayout->addWidget(userName);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 2);

        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_2 = new QLabel(Regist);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_2);

        password = new QLineEdit(Regist);
        password->setObjectName("password");

        horizontalLayout_2->addWidget(password);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 2);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_3 = new QLabel(Regist);
        label_3->setObjectName("label_3");
        label_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_3);

        confirmPassword = new QLineEdit(Regist);
        confirmPassword->setObjectName("confirmPassword");

        horizontalLayout_3->addWidget(confirmPassword);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 2);

        verticalLayout->addLayout(horizontalLayout_3);

        btnRegist = new QPushButton(Regist);
        btnRegist->setObjectName("btnRegist");
        btnRegist->setStyleSheet(QString::fromUtf8("/**\346\255\243\345\270\270\346\203\205\345\206\265\344\270\213\346\240\267\345\274\217**/\n"
"QPushButton{\n"
"	font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"    color: white;\n"
"    background-color: gray;\n"
"    border-color: #2f3640;\n"
"    border-radius: 5px;\n"
"    border-style: none;\n"
"    border-width: 1px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"/**\351\274\240\346\240\207\345\201\234\347\225\231\345\234\250\346\214\211\351\222\256\344\270\212\347\232\204\346\240\267\345\274\217**/\n"
"QPushButton::hover{	\n"
"    color: #FFFFFF;\n"
"    background-color: #718093;\n"
"    border-color: #2f3640;\n"
"}\n"
"\n"
"/**\351\274\240\346\240\207\346\214\211\345\216\213\344\270\213\345\216\273\347\232\204\346\240\267\345\274\217**/\n"
"QPushButton::pressed,QPushButton::checked{\n"
"    color: #FFFFFF;\n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #273c75, stop:1 #487eb0);\n"
"}\n"
"\n"
"/**\346\214\211\351\222\256\345\244\261\350\203\275\346\203\205"
                        "\345\206\265\344\270\213\346\240\267\345\274\217**/\n"
"QPushButton::disabled{\n"
"    color: #FFFFFF;\n"
"    background-color: #dcdde1;\n"
"    border-color: #dcdde1;\n"
"}\n"
""));

        verticalLayout->addWidget(btnRegist);


        retranslateUi(Regist);

        QMetaObject::connectSlotsByName(Regist);
    } // setupUi

    void retranslateUi(QDialog *Regist)
    {
        Regist->setWindowTitle(QCoreApplication::translate("Regist", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("Regist", "\347\224\250\346\210\267\345\220\215", nullptr));
        label_2->setText(QCoreApplication::translate("Regist", "\345\257\206\347\240\201", nullptr));
        label_3->setText(QCoreApplication::translate("Regist", "\347\241\256\350\256\244\345\257\206\347\240\201", nullptr));
        btnRegist->setText(QCoreApplication::translate("Regist", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Regist: public Ui_Regist {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGIST_H
