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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void ConnectServer();    // 连接服务器
public slots:
    void registerUser(const QString &username, const QString &password);
    void loginUser(const QString &username, const QString &password);
    void send_slelectByName(const QString &username);
private slots:

    void onSendClicked();       // 发送消息
    void onReadyRead();         // 处理服务器返回的数据
    void onDisconnected();      // 处理断开连接
    void sendImage(); //发送图片

    void loadChatHistoryFromFile(QString targetId);//从文件中加载聊天记录
    void handleData(QByteArray data);
    void send_updateAvatar(const QString &path);
    void setAvatar(const QString &path);
private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;//可以换成QSslSocket来加密连接

    QString m_avatarPath;
    QString m_userName;
    QString m_userId;
    QButtonGroup *m_sideBar_btnGroup;//管理侧边栏按钮
    UpdateAvatar *m_updateAvatar;

    void initSideBar();
    void showAvatar(const QString &path);//绘制侧边栏的头像

    DataTransfer *m_dataTransfer;
    CommonEnum::message_type messageType;

    void handle_message(QDataStream &in);
    void receiveImage(QDataStream &in);
    struct userInfo{
        QString userName = "";
        bool state = false;
        QString avatarPath = "";
    };
    QMap<QString, userInfo> m_friendList; // 用于存储好友列表 id和userName
    void updateUserList(const QMap<QString, QString> &newUserList ,const QMap<QString,QByteArray> &new_idAvatar); //更新用户列表
    void handle_userList(QDataStream &in);//接收好友数据
    void handle_onlineFriendsList(QDataStream &in);//接收在线用户列表
    void handle_userInfo(QDataStream &in);//接收用户信息
    QString getChatHistoryFilePath();//获取聊天记录在文件中的路径
    void storeMessageToFile(const QString &targetId, const QString &sender, const QString &message, const QString &msgTime);//将聊天记录存在文件中
    QString storeImageToFile(const QString &targetId, const QString &sender, const QByteArray &imageData, const QString &msgTime);
    QString storeImage(QString imageName,const QByteArray &imageData);
    // QString getLastMessage(const QString &targetId);
    //todo 消息的时间应该服务器传过来，不然有些用户是登录之后才接收到消息就设定的不一样了
    //todo 好友请求没有保存记录，下线之后就看不到了
    QPair<QString,QString> getLastMessage(const QString &targetId);

    void addMessage_toList(const QString &text,const QString &chatId,const QString &senderId,const QString &time);//添加消息到聊天界面
    void addImage_toList(const QString &imagePath,const QString &chatId,const QString &senderId,const QString &time);//添加图片到聊天界面
    void addTime_toList(const QString &chatId,const QString &time);//添加时间到聊天界面

    MessageModel *message_model;//存储消息数据
    MessageDelegate *message_delegate;//绘制消息
    void initMessageList();

    FriendsModel *friends_model;//存储好友数据
    FriendsDelegate *friends_delegate;//绘制好友项
    void initFriendsList();

    ChatModel *chat_model;
    ChatDelegate *chat_delegate;
    void initChatList();

    FriendsModel *friendsApply_model;//存储申请添加好友的用户数据
    ApplyDelegate *friendsApply_delegate;//绘制申请添加好友的用户
    void initFriendApplyList();

    template <typename... Args>//c++17模板参数包允许函数接受任意数量的参数
    QByteArray getPacket(Args... args);//把发送数据的重复语句封装，可以传入任意数量的变量
    void sendData(QByteArray &packet);

    AddFriends add_friends;//添加好友窗口
    void initAddFriends();
    void handle_addFriend(QDataStream &in);//处理添加好友请求
    void send_addFriend_result(QString id);//发送处理后的好友请求
    void handle_addFriend_result(QDataStream &in);//处理对方返回的请求结果

    void handle_selectByName_reuslt(QDataStream &in);//处理返回的用户名模糊查询的用户列表

    QString getCurrentTime();
    bool compareTime(const QString &pastTime,const QString &lastTime);

    //内存和外存都要维护好友列表，每次启动时从外存加载，每次更新时写入外存
signals:
    void loginResult(CommonEnum::message_type result);
    void registResult(CommonEnum::message_type result);
};

#endif // MAINWINDOW_H
