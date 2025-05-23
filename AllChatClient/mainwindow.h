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
#include <QTimer>
#include <QScrollBar>
#include <QJsonObject>
#include <QJsonDocument>
#include <QButtonGroup>
#include "Model/friendsmodel.h"
#include "View/addfriends.h"
#include "Core/datatransfer.h"
#include <View/updateavatar.h>
#include <Model/chatmodel.h>
#include <Delegate/chatdelegate.h>
#include <Core/currentuser.h>
#include <Core/chathistorymanager.h>

#include <Model/strangermodel.h>
#include <Model/user.h>

#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(ChatHistoryManager *historyManager, QWidget *parent = nullptr);
    ~MainWindow();

    void send_slelectByName(const QString &username);

    void initAddFriends();
    void initHistoryManager();
    void initSideBar();
    void initFriendsList();
    void initChatList();
    void initFriendApplyList();

    void showAvatar(const QString &path);//绘制侧边栏的头像
    QPixmap getRoundPix(const QString &path);

    void handle_message(QDataStream &in);
    void receiveImage(QDataStream &in);

    void updateUserList(const QMap<QString, QString> &newUserList ,const QMap<QString,QByteArray> &new_idAvatar); //更新用户列表
    void handle_userList(QDataStream &in);//接收好友数据
    void handle_onlineFriendsList(QDataStream &in);//接收在线用户列表

    void storeMessageToFile(const Message &msg);//将聊天记录存在文件中
    void loadChatHistoryFromFile(QString targetId);//从文件中加载聊天记录

    // void addOlderMessage_toList(const Message &message);
    void addMessage_toList(const Message &message);
    // void showMessage_toList(const Message &message);

    void handle_addFriend(QDataStream &in);//处理添加好友请求
    void send_addFriend_result(QString id);//发送处理后的好友请求
    void handle_addFriend_result(QDataStream &in);//处理对方返回的请求结果

    void handle_selectByName_reuslt(QDataStream &in);//处理返回的用户名模糊查询的用户列表
    void handle_deleteFriend_result(QDataStream &in);
    // 清除聊天消息和聊天对象信息
    void resetChatState();

    QString getCurrentTime();

    // 发送消息
    void onSendClicked();
    // 发送图片
    void sendImage();
    void sendFile();

    void handleData(QByteArray data);
    void send_updateAvatar(const QString &path);

    void setAvatar(const QString &path);

    void switch_chatUser(const QString &id);

    // 群聊
    void handle_createGroup(QDataStream &in);
    void handle_GroupList(QDataStream &in);
    void handle_groupChat(QDataStream &in);
    void handle_strangerList(QDataStream &in);

    void handle_privateFile(QDataStream &in);

    void initTray();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void showSysMsg(const Message &msg);

protected:
    void showEvent(QShowEvent* event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

signals:
    void updateStrangerList(QMap<QString,QString> id_name,QMap<QString,QString> id_avatar);

private:
    Ui::MainWindow *ui;

    CurrentUser *m_user;
    QButtonGroup *m_sideBar_btnGroup;//管理侧边栏按钮
    UpdateAvatar *m_updateAvatar;
    AddFriends m_add_friends;//添加好友窗口

    ChatHistoryManager *m_historyManager;
    DataTransfer *m_dataTransfer;
    // CommonEnum::message_type messageType;
    QMap<QString, User> &m_friendList; // 用于存储好友列表 id和userName
    QMap<QString, User> &m_strangerList;
    QSet<QString> &m_groupIds;
    QMap<QString, Group> &m_groupList;

    FriendsModel *m_friends_model;//存储好友数据
    ChatModel *m_chat_model;//聊天对象数据
    StrangerModel *m_apply_model;//存储申请添加好友的用户数据

    QSystemTrayIcon *m_trayIcon;   // 托盘图标
    QMenu *m_trayMenu;             // 托盘菜单
};

#endif // MAINWINDOW_H
