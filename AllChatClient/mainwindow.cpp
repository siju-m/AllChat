#include "mainwindow.h"

#include <QImageReader>
#include <QQuickView>
#include <QShortcut>
#include <QShowEvent>
#include <QSqlDatabase>
#include <windows.h>

#include <Model/Packet.h>



MainWindow::MainWindow(DataTransfer *dataTransfer, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    // socket(new QTcpSocket(this)),
    // m_avatarPath(""),
    m_user(CurrentUser::getInstance()),
    m_sideBar_btnGroup(new QButtonGroup(this)),
    m_historyManager(new ChatHistoryManager(this)),
    m_dataTransfer(dataTransfer)
{
    this->resize(1000, 700);
    ui->setupUi(this);
    setWindowTitle("AllChat");
    this->setWindowFlags(Qt::CustomizeWindowHint);//去除标题栏但仍可调整大小

    // 绑定按钮槽函数
    connect(ui->btnSend, &QPushButton::clicked, this, &MainWindow::onSendClicked);
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return), this);
    connect(shortcut, &QShortcut::activated, this, &MainWindow::onSendClicked);
    connect(ui->btnImage, &QPushButton::clicked, this, &MainWindow::sendImage);
    connect(ui->btnUpdateAvatar,&QPushButton::clicked, this,[=](){
        m_updateAvatar = new UpdateAvatar(this);
        connect(m_updateAvatar,&UpdateAvatar::send_updateAvatar,this,&MainWindow::send_updateAvatar);
        connect(m_updateAvatar,&UpdateAvatar::setAvatar,this,&MainWindow::setAvatar);
        // m_updateAvatar->setAvatarPath(m_avatarPath);
        m_updateAvatar->setAvatarPath(m_user->get_avatarPath());
        m_updateAvatar->exec();
    });

    // connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(m_dataTransfer,&DataTransfer::handleData,this,&MainWindow::handleData);
    // connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);

    // 连接标题栏按钮的信号
    connect(ui->titleBar, &CustomTitleBar::minimizeClicked, this, &MainWindow::showMinimized);
    connect(ui->titleBar, &CustomTitleBar::maximizeClicked, this, [this]() {
        isMaximized() ? showNormal() : showMaximized();
    });
    connect(ui->titleBar, &CustomTitleBar::closeClicked, this, &MainWindow::close);

    // initMessageList();
    initFriendsList();
    initChatList();
    initAddFriends();
    initSideBar();
    initFriendApplyList();
    initHistoryManager();

    m_dataTransfer->ConnectServer();
}

MainWindow::~MainWindow() {
    delete ui;
    // delete socket;
}

void MainWindow::initFriendsList()
{
    friends_model = new FriendsModel(this);
    FriendsDelegate *friends_delegate = new FriendsDelegate(this);

    ui->friendList->setModel(friends_model);
    ui->friendList->setItemDelegate(friends_delegate);

    ui->friendList->setVerticalScrollMode(QListView::ScrollPerPixel); // 平滑滚动
    ui->friendList->setResizeMode(QListView::Adjust);                 // 自动调整项大小
    ui->friendList->setSelectionMode(QAbstractItemView::SingleSelection);  // 单选
    ui->friendList->setSelectionBehavior(QAbstractItemView::SelectItems);

    connect(ui->friendList, &QListView::clicked, this, [=](const QModelIndex &index){
        QString id = index.data(FriendsModel::IdRole).toString();
        ui->friendInfo->showUserInfo(id,m_friendList[id]);
    });
    connect(ui->friendInfo,&UserDetailView::showMessage,this,[=](const QString &id){
        m_sideBar_btnGroup->button(0)->click();
        int row = chat_model->get_rowById(id);
        qDebug()<<row;
        QModelIndex index = ui->chatList->model()->index(row, 0);
        emit ui->chatList->clicked(index);
        ui->chatList->setCurrentIndex(index);
    });
    connect(ui->friendInfo,&UserDetailView::deleteFriend,this,[=](const QString &id){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认操作", "你确定要删除好友吗？",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            Packet data(CommonEnum::DELETEFRIEND,id);
            m_dataTransfer->sendData(data);
        }
    });
}

