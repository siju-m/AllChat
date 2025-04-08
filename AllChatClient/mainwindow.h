#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QMessageBox>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QStandardPaths>
#include <QLabel>
#include <QBuffer>
#include "Model/messagemodel.h"
#include <QTimer>
#include <QScrollBar>
#include <QJsonObject>
#include <QJsonDocument>
#include <QButtonGroup>
#include "Delegate/messagedelegate.h"
#include "Utils/CustomTypes.h"
#include "Delegate/friendsdelegate.h"
#include "Model/friendsmodel.h"
#include "Delegate/applydelegate.h"
#include "View/addfriends.h"
#include "Core/datatransfer.h"
#include <View/updateavatar.h>
#include <Model/chatmodel.h>
#include <Delegate/chatdelegate.h>
#include <Core/currentuser.h>
#include <Core/chathistorymanager.h>

#include <Model/strangermodel.h>
#include <Model/user.h>
#include "Model/message.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void registerUser(const QString &username, const QString &password);
    void loginUser(const QString &username, const QString &password);
    void send_slelectByName(const QString &username);

    void initAddFriends();
    void initHistoryManager();
    void initSideBar();
    // void initMessageList();
    void initFriendsList();
    void initChatList();
    void initFriendApplyList();

    void showAvatar(const QString &path);//绘制侧边栏的头像
    void handle_message(QDataStream &in);
    void receiveImage(QDataStream &in);

    void updateUserList(const QMap<QString, QString> &newUserList ,const QMap<QString,QByteArray> &new_idAvatar); //更新用户列表
    void handle_userList(QDataStream &in);//接收好友数据
    void handle_onlineFriendsList(QDataStream &in);//接收在线用户列表
    void handle_userInfo(QDataStream &in);//接收用户信息

    void storeMessageToFile(const QString &targetId, const User &sender, const QString &message, const QString &msgTime);//将聊天记录存在文件中
    QString storeImageToFile(const QString &targetId, const User &sender, const QByteArray &imageData, const QString &msgTime);
    void loadChatHistoryFromFile(QString targetId);//从文件中加载聊天记录

    void addMessage_toList(const QString &text,const QString &chatId,const QString &senderId,const QString &time);//添加消息到聊天界面
    void addImage_toList(const QString &imagePath,const QString &chatId,const QString &senderId,const QString &time);//添加图片到聊天界面


    void handle_addFriend(QDataStream &in);//处理添加好友请求
    void send_addFriend_result(QString id);//发送处理后的好友请求
    void handle_addFriend_result(QDataStream &in);//处理对方返回的请求结果

    void handle_selectByName_reuslt(QDataStream &in);//处理返回的用户名模糊查询的用户列表
    void handle_deleteFriend_result(QDataStream &in);

    QString getCurrentTime();

    void onSendClicked();       // 发送消息
    void sendImage(); //发送图片

    void handleData(QByteArray data);
    void send_updateAvatar(const QString &path);

    void setAvatar(const QString &path);
signals:
    void loginResult(CommonEnum::message_type result);
    void registResult(CommonEnum::message_type result);
    void updateStrangerList(QMap<QString,QString> id_name,QMap<QString,QString> id_avatar);

private:
    Ui::MainWindow *ui;
    // QTcpSocket *socket;//可以换成QSslSocket来加密连接

    CurrentUser *m_user;
    QButtonGroup *m_sideBar_btnGroup;//管理侧边栏按钮
    UpdateAvatar *m_updateAvatar;

    ChatHistoryManager *m_historyManager;
    DataTransfer *m_dataTransfer;
    CommonEnum::message_type messageType;
    QMap<QString, User> m_friendList; // 用于存储好友列表 id和userName
    FriendsModel *friends_model;//存储好友数据
    ChatModel *chat_model;//聊天对象数据
    StrangerModel *apply_model;//存储申请添加好友的用户数据
    AddFriends add_friends;//添加好友窗口

};

#endif // MAINWINDOW_H
