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
#include "messagemodel.h"
#include <QTimer>
#include <QScrollBar>
#include <QJsonObject>
#include <QJsonDocument>
#include "messagedelegate.h"
#include "imageviewer.h"
#include "CommonEnum.h"
#include "friendsdelegate.h"
#include "friendsmodel.h"
#include "applydelegate.h"
#include "addfriends.h"
#include "Utils/datatransfer.h"

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
private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;//可以换成QSslSocket来加密连接

    QString m_avatarPath;
    QString m_userName;

    DataTransfer *m_dataTransfer;

    // enum ReceivingState {
    //     WaitingForHeader, // 等待接收数据头
    //     ReceivingData     // 接收数据中
    // };

    CommonEnum::message_type messageType;
    // ReceivingState currentReceivingState = WaitingForHeader; // 当前状态初始化为等待数据头

    void handle_message(QDataStream &in);
    void receiveImage(QDataStream &in);

    // QString messageType;
    // qint32 currentDataLength = 0;    // 数据长度
    // qint32 receivedBytes = 0;        // 已接收字节数
    // QByteArray dataBuffer;           // 用于暂存接收到的数据
    // void resetState();

    struct userInfo{
        QString userName = "";
        bool state = false;
        QString avatarPath = "";
    };
    QMap<QString, userInfo> m_userList; // 用于存储好友列表 id和userName
    void updateUserList(const QMap<QString, QString> &newUserList ,const QMap<QString,QByteArray> &new_idAvatar); //更新用户列表
    void handle_userList(QDataStream &in);//接收好友数据
    void handle_onlineFriendsList(QDataStream &in);//接收在线用户列表
    void handle_userAvatar(QDataStream &in);//接收用户头像
    QString getChatHistoryFilePath();//获取聊天记录在文件中的路径
    void storeMessageToFile(const QString &targetId, const QString &sender, const QString &message);//将聊天记录存在文件中
    QString storeImageToFile(const QString &targetId, const QString &sender, const QByteArray &imageData);
    QString storeImage(QString imageName,const QByteArray &imageData);
    QString getLastMessage(const QString &targetId);

    MessageModel *message_model;//存储消息数据
    MessageDelegate *message_delegate;//绘制消息
    void initMessageList();
    void addMessage_toList(const QString &text,const bool &isOutgoing,const QString &userName,const QString &avatarPath="");//添加消息到聊天界面
    void addImage_toList(const QString &imagePath,const bool &isOutgoing,const QString &userName,const QString &avatarPath="");//添加图片到聊天界面

    FriendsModel *friends_model;//存储好友数据
    FriendsDelegate *friends_delegate;//绘制好友项
    void initFriendsList();

    template <typename... Args>//c++17模板参数包允许函数接受任意数量的参数
    void sendData(Args... args);//把发送数据的重复语句封装，可以传入任意数量的变量

    AddFriends add_friends;//添加好友窗口
    void handle_addFriend(QDataStream &in);//处理添加好友请求
    void send_addFriend_result(QString id);//发送处理后的好友请求
    void handle_addFriend_result(QDataStream &in);//处理对方返回的请求结果

    FriendsModel *friendsApply_model;//存储申请添加好友的用户数据
    ApplyDelegate *friendsApply_delegate;//绘制申请添加好友的用户
    void initFriendApplyList();

    void handle_selectByName_reuslt(QDataStream &in);//处理返回的用户名模糊查询的用户列表

    //内存和外存都要维护好友列表，每次启动时从外存加载，每次更新时写入外存
signals:
    void loginResult(CommonEnum::message_type result);
    void registResult(CommonEnum::message_type result);
};

#endif // MAINWINDOW_H