void MainWindow::initChatList()
{
    chat_model = new ChatModel(this);
    ChatDelegate *chat_delegate = new ChatDelegate(this);

    ui->chatList->setModel(chat_model);
    ui->chatList->setItemDelegate(chat_delegate);

    ui->chatList->setVerticalScrollMode(QListView::ScrollPerPixel); // 平滑滚动
    ui->chatList->setResizeMode(QListView::Adjust);                 // 自动调整项大小
    ui->chatList->setSelectionMode(QAbstractItemView::SingleSelection);  // 单选
    ui->chatList->setSelectionBehavior(QAbstractItemView::SelectItems);

    ui->chatList->setUniformItemSizes(true);//启用统一项尺寸优化

    connect(ui->chatList, &QListView::clicked, this, [=](const QModelIndex &index){
        QString id = index.data(FriendsModel::IdRole).toString();
        loadChatHistoryFromFile(id);
        chat_model->set_currentChatId(id);
        chat_model->clear_unreadMsgNum(id);
    });
    connect(chat_model,&ChatModel::sortEnd,this,[=](){
        //将当前聊天对象的选择设为排序后的当前聊天对象
        //QTimer的作用 使得排序完成后的其他事件先处理，再更新选择状态
        QTimer::singleShot(0, this, [=]() {
            if(ui->chatList->model()->rowCount() > 0){
                int row = chat_model->get_currentChatRow();
                QModelIndex index = ui->chatList->model()->index(row, 0);
                if(ui->chatList->currentIndex() == index)
                    return;
                qDebug() << "设定";
                ui->chatList->setCurrentIndex(index);
                QString id = index.data(FriendsModel::IdRole).toString();
                loadChatHistoryFromFile(id);
            }
        });
    });
}

void MainWindow::initAddFriends()
{
    connect(ui->addFriends,&QPushButton::clicked,this,[=](){
        add_friends.exec();
    });
    connect(&add_friends,&AddFriends::send_slelectByName,this,&MainWindow::send_slelectByName);
    connect(&add_friends,&AddFriends::sendData,this,[=](const QString &id){
        // this->sendData(CommonEnum::ADD_FRIEND,id);
        Packet data(CommonEnum::ADD_FRIEND,id);
        m_dataTransfer->sendData(data);
    });
    connect(this,&MainWindow::updateStrangerList,&add_friends,&AddFriends::updateListView);
}

void MainWindow::initSideBar()
{
    m_sideBar_btnGroup->addButton(ui->switchMessageList,0);
    m_sideBar_btnGroup->addButton(ui->switchFriendsList,1);
    m_sideBar_btnGroup->addButton(ui->switchaddList,2);
    connect(m_sideBar_btnGroup,&QButtonGroup::buttonClicked,this,[=](QAbstractButton *button){
        int index = m_sideBar_btnGroup->id(button);
        ui->stackedList->setCurrentIndex(index);
        if (index >= 0 && index < ui->stackedWidget->count())
            ui->stackedWidget->setCurrentIndex(index);
    });

    ui->stackedList->setCurrentIndex(0);//默认显示聊天列表
    m_sideBar_btnGroup->button(0)->setChecked(true);
}

void MainWindow::initFriendApplyList()
{
    apply_model = new StrangerModel(this);
    ApplyDelegate *apply_delegate = new ApplyDelegate(this);
    ui->friendApplyList->setModel(apply_model);
    ui->friendApplyList->setItemDelegate(apply_delegate);

    connect(apply_delegate,&ApplyDelegate::applyResult,this,[=](const QString &id,const int &row){
        send_addFriend_result(id);
        apply_model->removeItem(row);
    });
}

void MainWindow::initHistoryManager()
{
    connect(m_historyManager,&ChatHistoryManager::addImage_toList,this,&MainWindow::addImage_toList);
    connect(m_historyManager,&ChatHistoryManager::addMessage_toList,this,&MainWindow::addMessage_toList);
}

