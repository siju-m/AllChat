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
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "View/UserDetailView.h"
#include "View/customtitlebar.h"
#include "View/onlinestatelabel.h"
#include "View\messagelistview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    CustomTitleBar *titleBar;
    QHBoxLayout *horizontalLayout_4;
    QWidget *sideBar;
    QVBoxLayout *verticalLayout_6;
    QSpacerItem *verticalSpacer_3;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *avatar;
    QSpacerItem *verticalSpacer_2;
    QWidget *widget_6;
    QHBoxLayout *horizontalLayout_6;
    QToolButton *switchMessageList;
    QWidget *widget_7;
    QHBoxLayout *horizontalLayout_7;
    QToolButton *switchFriendsList;
    QWidget *widget_8;
    QHBoxLayout *horizontalLayout_8;
    QToolButton *switchaddList;
    QSpacerItem *verticalSpacer;
    QStackedWidget *stackedList;
    QWidget *page;
    QVBoxLayout *verticalLayout_7;
    QListView *chatList;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_3;
    QListView *friendList;
    QWidget *page_4;
    QVBoxLayout *verticalLayout_5;
    QListView *friendApplyList;
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *chatView;
    QVBoxLayout *verticalLayout_8;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout;
    QLabel *chatPartner;
    OnlineStateLabel *friendState;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btnUpdateAvatar;
    QPushButton *addFriends;
    MessageListView *messageList;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_4;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnImage;
    QSpacerItem *horizontalSpacer;
    QPlainTextEdit *lineEditMessage;
    QWidget *widget_9;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnSend;
    QWidget *page_5;
    QVBoxLayout *verticalLayout_9;
    UserDetailView *friendInfo;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        MainWindow->setMinimumSize(QSize(400, 300));
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(241, 241, 241)"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(1, 1, 1, 1);
        titleBar = new CustomTitleBar(centralwidget);
        titleBar->setObjectName("titleBar");
        titleBar->setMinimumSize(QSize(0, 30));
        titleBar->setMaximumSize(QSize(16777215, 30));

        verticalLayout_2->addWidget(titleBar);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        sideBar = new QWidget(centralwidget);
        sideBar->setObjectName("sideBar");
        sideBar->setMinimumSize(QSize(50, 0));
        sideBar->setMaximumSize(QSize(50, 16777215));
        sideBar->setStyleSheet(QString::fromUtf8("/* \351\273\230\350\256\244 */\n"
"QWidget#sideBar{\n"
"	border-right: 1px solid gray;\n"
"}\n"
"QToolButton{   \n"
"	border-top: 3px outset transparent;           /* \344\270\212\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\351\200\217\346\230\216 */\n"
"	border-bottom: 7px outset transparent;\n"
"	border-right: 3px outset transparent;\n"
"	border-left: 3px outset transparent;\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\346\202\254\345\201\234 */\n"
"QToolButton:hover{\n"
"	background-color: rgb(205, 205, 205);\n"
"}\n"
"\n"
"/* \347\202\271\345\207\273\345\222\214\346\214\211\344\270\213 */\n"
"QToolButton:pressed,QToolButton:checked{\n"
"	border-left: 3px outset rgb(93, 95, 97);  /* \345\267\246\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\346"
                        "\267\261\346\267\261\347\201\260 */\n"
"	background-color: rgb(228, 228, 228);	\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}\n"
""));
        verticalLayout_6 = new QVBoxLayout(sideBar);
        verticalLayout_6->setObjectName("verticalLayout_6");
        verticalLayout_6->setContentsMargins(2, 2, 2, 2);
        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_6->addItem(verticalSpacer_3);

        widget_5 = new QWidget(sideBar);
        widget_5->setObjectName("widget_5");
        widget_5->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_5 = new QHBoxLayout(widget_5);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        avatar = new QLabel(widget_5);
        avatar->setObjectName("avatar");
        avatar->setMinimumSize(QSize(40, 40));
        avatar->setMaximumSize(QSize(40, 40));
        avatar->setStyleSheet(QString::fromUtf8("border-radius: 20px; /* \345\234\206\345\275\242\346\225\210\346\236\234\357\274\214\351\234\200\344\270\216 QLabel \347\232\204\345\256\275\345\272\246\344\270\200\350\207\264 */\n"
"border: 2px solid #cccccc; /* \345\217\257\351\200\211\357\274\214\346\267\273\345\212\240\350\276\271\346\241\206 */\n"
"background-color: #cccccc;"));
        avatar->setScaledContents(true);
        avatar->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(avatar);


        verticalLayout_6->addWidget(widget_5);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_6->addItem(verticalSpacer_2);

        widget_6 = new QWidget(sideBar);
        widget_6->setObjectName("widget_6");
        horizontalLayout_6 = new QHBoxLayout(widget_6);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        switchMessageList = new QToolButton(widget_6);
        switchMessageList->setObjectName("switchMessageList");
        switchMessageList->setMinimumSize(QSize(45, 40));
        switchMessageList->setMaximumSize(QSize(45, 40));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icon/comment.png"), QSize(), QIcon::Normal, QIcon::On);
        switchMessageList->setIcon(icon);
        switchMessageList->setIconSize(QSize(30, 30));
        switchMessageList->setCheckable(true);
        switchMessageList->setAutoExclusive(true);

        horizontalLayout_6->addWidget(switchMessageList);


        verticalLayout_6->addWidget(widget_6);

        widget_7 = new QWidget(sideBar);
        widget_7->setObjectName("widget_7");
        horizontalLayout_7 = new QHBoxLayout(widget_7);
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        switchFriendsList = new QToolButton(widget_7);
        switchFriendsList->setObjectName("switchFriendsList");
        switchFriendsList->setMinimumSize(QSize(45, 40));
        switchFriendsList->setMaximumSize(QSize(45, 40));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Icon/users.png"), QSize(), QIcon::Normal, QIcon::On);
        switchFriendsList->setIcon(icon1);
        switchFriendsList->setIconSize(QSize(30, 30));
        switchFriendsList->setCheckable(true);
        switchFriendsList->setAutoExclusive(true);

        horizontalLayout_7->addWidget(switchFriendsList);


        verticalLayout_6->addWidget(widget_7);

        widget_8 = new QWidget(sideBar);
        widget_8->setObjectName("widget_8");
        horizontalLayout_8 = new QHBoxLayout(widget_8);
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        switchaddList = new QToolButton(widget_8);
        switchaddList->setObjectName("switchaddList");
        switchaddList->setMinimumSize(QSize(45, 40));
        switchaddList->setMaximumSize(QSize(45, 40));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Icon/user-add.png"), QSize(), QIcon::Normal, QIcon::On);
        switchaddList->setIcon(icon2);
        switchaddList->setIconSize(QSize(30, 30));
        switchaddList->setCheckable(true);
        switchaddList->setAutoExclusive(true);

        horizontalLayout_8->addWidget(switchaddList);


        verticalLayout_6->addWidget(widget_8);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer);


        horizontalLayout_4->addWidget(sideBar);

        stackedList = new QStackedWidget(centralwidget);
        stackedList->setObjectName("stackedList");
        stackedList->setMinimumSize(QSize(200, 0));
        stackedList->setMaximumSize(QSize(200, 16777215));
        stackedList->setStyleSheet(QString::fromUtf8("QScrollBar:vertical {\n"
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
        page = new QWidget();
        page->setObjectName("page");
        verticalLayout_7 = new QVBoxLayout(page);
        verticalLayout_7->setObjectName("verticalLayout_7");
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        chatList = new QListView(page);
        chatList->setObjectName("chatList");
        chatList->setStyleSheet(QString::fromUtf8("QListView { \n"
"border: none;\n"
"border-right: 1px solid gray;  /*\345\217\252\344\277\235\347\225\231\345\217\263\350\276\271\350\276\271\346\241\206 */\n"
"}"));

        verticalLayout_7->addWidget(chatList);

        stackedList->addWidget(page);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        verticalLayout_3 = new QVBoxLayout(page_3);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        friendList = new QListView(page_3);
        friendList->setObjectName("friendList");
        friendList->setStyleSheet(QString::fromUtf8("QListView { \n"
"border: none;\n"
"border-right: 1px solid gray;  /*\345\217\252\344\277\235\347\225\231\345\217\263\350\276\271\350\276\271\346\241\206 */\n"
"}"));

        verticalLayout_3->addWidget(friendList);

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
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        chatView = new QWidget();
        chatView->setObjectName("chatView");
        verticalLayout_8 = new QVBoxLayout(chatView);
        verticalLayout_8->setObjectName("verticalLayout_8");
        verticalLayout_8->setContentsMargins(0, 0, 0, 0);
        widget_4 = new QWidget(chatView);
        widget_4->setObjectName("widget_4");
        widget_4->setMinimumSize(QSize(0, 20));
        horizontalLayout = new QHBoxLayout(widget_4);
        horizontalLayout->setObjectName("horizontalLayout");
        chatPartner = new QLabel(widget_4);
        chatPartner->setObjectName("chatPartner");

        horizontalLayout->addWidget(chatPartner);

        friendState = new OnlineStateLabel(widget_4);
        friendState->setObjectName("friendState");
        friendState->setMinimumSize(QSize(30, 30));
        friendState->setMaximumSize(QSize(30, 30));

        horizontalLayout->addWidget(friendState);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        btnUpdateAvatar = new QPushButton(widget_4);
        btnUpdateAvatar->setObjectName("btnUpdateAvatar");
        btnUpdateAvatar->setStyleSheet(QString::fromUtf8("/**\346\255\243\345\270\270\346\203\205\345\206\265\344\270\213\346\240\267\345\274\217**/\n"
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

        horizontalLayout->addWidget(btnUpdateAvatar);

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
        addFriends->setIcon(icon2);

        horizontalLayout->addWidget(addFriends);


        verticalLayout_8->addWidget(widget_4);

        messageList = new MessageListView(chatView);
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

        verticalLayout_8->addWidget(messageList);

        widget_3 = new QWidget(chatView);
        widget_3->setObjectName("widget_3");
        widget_3->setMinimumSize(QSize(0, 50));
        widget_3->setMaximumSize(QSize(16777215, 150));
        verticalLayout_4 = new QVBoxLayout(widget_3);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        widget_2 = new QWidget(widget_3);
        widget_2->setObjectName("widget_2");
        widget_2->setMinimumSize(QSize(0, 30));
        widget_2->setMaximumSize(QSize(16777215, 20));
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(10, 0, 0, 0);
        btnImage = new QPushButton(widget_2);
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


        verticalLayout_4->addWidget(widget_2);

        lineEditMessage = new QPlainTextEdit(widget_3);
        lineEditMessage->setObjectName("lineEditMessage");
        lineEditMessage->setMaximumSize(QSize(16777215, 16777215));
        lineEditMessage->setStyleSheet(QString::fromUtf8("QPlainTextEdit{\n"
"background-color: rgb(241, 241, 241);\n"
"border:none;\n"
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

        verticalLayout_4->addWidget(lineEditMessage);

        widget_9 = new QWidget(widget_3);
        widget_9->setObjectName("widget_9");
        widget_9->setMinimumSize(QSize(0, 30));
        horizontalLayout_3 = new QHBoxLayout(widget_9);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 10, 0);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        btnSend = new QPushButton(widget_9);
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


        verticalLayout_4->addWidget(widget_9);


        verticalLayout_8->addWidget(widget_3);

        stackedWidget->addWidget(chatView);
        page_5 = new QWidget();
        page_5->setObjectName("page_5");
        verticalLayout_9 = new QVBoxLayout(page_5);
        verticalLayout_9->setObjectName("verticalLayout_9");
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);
        friendInfo = new UserDetailView(page_5);
        friendInfo->setObjectName("friendInfo");

        verticalLayout_9->addWidget(friendInfo);

        stackedWidget->addWidget(page_5);

        verticalLayout->addWidget(stackedWidget);


        horizontalLayout_4->addLayout(verticalLayout);

        horizontalLayout_4->setStretch(2, 4);

        verticalLayout_2->addLayout(horizontalLayout_4);

        MainWindow->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 800, 17));
        MainWindow->setMenuBar(menuBar);

        retranslateUi(MainWindow);

        stackedList->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        avatar->setText(QString());
#if QT_CONFIG(tooltip)
        switchMessageList->setToolTip(QCoreApplication::translate("MainWindow", "\346\266\210\346\201\257", nullptr));
#endif // QT_CONFIG(tooltip)
        switchMessageList->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
#if QT_CONFIG(tooltip)
        switchFriendsList->setToolTip(QCoreApplication::translate("MainWindow", "\345\245\275\345\217\213", nullptr));
#endif // QT_CONFIG(tooltip)
        switchFriendsList->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
#if QT_CONFIG(tooltip)
        switchaddList->setToolTip(QCoreApplication::translate("MainWindow", "\347\224\263\350\257\267", nullptr));
#endif // QT_CONFIG(tooltip)
        switchaddList->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        chatPartner->setText(QString());
        friendState->setText(QString());
        btnUpdateAvatar->setText(QCoreApplication::translate("MainWindow", "\344\277\256\346\224\271\345\244\264\345\203\217", nullptr));
        addFriends->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\345\245\275\345\217\213", nullptr));
        btnImage->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201\345\233\276\347\211\207", nullptr));
        btnSend->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
