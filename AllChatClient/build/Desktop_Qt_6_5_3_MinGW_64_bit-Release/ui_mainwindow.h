/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "messagelistview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *friendsGroup;
    QAction *messageGroup;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QStackedWidget *stackedList;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_3;
    QListView *friendsList;
    QWidget *page_4;
    QVBoxLayout *verticalLayout_5;
    QListView *friendApplyList;
    QVBoxLayout *verticalLayout;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout;
    QLabel *chatPartner;
    QLabel *friendState;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *addFriends;
    MessageListView *messageList;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_4;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnImage;
    QSpacerItem *horizontalSpacer;
    QPlainTextEdit *lineEditMessage;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnSend;
    QStatusBar *statusBar;
    QToolBar *toolBar;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        MainWindow->setMinimumSize(QSize(800, 600));
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(241, 241, 241)"));
        friendsGroup = new QAction(MainWindow);
        friendsGroup->setObjectName("friendsGroup");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icon/user-add.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/Icon/user-add.png"), QSize(), QIcon::Normal, QIcon::On);
        friendsGroup->setIcon(icon);
        friendsGroup->setMenuRole(QAction::NoRole);
        messageGroup = new QAction(MainWindow);
        messageGroup->setObjectName("messageGroup");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Icon/comment.png"), QSize(), QIcon::Normal, QIcon::Off);
        messageGroup->setIcon(icon1);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        stackedList = new QStackedWidget(centralwidget);
        stackedList->setObjectName("stackedList");
        stackedList->setMinimumSize(QSize(200, 0));
        stackedList->setMaximumSize(QSize(200, 16777215));
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        verticalLayout_3 = new QVBoxLayout(page_3);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        friendsList = new QListView(page_3);
        friendsList->setObjectName("friendsList");
        friendsList->setStyleSheet(QString::fromUtf8("QListView { \n"
"border: none;\n"
"border-right: 1px solid gray;  /*\345\217\252\344\277\235\347\225\231\345\217\263\350\276\271\350\276\271\346\241\206 */\n"
"}"));

        verticalLayout_3->addWidget(friendsList);

        stackedList->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName("page_4");
        verticalLayout_5 = new QVBoxLayout(page_4);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        friendApplyList = new QListView(page_4);
        friendApplyList->setObjectName("friendApplyList");
        friendApplyList->setMaximumSize(QSize(200, 16777215));
        friendApplyList->setStyleSheet(QString::fromUtf8("QListView { \n"
"border: none;\n"
"border-right: 1px solid gray;  /*\345\217\252\344\277\235\347\225\231\345\217\263\350\276\271\350\276\271\346\241\206 */\n"
"}"));

        verticalLayout_5->addWidget(friendApplyList);

        stackedList->addWidget(page_4);

        horizontalLayout_4->addWidget(stackedList);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        widget_4 = new QWidget(centralwidget);
        widget_4->setObjectName("widget_4");
        widget_4->setMinimumSize(QSize(0, 20));
        horizontalLayout = new QHBoxLayout(widget_4);
        horizontalLayout->setObjectName("horizontalLayout");
        chatPartner = new QLabel(widget_4);
        chatPartner->setObjectName("chatPartner");

        horizontalLayout->addWidget(chatPartner);

        friendState = new QLabel(widget_4);
        friendState->setObjectName("friendState");

        horizontalLayout->addWidget(friendState);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        addFriends = new QPushButton(widget_4);
        addFriends->setObjectName("addFriends");
        addFriends->setStyleSheet(QString::fromUtf8("/**\346\255\243\345\270\270\346\203\205\345\206\265\344\270\213\346\240\267\345\274\217**/\n"
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
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Icon/user-add.png"), QSize(), QIcon::Normal, QIcon::On);
        addFriends->setIcon(icon2);

        horizontalLayout->addWidget(addFriends);


        verticalLayout->addWidget(widget_4);

        messageList = new MessageListView(centralwidget);
        messageList->setObjectName("messageList");
        messageList->setMinimumSize(QSize(0, 250));
        messageList->setStyleSheet(QString::fromUtf8("\n"
"\n"
"QListView {\n"
"    border: none;\n"
"	background-color: rgb(241, 241, 241);\n"
"	border-bottom : 1px solid gray; /* \345\217\252\344\277\235\347\225\231\344\270\212\344\270\213\350\276\271\346\241\206 */\n"
"	border-top: 1px solid gray;\n"
"}\n"
"QScrollBar:vertical {\n"
"    border: none;\n"
"    background: #f0f0f0;\n"
"    width: 10px;\n"
"    margin: 0px 0px 0px 0px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"    background: #888;\n"
"    min-height: 20px;\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical:hover {\n"
"    background: #666;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical:pressed {\n"
"    background: #444;\n"
"}\n"
"\n"
"QScrollBar::add-line:vertical, \n"
"QScrollBar::sub-line:vertical {\n"
"    background: none;\n"
"    border: none;\n"
"}\n"
"\n"
"QScrollBar::add-page:vertical, \n"
"QScrollBar::sub-page:vertical {\n"
"    background: none;\n"
"}\n"
""));

        verticalLayout->addWidget(messageList);

        widget_3 = new QWidget(centralwidget);
        widget_3->setObjectName("widget_3");
        widget_3->setMinimumSize(QSize(0, 50));
        widget_3->setMaximumSize(QSize(16777215, 150));
        verticalLayout_4 = new QVBoxLayout(widget_3);
        verticalLayout_4->setObjectName("verticalLayout_4");
        widget = new QWidget(widget_3);
        widget->setObjectName("widget");
        widget->setMinimumSize(QSize(0, 30));
        widget->setMaximumSize(QSize(16777215, 20));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        btnImage = new QPushButton(widget);
        btnImage->setObjectName("btnImage");
        btnImage->setMaximumSize(QSize(60, 16777215));
        btnImage->setStyleSheet(QString::fromUtf8("/**\346\255\243\345\270\270\346\203\205\345\206\265\344\270\213\346\240\267\345\274\217**/\n"
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

        horizontalLayout_2->addWidget(btnImage);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_4->addWidget(widget);

        lineEditMessage = new QPlainTextEdit(widget_3);
        lineEditMessage->setObjectName("lineEditMessage");
        lineEditMessage->setMaximumSize(QSize(16777215, 16777215));
        lineEditMessage->setStyleSheet(QString::fromUtf8("background-color: rgb(241, 241, 241);\n"
"border:none;\n"
""));

        verticalLayout_4->addWidget(lineEditMessage);

        widget_2 = new QWidget(widget_3);
        widget_2->setObjectName("widget_2");
        widget_2->setMinimumSize(QSize(0, 30));
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        btnSend = new QPushButton(widget_2);
        btnSend->setObjectName("btnSend");
        btnSend->setMinimumSize(QSize(0, 0));
        btnSend->setMaximumSize(QSize(40, 16777215));
        btnSend->setSizeIncrement(QSize(0, 0));
        btnSend->setStyleSheet(QString::fromUtf8("/**\346\255\243\345\270\270\346\203\205\345\206\265\344\270\213\346\240\267\345\274\217**/\n"
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

        horizontalLayout_3->addWidget(btnSend);


        verticalLayout_4->addWidget(widget_2);


        verticalLayout->addWidget(widget_3);


        horizontalLayout_4->addLayout(verticalLayout);

        horizontalLayout_4->setStretch(1, 4);

        verticalLayout_2->addLayout(horizontalLayout_4);

        MainWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        toolBar->setStyleSheet(QString::fromUtf8(""));
        MainWindow->addToolBar(Qt::LeftToolBarArea, toolBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 800, 17));
        MainWindow->setMenuBar(menuBar);

        toolBar->addAction(messageGroup);
        toolBar->addAction(friendsGroup);

        retranslateUi(MainWindow);

        stackedList->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        friendsGroup->setText(QCoreApplication::translate("MainWindow", "\345\245\275\345\217\213", nullptr));
        messageGroup->setText(QCoreApplication::translate("MainWindow", "\346\266\210\346\201\257", nullptr));
        chatPartner->setText(QString());
        friendState->setText(QString());
        addFriends->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\345\245\275\345\217\213", nullptr));
        btnImage->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201\345\233\276\347\211\207", nullptr));
        btnSend->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
