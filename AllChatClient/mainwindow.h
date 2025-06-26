#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Model/chatmodel.h"
#include "View/Components/searchresultlist.h"
#include "View/Components/sidebar.h"
#include "Model/friendsmodel.h"
#include "View/addfriends.h"
#include "Core/datatransfer.h"
#include "View/creategroup.h"
#include "View/messagesenderview.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
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
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void send_slelectByName(const QString &username);

    void initAddFriends();
    void initHistoryManager();
    void initSideBar();
    void initFriendsList();
    void initChatList();
    void initFriendApplyList();

    // void showAvatar(const QString &path);//绘制侧边栏的头像
    // QPixmap getRoundPix(const QString &path);

    void handle_message(QDataStream &in);
    void receiveImage(QDataStream &in);

    // void updateUserList(const QMap<QString, QString> &newUserList ,const QMap<QString,QByteArray> &new_idAvatar); //更新用户列表
    void onClearModel();
    void handleUserList(QDataStream &in);//接收好友数据
    void handleFrdOnlineList(QDataStream &in);//接收在线用户列表
    void handleFrdOnlineState(QDataStream &in);

    void storeMessageToFile(const Message &msg);//将聊天记录存在文件中
    void loadChatHistoryFromFile(QString targetId);//从文件中加载聊天记录

    void addMessage_toList(const Message &message);

    void handle_addFriend(QDataStream &in);//处理添加好友请求
    void onAddFriendReply(const QString &id,const int &row);//同意好友请求
    void handle_addFriend_result(QDataStream &in);//处理对方返回的请求结果

    void handle_selectByName_reuslt(QDataStream &in);//处理返回的用户名模糊查询的用户列表
    void handle_deleteFriend_result(QDataStream &in);
    // 清除聊天消息和聊天对象信息
    void resetChatState();

    void handleData(QByteArray data);
    // void send_updateAvatar(const QString &path);

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

    //好友申请红点
    void showApplyRedDot();
    void hideApplyRedDot();

    void onSearchFrd(const QString &name);
    void onSwitchView(int index);

protected:
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent *event) override;

signals:
    void updateStrangerList(QMap<QString,QString> id_name,QMap<QString,QString> id_avatar);

private:
    Ui::MainWindow *ui;

    CurrentUser *m_user;
    // QButtonGroup *m_sideBar_btnGroup;//管理侧边栏按钮
    // UpdateAvatar *m_updateAvatar;
    AddFriends m_add_friends;//添加好友窗口

    ChatHistoryManager *m_historyManager;
    DataTransfer *m_dataTransfer;
    ContactManager *m_contact_manager;

    FriendsModel *m_friends_model;//存储好友数据
    ChatModel *m_chat_model;//聊天对象数据
    StrangerModel *m_apply_model;//存储申请添加好友的用户数据

    QSystemTrayIcon *m_trayIcon;   // 托盘图标
    QMenu *m_trayMenu;             // 托盘菜单
    MessageSenderView *m_msgSenderView;

    SearchResultList *m_search_result_list;
    SideBar *m_side_bar;
    CreateGroup *m_create_group;
};

#endif // MAINWINDOW_H
