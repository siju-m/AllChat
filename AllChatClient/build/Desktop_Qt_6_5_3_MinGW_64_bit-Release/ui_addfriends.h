/********************************************************************************
** Form generated from reading UI file 'addfriends.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDFRIENDS_H
#define UI_ADDFRIENDS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AddFriends
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLineEdit *nameInputEdit;
    QPushButton *btnSelect;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QListView *strangerList;

    void setupUi(QDialog *AddFriends)
    {
        if (AddFriends->objectName().isEmpty())
            AddFriends->setObjectName("AddFriends");
        AddFriends->resize(400, 300);
        AddFriends->setStyleSheet(QString::fromUtf8("background-color: rgb(241, 241, 241)"));
        verticalLayout_2 = new QVBoxLayout(AddFriends);
        verticalLayout_2->setObjectName("verticalLayout_2");
        widget = new QWidget(AddFriends);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(-1, -1, -1, 0);
        nameInputEdit = new QLineEdit(widget);
        nameInputEdit->setObjectName("nameInputEdit");

        horizontalLayout->addWidget(nameInputEdit);

        btnSelect = new QPushButton(widget);
        btnSelect->setObjectName("btnSelect");
        btnSelect->setStyleSheet(QString::fromUtf8("/**\346\255\243\345\270\270\346\203\205\345\206\265\344\270\213\346\240\267\345\274\217**/\n"
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

        horizontalLayout->addWidget(btnSelect);


        verticalLayout_2->addWidget(widget);

        widget_2 = new QWidget(AddFriends);
        widget_2->setObjectName("widget_2");
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setObjectName("verticalLayout");
        strangerList = new QListView(widget_2);
        strangerList->setObjectName("strangerList");
        strangerList->setStyleSheet(QString::fromUtf8("QListView {\n"
"    border: none;\n"
"}"));

        verticalLayout->addWidget(strangerList);


        verticalLayout_2->addWidget(widget_2);


        retranslateUi(AddFriends);

        QMetaObject::connectSlotsByName(AddFriends);
    } // setupUi

    void retranslateUi(QDialog *AddFriends)
    {
        AddFriends->setWindowTitle(QCoreApplication::translate("AddFriends", "Dialog", nullptr));
        btnSelect->setText(QCoreApplication::translate("AddFriends", "\346\237\245\350\257\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddFriends: public Ui_AddFriends {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDFRIENDS_H
