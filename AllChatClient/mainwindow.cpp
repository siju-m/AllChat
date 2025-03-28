#include "mainwindow.h"

#include <QImageReader>
#include <QQuickView>
#include <QShortcut>
#include <QShowEvent>
#include <QSqlDatabase>
#include <windows.h>

#include "View/imageviewer.h"

#include <Model/Packet.h>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    // socket(new QTcpSocket(this)),
    // m_avatarPath(""),
    m_user(CurrentUser::getInstance()),
    m_sideBar_btnGroup(new QButtonGroup(this)),
    m_historyManager(new ChatHistoryManager(this)),
    m_dataTransfer(new DataTransfer(this))
{

    ui->setupUi(this);
    setWindowTitle("AllChat");
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

    initMessageList();
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

//todo 封装到messageListView里面
void MainWindow::initMessageList()
{
    message_model = new MessageModel(this);
    MessageDelegate *message_delegate = new MessageDelegate(this);

    /*使用listView在消息比较多的情况下内存低、可以流畅滚动
    用widget的话消息一多就比较占用内存*/
    ui->messageList->setModel(message_model);
    ui->messageList->setItemDelegate(message_delegate);
    ui->messageList->setVerticalScrollMode(QListView::ScrollPerPixel); // 平滑滚动
    ui->messageList->setResizeMode(QListView::Adjust);                 // 自动调整项大小

    // 设置ListView属性
    ui->messageList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//始终隐藏水平滚动条
    ui->messageList->setSelectionMode(QAbstractItemView::NoSelection);


    QScrollBar *verticalScrollBar = ui->messageList->verticalScrollBar();
    verticalScrollBar->setSingleStep(10); // 垂直滚动步长
    verticalScrollBar->setPageStep(100);  // 垂直滚动一页的距离

    connect(message_delegate,&MessageDelegate::imageClicked,this,[=](const QPixmap &image){
        ImageViewer *viewer = new ImageViewer(image);
        viewer->resize(800, 600);
        viewer->exec(); // 或 show() 非模态显示
    });
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
        ui->friendInfo->showUserInfo(m_friendList[id].userName,id,m_friendList[id].state,m_friendList[id].avatarPath);
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

bool MainWindow::compareTime(const QString &pastTime,const QString &lastTime)
{
    QDateTime lastDateTime = QDateTime::fromString(lastTime,"yyyy-MM-dd hh:mm:ss");
    QDateTime pastDateTime = QDateTime::fromString(pastTime,"yyyy-MM-dd hh:mm:ss");
    if (lastDateTime.isValid() && pastDateTime.isValid()) {
        // 计算相差分钟数
        qint64 diffSeconds = lastDateTime.secsTo(pastDateTime); // time1 到 time2 的秒数（可能是负数）
        qint64 diffMinutes = diffSeconds / 60;    // 转换成分钟
        if(qAbs(diffMinutes)>2) return true;
        // qDebug() << "相差" << qAbs(diffMinutes) << "分钟"; // 取绝对值显示
    } else {
        qDebug() << "时间格式错误";
    }
    return false;
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
        // addMessage_toList("我们已成功添加好友，现在可以开始聊天啦~",senderId,m_userId,getCurrentTime());
        addMessage_toList("我们已成功添加好友，现在可以开始聊天啦~",senderId,m_user->get_userId(),getCurrentTime());
    storeMessageToFile(senderId,m_friendList[senderId].userName,"我们已成功添加好友，现在可以开始聊天啦~",getCurrentTime());
}

void MainWindow::registerUser(const QString &username, const QString &password) {
    if (username.isEmpty() || password.isEmpty()) return;
    // sendData(CommonEnum::message_type::REGISTER,username,password);
    Packet data(CommonEnum::message_type::REGISTER,username,password);
    m_dataTransfer->sendData(data);
}

void MainWindow::loginUser(const QString &username, const QString &password) {
    // this->m_userName = username;
    m_user->set_userName(username);
    Packet data(CommonEnum::message_type::LOGIN,username,password);
    m_dataTransfer->sendData(data);
}

void MainWindow::send_slelectByName(const QString &username)
{
    if(!username.isEmpty()){
        // sendData(CommonEnum::FIND_NEW_FRIEND,username);
        Packet data(CommonEnum::FIND_NEW_FRIEND,username);
        m_dataTransfer->sendData(data);
    }
}


void MainWindow::handleData(QByteArray data)
{
    QDataStream in(data);
    in.setVersion(QDataStream::Qt_5_15);

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
    case CommonEnum::LOGIN_SUCCESS:
    case CommonEnum::LOGIN_FAILED:{
        emit loginResult(messageType);
        if(messageType == CommonEnum::LOGIN_SUCCESS){
            handle_userInfo(in);
        }
    }break;
    case CommonEnum::REGISTER_SUCCESS:
    case CommonEnum::REGISTER_FAILED:{
        emit registResult(messageType);
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
    // storeImage(m_userName,imageData);
    // m_avatarPath = path;
    // showAvatar(m_avatarPath);
    m_historyManager->storeImage(m_user->get_userName(),imageData);
    m_user->set_avatarPath(path);
    showAvatar(m_user->get_avatarPath());
    update();//通知重新绘制头像
}

// void MainWindow::onReadyRead() {
//     m_dataTransfer->receiveData(socket);
// }

void MainWindow::receiveImage(QDataStream &in)
{
    QByteArray imageData;
    QString id;
    QString msgTime;
    in >> id>> imageData>>msgTime;

    QString imagePath = storeImageToFile(id,m_friendList[id].userName,imageData,msgTime);
    if(ui->chatList->currentIndex().data(FriendsModel::IdRole).toString()==id)
        addImage_toList(imagePath,id,id,msgTime);
    else chat_model->add_unreadMsgNum(id);
}

void MainWindow::storeMessageToFile(const QString &targetId, const QString &sender, const QString &message, const QString &msgTime) {
    //更新聊天列表的最新消息
    //todo 添加未读消息数量
    chat_model->updateLastMessage(targetId,message,msgTime);

    m_historyManager->storeMessageToFile(targetId,sender,message,msgTime);
}

QString MainWindow::storeImageToFile(const QString &targetId, const QString &sender,const QByteArray &imageData, const QString &msgTime){
    chat_model->updateLastMessage(targetId,"图片",msgTime);

    return m_historyManager->storeImageToFile(targetId,sender,imageData,msgTime);
}


void MainWindow::loadChatHistoryFromFile(QString targetId) {
    //更新好友信息
    ui->chatPartner->setText(m_friendList[targetId].userName);
    ui->friendState->setOnlineState(m_friendList[targetId].state?StateEnum::ONLINE:StateEnum::OFFLINE);
    message_model->clear();

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
    storeMessageToFile(id,m_friendList[id].userName,textMessage,msgTime);
}



void MainWindow::addMessage_toList(const QString &text,const QString &chatId,const QString &senderId,const QString &time)
{
    addTime_toList(chatId,time);
    bool isOutgoing = (m_user->get_userId()==senderId);
    QString userName = isOutgoing?m_user->get_userName():m_friendList[chatId].userName;
    QString avatarPath = isOutgoing?m_user->get_avatarPath():m_friendList[chatId].avatarPath;
    message_model->addTextMessage(text,isOutgoing,userName,avatarPath,time);
    ui->messageList->scrollToBottom(); // 自动滚动到底部
}

void MainWindow::addImage_toList(const QString &imagePath,const QString &chatId,const QString &senderId,const QString &time)
{
    //因为最新的聊天消息和时间都存在聊天对象里面，并且需要判断是谁发送的消息，所以需要两个id
    //把自己和对方的最新消息都存在对方里
    addTime_toList(chatId,time);
    bool isOutgoing = (m_user->get_userId()==senderId);
    QString userName = isOutgoing?m_user->get_userName():m_friendList[chatId].userName;
    QString avatarPath = isOutgoing?m_user->get_avatarPath():m_friendList[chatId].avatarPath;
    message_model->addImageMessage(imagePath,isOutgoing,userName,avatarPath,time);
    ui->messageList->scrollToBottom(); // 自动滚动到底部
}

void MainWindow::addTime_toList(const QString &chatId,const QString &time)
{
    // QString lastMsgTime = chat_model->get_lastTempTime(chatId);
    QString lastMsgTime = message_model->get_lastTempTime(chatId);
    if(lastMsgTime.isEmpty()){
        message_model->addTimeMessage(time);
    }else{
        bool isTooLong = compareTime(lastMsgTime,time);
        if(isTooLong) message_model->addTimeMessage(time);
    }
    message_model->update_lastTempTime(chatId,time);
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
        m_friendList[userId].userName = newUserList[userId];
        QPair<QString,QString> lastMessage = m_historyManager->getLastMessage(userId);
        QString avatarPath = new_idAvatar[userId].size()?m_historyManager->storeImage(newUserList[userId],new_idAvatar[userId]):"";
        chat_model->addChat_toList(newUserList[userId],
                                   userId,
                                   lastMessage.first,
                                   lastMessage.second,
                                   0,
                                   avatarPath);
    }

    //test
    friends_model->clear();
    const auto &keys = m_friendList.keys();
    for(auto &it:keys){
        m_friendList[it].avatarPath = new_idAvatar[it].size()?m_historyManager->storeImage(m_friendList[it].userName,new_idAvatar[it]):"";
        //test
        friends_model->addFriends_ToList(m_friendList[it].userName,it,StateEnum::onlineState_type(m_friendList[it].state?0:1),m_friendList[it].avatarPath);
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
            m_friendList[it].state=onlineList.contains(it);
            //test
            friends_model->updateOnlineState(it,m_friendList[it].state?StateEnum::ONLINE:StateEnum::OFFLINE);
        }
    }
    //更新当前聊天对象的在线状态
    QModelIndex index = ui->chatList->currentIndex();
    if(index.row()!=-1){
        QString id = index.data(FriendsModel::IdRole).toString();
        ui->friendState->setOnlineState(m_friendList[id].state?StateEnum::ONLINE:StateEnum::OFFLINE);
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

    if(m_friendList[userId].userName != m_user->get_userName()){
        Packet data(CommonEnum::message_type::CHAT,userId,ui->lineEditMessage->toPlainText());
        m_dataTransfer->sendData(data);
    }
    QString textMessage = ui->lineEditMessage->toPlainText();
    addMessage_toList(textMessage,userId,m_user->get_userId(),getCurrentTime());
    storeMessageToFile(userId,m_user->get_userName(),textMessage,getCurrentTime());//保存聊天记录在硬盘中
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

    if(m_friendList[userId].userName != m_user->get_userName()){
        // 数据包封装
        // QByteArray packet = getPacket(CommonEnum::IMAGE,userId, imageData);
        Packet packet(CommonEnum::IMAGE,userId, imageData);
        m_dataTransfer->sendData(packet);

    }

    addImage_toList(imagePath,userId,m_user->get_userId(),getCurrentTime());
    storeImageToFile(userId,m_user->get_userName(),imageData,getCurrentTime());

}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    // 获取窗口句柄
    HWND hwnd = reinterpret_cast<HWND>(winId());
    // 获取当前样式
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    // 去掉标题栏，保留边框和调整大小功能
    style &= ~WS_CAPTION;
    SetWindowLong(hwnd, GWL_STYLE, style);
    // 更新窗口外观
    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