void MainWindow::showAvatar(const QString &path)
{
    QPixmap pixmap(path);
    if(pixmap.isNull()){
        return;
    }
    QPixmap roundedPixmap(pixmap.size());
    roundedPixmap.fill(Qt::transparent);

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPainterPath painterPath;
    painterPath.addEllipse(0, 0, pixmap.width(), pixmap.height());
    painter.setClipPath(painterPath);
    painter.drawPixmap(0, 0, pixmap);

    ui->avatar->setPixmap(roundedPixmap);
}

void MainWindow::handle_selectByName_reuslt(QDataStream &in)
{
    QMap<QString,QString> id_names;
    QMap<QString,QByteArray> id_avatar;
    QMap<QString,QString> id_avatarPath;
    in>>id_names>>id_avatar;
    const auto &keys = id_names.keys();
    for(auto &id:keys){
        id_avatarPath[id] = m_historyManager->storeImage(id_names[id],id_avatar[id]);
    }
    //更新查询后的列表，后面考虑改为信号与槽
    // add_friends.updateListView(id_names);
    emit updateStrangerList(id_names,id_avatarPath);
}

void MainWindow::handle_deleteFriend_result(QDataStream &in)
{
    bool result;
    QString friendId;
    in>>result>>friendId;
    QMessageBox msgBox;
    if(result){
        msgBox.setWindowTitle("成功");
        msgBox.setText("已删除好友");
        m_friendList.remove(friendId);
        chat_model->removeItem(friendId);
        friends_model->removeItem(friendId);
    }else{
        msgBox.setWindowTitle("失败");
        msgBox.setText("删除好友失败");
    }
    msgBox.exec();

}

QString MainWindow::getCurrentTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}


void MainWindow::handle_addFriend(QDataStream &in)
{
    QString senderName,senderId;
    QByteArray avatar;
    in>>senderName>>senderId>>avatar;
    QString avatarPath = m_historyManager->storeImage(senderName,avatar);
    qDebug()<<senderName<<senderId;
    apply_model->addFriends_ToList(senderName,senderId,"请求添加好友",avatarPath);
}

void MainWindow::send_addFriend_result(QString id)
{
    Packet data(CommonEnum::AGREE_FRIEND,id);
    m_dataTransfer->sendData(data);
}

void MainWindow::handle_addFriend_result(QDataStream &in)
{
    QString senderName,senderId;
    in>>senderName>>senderId;
    if(ui->chatList->currentIndex().data(FriendsModel::IdRole).toString()==senderId)//接收的消息和当前聊天对象是同一个才在窗口显示
        addMessage_toList("我们已成功添加好友，现在可以开始聊天啦~",senderId,m_user->get_userId(),getCurrentTime());
    // storeMessageToFile(senderId,m_friendList[senderId].userName,"我们已成功添加好友，现在可以开始聊天啦~",getCurrentTime());
    storeMessageToFile(senderId,m_friendList[senderId],"我们已成功添加好友，现在可以开始聊天啦~",getCurrentTime());
}

// void MainWindow::registerUser(const QString &username, const QString &password) {
//     if (username.isEmpty() || password.isEmpty()) return;
//     Packet data(CommonEnum::message_type::REGISTER,username,password);
//     m_dataTransfer->sendData(data);
// }

// void MainWindow::loginUser(const QString &username, const QString &password) {
//     qDebug()<<username<<password;

//     m_user->set_userName(username);
//     Packet data(CommonEnum::message_type::LOGIN,username,password);
//     m_dataTransfer->sendData(data);
// }

void MainWindow::send_slelectByName(const QString &username)
{
    if(!username.isEmpty()){
        Packet data(CommonEnum::FIND_NEW_FRIEND,username);
        m_dataTransfer->sendData(data);
    }
}

