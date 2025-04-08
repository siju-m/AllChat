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
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Regist
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QLineEdit *userName;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_2;
    QLineEdit *password;
    QSpacerItem *horizontalSpacer_4;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_3;
    QLineEdit *confirmPassword;
    QSpacerItem *horizontalSpacer_6;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *btnRegist;
    QSpacerItem *horizontalSpacer_8;

    void setupUi(QDialog *Regist)
    {
        if (Regist->objectName().isEmpty())
            Regist->setObjectName("Regist");
        Regist->resize(220, 230);
        verticalLayout = new QVBoxLayout(Regist);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget_4 = new QWidget(Regist);
        widget_4->setObjectName("widget_4");
        horizontalLayout_6 = new QHBoxLayout(widget_4);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        label = new QLabel(widget_4);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(label);

        userName = new QLineEdit(widget_4);
        userName->setObjectName("userName");
        userName->setMinimumSize(QSize(0, 30));
        userName->setMaximumSize(QSize(110, 30));
        userName->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"border-radius: 5px;\n"
"};"));

        horizontalLayout_6->addWidget(userName);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(widget_4);

        widget_3 = new QWidget(Regist);
        widget_3->setObjectName("widget_3");
        horizontalLayout_3 = new QHBoxLayout(widget_3);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        label_2 = new QLabel(widget_3);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_2);

        password = new QLineEdit(widget_3);
        password->setObjectName("password");
        password->setMinimumSize(QSize(0, 30));
        password->setMaximumSize(QSize(110, 30));
        password->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"border-radius: 5px;\n"
"};"));

        horizontalLayout_3->addWidget(password);

        horizontalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout->addWidget(widget_3);

        widget_2 = new QWidget(Regist);
        widget_2->setObjectName("widget_2");
        horizontalLayout_5 = new QHBoxLayout(widget_2);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);

        label_3 = new QLabel(widget_2);
        label_3->setObjectName("label_3");
        label_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(label_3);

        confirmPassword = new QLineEdit(widget_2);
        confirmPassword->setObjectName("confirmPassword");
        confirmPassword->setMinimumSize(QSize(0, 30));
        confirmPassword->setMaximumSize(QSize(110, 30));
        confirmPassword->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"border-radius: 5px;\n"
"};"));

        horizontalLayout_5->addWidget(confirmPassword);

        horizontalSpacer_6 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_6);


        verticalLayout->addWidget(widget_2);

        widget = new QWidget(Regist);
        widget->setObjectName("widget");
        horizontalLayout_4 = new QHBoxLayout(widget);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        btnRegist = new QPushButton(widget);
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

        horizontalLayout_4->addWidget(btnRegist);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_8);


        verticalLayout->addWidget(widget);


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