void MainWindow::handleData(QByteArray data)
{
    QDataStream in(data);
    in.setVersion(QDataStream::Qt_5_15);

    CommonEnum::message_type messageType;
    in >> messageType;
    switch(messageType){
    case CommonEnum::IMAGE:{
        //接收图片
        // qDebug()<<"IMAGE";
        receiveImage(in);
    }break;
    case CommonEnum::USER_LIST:{
        // qDebug()<<"USER_LIST";
        handle_userList(in);
    }break;
    case CommonEnum::CHAT:{
        // qDebug()<<"CHAT";
        handle_message(in);
    }break;
    case CommonEnum::LOGIN_SUCCESS:{
            handle_userInfo(in);

    }break;
    case CommonEnum::ADD_FRIEND:{
        // qDebug()<<"ADD_FRIEND";
        handle_addFriend(in);
    }break;
    case CommonEnum::AGREE_FRIEND:{
        // qDebug()<<"AGREE_FRIEND";
        handle_addFriend_result(in);
    }break;
    case CommonEnum::NEW_FRIEND_REULT:{
        // qDebug()<<"NEW_FRIEND_REULT";
        handle_selectByName_reuslt(in);
    }break;
    case CommonEnum::ONLINE_LIST:{
        // qDebug()<<"ONLINE_LIST";
        handle_onlineFriendsList(in);
    }break;
    case CommonEnum::UPDATE_AVATAR_RESULT:{
        bool result;
        in>>result;
        // qDebug()<<result;
        if(result) emit m_updateAvatar->toSetAvatar();
    }break;
    case CommonEnum::DELETEFRIEND:{
        handle_deleteFriend_result(in);
    }break;
    default:qDebug() << "未知消息类型!";break;
    }
}

void MainWindow::send_updateAvatar(const QString &path)
{
    QFile imageFile(path);
    if (!imageFile.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开头像图片文件";
        return;
    }

    // 读取图片数据
    QByteArray imageData = imageFile.readAll();
    imageFile.close();

    // QByteArray packet = getPacket(CommonEnum::UPDATE_AVATAR,imageData);
    Packet packet(CommonEnum::UPDATE_AVATAR,imageData);
    m_dataTransfer->sendData(packet);
}

void MainWindow::setAvatar(const QString &path)
{
    QFile imageFile(path);
    if (!imageFile.open(QIODevice::ReadOnly)) {
        return;
    }
    // 读取图片数据
    QByteArray imageData = imageFile.readAll();
    imageFile.close();
    m_historyManager->storeImage(m_user->get_userName(),imageData);
    m_user->set_avatarPath(path);
    showAvatar(m_user->get_avatarPath());
    update();//通知重新绘制头像
}


void MainWindow::receiveImage(QDataStream &in)
{
    QByteArray imageData;
    QString id;
    QString msgTime;
    in >> id>> imageData>>msgTime;

    QString imagePath = storeImageToFile(id,m_friendList[id],imageData,msgTime);
    if(ui->chatList->currentIndex().data(FriendsModel::IdRole).toString()==id)
        addImage_toList(imagePath,id,id,msgTime);
    else chat_model->add_unreadMsgNum(id);
}

void MainWindow::storeMessageToFile(const QString &targetId, const User &sender, const QString &message, const QString &msgTime) {
    //更新聊天列表的最新消息
    //todo 添加未读消息数量
    chat_model->updateLastMessage(targetId,message,msgTime);

    Message msg(Message::Text, message, msgTime, sender, targetId);
    m_historyManager->addHistoryToFile(msg);
}

QString MainWindow::storeImageToFile(const QString &targetId, const User &sender,const QByteArray &imageData, const QString &msgTime){
    chat_model->updateLastMessage(targetId,"图片",msgTime);

    QString imgPath = m_historyManager->storeImage("",imageData);
    Message msg(Message::Image, imgPath, msgTime, sender, targetId);
    m_historyManager->addHistoryToFile(msg);
    return imgPath;
}


void MainWindow::loadChatHistoryFromFile(QString targetId) {
    //更新好友信息
    ui->chatPartner->setText(m_friendList[targetId].getUserName());
    ui->friendState->setOnlineState(m_friendList[targetId].getOnlineState()?StateEnum::ONLINE:StateEnum::OFFLINE);
    ui->messageList->clear();

    m_historyManager->loadChatHistoryFromFile(targetId);
}


void MainWindow::handle_message(QDataStream &in)
{
    QString textMessage;
    QString id;
    QString msgTime;
    in>>id>>textMessage>>msgTime;
    // qDebug()<<id<<textMessage;

    if(ui->chatList->currentIndex().data(FriendsModel::IdRole).toString()==id)//接收的消息和当前聊天对象是同一个才在窗口显示
        addMessage_toList(textMessage,id,id,msgTime);
    else chat_model->add_unreadMsgNum(id);
    storeMessageToFile(id,m_friendList[id],textMessage,msgTime);
}



void MainWindow::addMessage_toList(const QString &text,const QString &chatId,const QString &senderId,const QString &time)
{
    ui->messageList->addTime_toList(chatId,time);

    bool isOutgoing = (m_user->get_userId()==senderId);
    QString userName = isOutgoing?m_user->get_userName():m_friendList[chatId].getUserName();
    QString avatarPath = isOutgoing?m_user->get_avatarPath():m_friendList[chatId].getAvatarPath();

    ui->messageList->addTextMessage(text,isOutgoing,userName,avatarPath,time);
}

void MainWindow::addImage_toList(const QString &imagePath,const QString &chatId,const QString &senderId,const QString &time)
{
    //因为最新的聊天消息和时间都存在聊天对象里面，并且需要判断是谁发送的消息，所以需要两个id
    //把自己和对方的最新消息都存在对方里
    ui->messageList->addTime_toList(chatId,time);

    bool isOutgoing = (m_user->get_userId()==senderId);
    QString userName = isOutgoing?m_user->get_userName():m_friendList[chatId].getUserName();
    QString avatarPath = isOutgoing?m_user->get_avatarPath():m_friendList[chatId].getAvatarPath();

    ui->messageList->addImageMessage(imagePath,isOutgoing,userName,avatarPath,time);
}


void MainWindow::handle_userList(QDataStream &in)
{
    QMap<QString,QString> id_name ;
    QMap<QString,QByteArray> id_avatar;
    in>>id_name>>id_avatar;
    updateUserList(id_name,id_avatar);
}

void MainWindow::updateUserList(const QMap<QString, QString> &newUserList,const QMap<QString,QByteArray> &new_idAvatar) {

    QSet<QString> newUserSet;
    const auto &newKeys = newUserList.keys();
    for(auto &it:newKeys)
        newUserSet<<it;
    QSet<QString> currentUserSet;
    const auto &ctKeys = m_friendList.keys();
    for(auto &it:ctKeys)
        currentUserSet<<it;
    // 计算需要删除的用户
    QSet<QString> usersToRemove = currentUserSet - newUserSet;
    // 计算新增的用户
    QSet<QString> usersToAdd = newUserSet - currentUserSet;

    // 删除用户
    for (const QString &userId : usersToRemove) {
        m_friendList.remove(userId);
        chat_model->removeItem(userId);
    }

    // 添加新用户
    for (const QString &userId : usersToAdd) {
        // m_friendList[userId].setUserName(newUserList[userId]);
        m_friendList.insert(userId,User(newUserList[userId], userId));
        QPair<QString,QString> lastMessage = m_historyManager->getLastMessage(userId);
        QString avatarPath = new_idAvatar[userId].size()?m_historyManager->storeImage(newUserList[userId],new_idAvatar[userId]):"";
        chat_model->addChat_toList(newUserList[userId],
                                   userId,
                                   lastMessage.first,
                                   lastMessage.second,
                                   0,
                                   avatarPath);
    }


    friends_model->clear();
    const auto &keys = m_friendList.keys();
    for(auto &it:keys){
        QString avatarPath = new_idAvatar[it].size()?m_historyManager->storeImage(m_friendList[it].getUserName(),new_idAvatar[it]):"";
        m_friendList[it].setAvatarPath(avatarPath);

        friends_model->addFriends_ToList(m_friendList[it].getUserName(),
                                         it,
                                         StateEnum::onlineState_type(m_friendList[it].getOnlineState()?0:1),
                                         m_friendList[it].getAvatarPath());
    }

    // 默认选择第一行
    if(ui->chatList->model()->rowCount() > 0){
        QModelIndex firstIndex = ui->chatList->model()->index(0, 0);
        ui->chatList->setCurrentIndex(firstIndex);
        QString id = ui->chatList->currentIndex().data(FriendsModel::IdRole).toString();
        loadChatHistoryFromFile(id);
        ui->chatList->selectionModel()->select(firstIndex, QItemSelectionModel::Select);
        chat_model->set_currentChatId(id);
    }
}

void MainWindow::handle_onlineFriendsList(QDataStream &in)
{
    QSet<QString> onlineList;
    in>>onlineList;
    const auto userIdList = m_friendList.keys();
    for(auto &it :userIdList){
        if(m_friendList.contains(it)){
            m_friendList[it].setOnlineState(onlineList.contains(it));
            //test
            friends_model->updateOnlineState(it,m_friendList[it].getOnlineState()?StateEnum::ONLINE:StateEnum::OFFLINE);
        }
    }
    //更新当前聊天对象的在线状态
    QModelIndex index = ui->chatList->currentIndex();
    if(index.row()!=-1){
        QString id = index.data(FriendsModel::IdRole).toString();
        ui->friendState->setOnlineState(m_friendList[id].getOnlineState()?StateEnum::ONLINE:StateEnum::OFFLINE);
    }
}

void MainWindow::handle_userInfo(QDataStream &in)
{
    QByteArray imageData;
    QString id;
    in >>id>> imageData;
    m_user->set_userId(id);
    if(imageData.isEmpty()) return;
    // qDebug()<<"存入头像"<<imageData.size();
    QString avatarPath = m_historyManager->storeImage(m_user->get_userName(),imageData);
    m_user->set_avatarPath(avatarPath);
    showAvatar(m_user->get_avatarPath());
}

// void MainWindow::onDisconnected() {//断开连接的槽函数
//     qDebug()<<"和服务器断开连接";
// }

void MainWindow::onSendClicked() {//发送按钮的槽函数
    if(ui->lineEditMessage->toPlainText().isEmpty())
        return;
    QString userId = ui->chatList->currentIndex().data(FriendsModel::IdRole).toString();
    if (!userId.isEmpty()) {
        qDebug() << "接收信息的用户id:" << userId;
    }else return;

    if(m_friendList[userId].getUserName() != m_user->get_userName()){
        Packet data(CommonEnum::message_type::CHAT,userId,ui->lineEditMessage->toPlainText());
        m_dataTransfer->sendData(data);
    }
    QString textMessage = ui->lineEditMessage->toPlainText();
    addMessage_toList(textMessage,userId,m_user->get_userId(),getCurrentTime());
    storeMessageToFile(userId,m_user->toUser(),textMessage,getCurrentTime());//保存聊天记录在硬盘中
    ui->lineEditMessage->clear(); // 清空输入框
}

void MainWindow::sendImage() {//发送图片的槽函数
    QString userId = ui->chatList->currentIndex().data(FriendsModel::IdRole).toString();
    if (userId.isEmpty()) return;
    QString imagePath = QFileDialog::getOpenFileName(this, "Select image File", "", "image Files (*.jpg *.png)");
    if(imagePath.isEmpty()) return;
    QFile imageFile(imagePath);
    if (!imageFile.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开图片文件";
        return;
    }

    // 读取图片数据
    QByteArray imageData = imageFile.readAll();
    imageFile.close();

    if(m_friendList[userId].getUserName() != m_user->get_userName()){
        // 数据包封装
        // QByteArray packet = getPacket(CommonEnum::IMAGE,userId, imageData);
        Packet packet(CommonEnum::IMAGE,userId, imageData);
        m_dataTransfer->sendData(packet);

    }

    addImage_toList(imagePath,userId,m_user->get_userId(),getCurrentTime());
    storeImageToFile(userId,m_user->toUser(),imageData,getCurrentTime());

}



