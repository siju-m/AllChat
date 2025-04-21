#include "mainwindow.h"
#include "Delegate/applydelegate.h"
#include "Delegate/friendsdelegate.h"
#include "View/Components/tipsbox.h"
#include "View/creategroup.h"

#include <QImageReader>
#include <QParallelAnimationGroup>
#include <QShortcut>
#include <QShowEvent>
#include <QSqlDatabase>
#include <windows.h>

#include <Model/Packet.h>

#include <View/Components/confirmbox.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_user(CurrentUser::getInstance()),
    m_sideBar_btnGroup(new QButtonGroup(this)),
    m_historyManager(new ChatHistoryManager(this)),
    m_dataTransfer(DataTransfer::getInstance()),
    m_friendList(m_user->getFriendList()),
    m_strangerList(m_user->getStrangerList()),
    m_groupIds(m_user->getGroupsIdList()),
    m_groupList(m_user->getGroupList())
{
    ui->setupUi(this);
    setWindowTitle("AllChat");
    this->resize(1000, 700);
    this->setWindowFlags(Qt::CustomizeWindowHint);//去除标题栏但仍可调整大小

    // 绑定按钮槽函数
    connect(ui->btnSend, &QPushButton::clicked, this, &MainWindow::onSendClicked);
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return), this);
    connect(shortcut, &QShortcut::activated, this, &MainWindow::onSendClicked);
    connect(ui->btnImage, &QPushButton::clicked, this, &MainWindow::sendImage);
    connect(ui->avatar,&AvatarLabel::clicked, this,[=](){
        m_updateAvatar = new UpdateAvatar(this);
        connect(m_updateAvatar,&UpdateAvatar::send_updateAvatar,this,&MainWindow::send_updateAvatar);
        connect(m_updateAvatar,&UpdateAvatar::setAvatar,this,&MainWindow::setAvatar);
        m_updateAvatar->setAvatarPath(m_user->get_avatarPath());
        m_updateAvatar->exec();
    });

    connect(m_dataTransfer,&DataTransfer::handleData,this,&MainWindow::handleData);

    // 连接标题栏按钮的信号
    connect(ui->titleBar, &CustomTitleBar::minimizeClicked, this, &MainWindow::showMinimized);
    connect(ui->titleBar, &CustomTitleBar::maximizeClicked, this, [this]() {
        isMaximized() ? showNormal() : showMaximized();
    });
    connect(ui->titleBar, &CustomTitleBar::closeClicked, this, &MainWindow::close);

    initFriendsList();
    initChatList();
    initAddFriends();
    initSideBar();
    initFriendApplyList();
    initHistoryManager();

    connect(ui->createGroup, &QPushButton::clicked, this, [=](){
        CreateGroup *createGroup = new CreateGroup(m_friends_model, this);
        createGroup->exec();
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::handleData(QByteArray data)
{
    QDataStream in(data);
    in.setVersion(QDataStream::Qt_5_15);

    CommonEnum::message_type messageType;
    while(!in.atEnd())
    {
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
        case CommonEnum::DELETE_FRIEND:{
            handle_deleteFriend_result(in);
        }break;
        case CommonEnum::CREATE_GROUP:{
            handle_createGroup(in);
        }break;
        case CommonEnum::GROUP_LIST:{
            handle_GroupList(in);
        }break;
        case CommonEnum::GROUP_CHAT:{
            handle_groupChat(in);
        }break;
        case CommonEnum::GROUP_STRANGER_LIST:{
            handle_strangerList(in);
        }break;
        default:qDebug() << "未知消息类型!";break;
        }
    }
}

void MainWindow::initFriendsList()
{
    m_friends_model = new FriendsModel(this);
    FriendsDelegate *friends_delegate = new FriendsDelegate(this);

    ui->friendList->setModel(m_friends_model);
    ui->friendList->setItemDelegate(friends_delegate);

    ui->friendList->setVerticalScrollMode(QListView::ScrollPerPixel); // 平滑滚动
    ui->friendList->setResizeMode(QListView::Adjust);                 // 自动调整项大小
    ui->friendList->setSelectionMode(QAbstractItemView::SingleSelection);  // 单选
    ui->friendList->setSelectionBehavior(QAbstractItemView::SelectItems);

    ui->friendList->setUniformItemSizes(true);//启用统一项尺寸优化

    connect(ui->friendList, &QListView::clicked, this, [=](const QModelIndex &index){
        QString id = index.data(FriendsModel::IdRole).toString();
        ui->friendInfo->showUserInfo(id,m_friendList[id]);
    });
    connect(ui->friendInfo,&UserDetailView::showMessage,this,&MainWindow::switch_chatUser);
    connect(ui->friendInfo,&UserDetailView::deleteFriend,this,[=](const QString &id){
        if(ConfirmBox::question(this)){
            Packet data(CommonEnum::DELETE_FRIEND,id);
            m_dataTransfer->sendData(data);
            ui->friendInfo->hideUserInfo();
        }
    });
}

void MainWindow::initChatList()
{
    m_chat_model = new ChatModel(this);
    ChatDelegate *chat_delegate = new ChatDelegate(this);

    ui->chatList->setModel(m_chat_model);
    ui->chatList->setItemDelegate(chat_delegate);

    ui->chatList->setVerticalScrollMode(QListView::ScrollPerPixel); // 平滑滚动
    ui->chatList->setResizeMode(QListView::Adjust);                 // 自动调整项大小
    ui->chatList->setSelectionMode(QAbstractItemView::SingleSelection);  // 单选
    ui->chatList->setSelectionBehavior(QAbstractItemView::SelectItems);

    ui->chatList->setUniformItemSizes(true);//启用统一项尺寸优化

    connect(ui->chatList, &QListView::clicked, this, [=](const QModelIndex &index){
        QString id = index.data(FriendsModel::IdRole).toString();
        loadChatHistoryFromFile(id);
        m_chat_model->set_currentChatId(id);
        m_chat_model->clear_unreadMsgNum(id);
    });
    connect(m_chat_model,&ChatModel::sortEnd,this,[=](){
        //将当前聊天对象的选择设为排序后的当前聊天对象
        //QTimer的作用 使得排序完成后的其他事件先处理，再更新选择状态
        QTimer::singleShot(10, this, [=]() {
            if(ui->chatList->model()->rowCount() > 0){
                int row = m_chat_model->get_currentChatRow();
                QModelIndex index = ui->chatList->model()->index(row, 0);
                if(ui->chatList->currentIndex() == index)
                    return;
                ui->chatList->setCurrentIndex(index);
                // QString id = index.data(FriendsModel::IdRole).toString();
                // loadChatHistoryFromFile(id);
            }
        });
    });
}

void MainWindow::initAddFriends()
{
    connect(ui->addFriends,&QPushButton::clicked,this,[=](){
        m_add_friends.exec();
    });
    connect(&m_add_friends,&AddFriends::send_slelectByName,this,&MainWindow::send_slelectByName);
    connect(&m_add_friends,&AddFriends::sendData,this,[=](const QString &id){
        // this->sendData(CommonEnum::ADD_FRIEND,id);
        Packet data(CommonEnum::ADD_FRIEND,id);
        m_dataTransfer->sendData(data);
    });
    connect(&m_add_friends,&AddFriends::showMessage,this, &MainWindow::switch_chatUser);
    connect(this,&MainWindow::updateStrangerList,&m_add_friends,&AddFriends::updateListView);
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

        if(button == ui->switchaddList){
            QLabel *redDot = ui->switchaddList->findChild<QLabel*>("redDot");
            if (redDot) {
                redDot->setVisible(false); // 或其他操作
            }
        }
    });

    ui->stackedList->setCurrentIndex(0);//默认显示聊天列表
    m_sideBar_btnGroup->button(0)->setChecked(true);
}

void MainWindow::initFriendApplyList()
{
    m_apply_model = new StrangerModel(this);
    ApplyDelegate *apply_delegate = new ApplyDelegate(this);
    ui->friendApplyList->setModel(m_apply_model);
    ui->friendApplyList->setItemDelegate(apply_delegate);
    ui->friendApplyList->viewport()->setMouseTracking(true);

    connect(apply_delegate,&ApplyDelegate::applyResult,this,[=](const QString &id,const int &row){
        send_addFriend_result(id);
        m_apply_model->removeItem(row);
    });
}

void MainWindow::initHistoryManager()
{
    connect(m_historyManager,&ChatHistoryManager::addMessage_toList,this,&MainWindow::showMessage_toList);
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
    emit updateStrangerList(id_names,id_avatarPath);
}

void MainWindow::handle_deleteFriend_result(QDataStream &in)
{
    bool result;
    QString friendId;
    in>>result>>friendId;
    if(result){
        TipsBox::showNotice("成功删除好友", SA_SUCCESS, this);
        m_friendList.remove(friendId);
        m_chat_model->removeItem(friendId);
        m_friends_model->removeItem(friendId);

        resetChatState();
    }else{
        TipsBox::showNotice("删除好友失败", SA_FAILED, this);
    }
}

void MainWindow::resetChatState()
{
    ui->chatList->clearSelection();
    ui->chatList->setCurrentIndex(QModelIndex());  // 取消当前索引

    ui->messageList->clear();
    ui->chatPartner->clear();
    ui->friendState->setOnlineState(StateEnum::NONE);
}

QString MainWindow::getCurrentTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}


void MainWindow::handle_addFriend(QDataStream &in)
{
    if(m_apply_model->isEmpty()){
        // 创建一个 QLabel 当作红点
        QLabel *redDot = new QLabel(ui->switchaddList);
        redDot->setObjectName("redDot");
        redDot->setFixedSize(10, 10);
        redDot->move(ui->switchaddList->width() - 15, 3); // 右上角

        // 设置红点样式
        redDot->setStyleSheet("background-color: red; border-radius: 5px;");
        redDot->show();
    }else{
        QLabel *redDot = ui->switchaddList->findChild<QLabel*>("redDot");
        if (redDot) {
            redDot->show();
        }
    }
    QString senderName,senderId;
    QByteArray avatar;
    in>>senderName>>senderId>>avatar;
    QString avatarPath = m_historyManager->storeImage(senderName,avatar);
    // qDebug()<<senderName<<senderId;
    m_apply_model->addFriends_ToList(senderName,senderId,false,avatarPath);
}

void MainWindow::send_addFriend_result(QString id)
{
    Packet data(CommonEnum::AGREE_FRIEND,id);
    m_dataTransfer->sendData(data);
}

void MainWindow::handle_addFriend_result(QDataStream &in)
{
    QString agreeId, applyId;
    in >> agreeId >> applyId;
    // qDebug()<<agreeId+" 同意 "+applyId<<"当前用户 "+m_user->get_userId();

    User sender;
    QString senderId;
    if(agreeId == m_user->get_userId())
    {
        sender = m_user->toUser();
        senderId = applyId;
    }
    else if(applyId == m_user->get_userId())
    {
        sender = *m_friendList.find(agreeId);
        senderId = agreeId;
    }
    Message msg(Message::Text, "我们已成功添加好友，现在可以开始聊天啦~", getCurrentTime(), sender, senderId);
    addMessage_toList(msg);

}


void MainWindow::send_slelectByName(const QString &username)
{
    if(!username.isEmpty()){
        Packet data(CommonEnum::FIND_NEW_FRIEND,username);
        m_dataTransfer->sendData(data);
    }
}

void MainWindow::send_updateAvatar(const QString &path)
{
    if(path.isEmpty())
        return;
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

void MainWindow::switch_chatUser(const QString &id)
{
    int row = m_chat_model->get_rowById(id);
    QModelIndex index = ui->chatList->model()->index(row, 0);
    m_sideBar_btnGroup->button(0)->click();
    emit ui->chatList->clicked(index);
    ui->chatList->setCurrentIndex(index);
}


void MainWindow::handle_createGroup(QDataStream &in)
{
    QString groupId, groupName;
    int count;
    in >> groupId >> groupName >> count;
    m_groupIds.insert(groupId);
    m_groupList.insert(groupId, Group(groupId, groupName, count));
    m_chat_model->addChat_toList(groupName, groupId, "", "", 0);
}

void MainWindow::handle_GroupList(QDataStream &in)
{
    QByteArray groups;
    in >> groups;
    QDataStream data(&groups, QIODevice::ReadOnly);
    data.setVersion(QDataStream::Qt_5_15);

    qsizetype size;
    data >> size;
    QString id, name;
    int count;

    for(qsizetype i=0; i<size; ++i){
        data >> id >> name >> count;
        m_groupIds.insert(id);
        m_groupList.insert(id, Group(id, name, count));
        QPair<QString,QString> lastMessage = m_historyManager->getLastMessage(id);
        m_chat_model->addChat_toList(name, id, lastMessage.first, lastMessage.second, 0);
    }

    // 选择排序后的第一个
    // QString fristId = ui->chatList->model()->index(0, 0).data(FriendsModel::IdRole).toString();
    // switch_chatUser(fristId);
}

void MainWindow::handle_groupChat(QDataStream &in)
{
    QString groupId, senderId;
    in >> groupId >> senderId;
    User sender;
    if(m_friendList.contains(senderId))
    {
        sender = m_friendList[senderId];
    }
    else if(m_strangerList.contains(senderId))
    {
        sender = m_strangerList[senderId];
    }

    QString type, msgTime;
    in >> type;
    if(type == "TEXT")
    {
        QString text;
        in >> text >> msgTime;

        Message msg(Message::Text, text, msgTime, sender, groupId);
        addMessage_toList(msg);
    }
    else if(type == "IMAGE")
    {
        QByteArray image;
        in >> image >> msgTime;

        QString path = m_historyManager->storeImage("", image);
        Message msg(Message::Image, path, msgTime, sender, groupId);
        addMessage_toList(msg);
    }
}

void MainWindow::handle_strangerList(QDataStream &in)
{
    // 添加群成员中陌生人的信息
    QByteArray strangerList;
    in >> strangerList;
    QDataStream data(&strangerList, QIODevice::ReadOnly);
    data.setVersion(QDataStream::Qt_5_15);
    qsizetype size;
    data >> size;
    for(qsizetype i = 0; i < size; ++i)
    {
        QString id;
        QString name;
        QByteArray avatar;
        data >> id >> name >> avatar;
        QString path = m_historyManager->storeImage(name, avatar);
        User user(name, id, path);
        m_strangerList.insert(id, user);
    }
}

void MainWindow::receiveImage(QDataStream &in)
{
    QByteArray imageData;
    QString id;
    QString msgTime;
    in >> id>> imageData>>msgTime;

    QString imagePath = m_historyManager->storeImage("", imageData);
    Message msg(Message::Image, imagePath, msgTime, m_friendList[id], id);
    addMessage_toList(msg);
}

void MainWindow::storeMessageToFile(const Message &msg) {
    //更新聊天列表的最新消息
    //todo 添加未读消息数量
    m_chat_model->updateLastMessage(msg.getChatId(), msg.getPlainText(), msg.getTime());

    Message message = msg;
    m_historyManager->addHistoryToFile(message);
}


// void MainWindow::storeMessageToFile(const QString &targetId, const User &sender, const QString &message, const QString &msgTime) {
//     //更新聊天列表的最新消息
//     //todo 添加未读消息数量
//     m_chat_model->updateLastMessage(targetId,message,msgTime);

//     Message msg(Message::Text, message, msgTime, sender, targetId);
//     m_historyManager->addHistoryToFile(msg);
// }

// QString MainWindow::storeImageToFile(const QString &targetId, const User &sender,const QByteArray &imageData, const QString &msgTime){
//     m_chat_model->updateLastMessage(targetId,"图片",msgTime);

//     QString imgPath = m_historyManager->storeImage("",imageData);
//     Message msg(Message::Image, imgPath, msgTime, sender, targetId);
//     m_historyManager->addHistoryToFile(msg);
//     return imgPath;
// }


void MainWindow::loadChatHistoryFromFile(QString targetId) {
    //更新好友信息
    if(!m_groupIds.contains(targetId)){
        ui->chatPartner->setText(m_friendList[targetId].getUserName());
        ui->friendState->setOnlineState(m_friendList[targetId].getOnlineState()?StateEnum::ONLINE:StateEnum::OFFLINE);
    }else{
        int row = m_chat_model->get_rowById(targetId);
        QString name = ui->chatList->model()->index(row, 0).data(ChatModel::UserNameRole).toString();
        ui->chatPartner->setText(QString(name + "（ %1 ）").arg(m_groupList.find(targetId)->memberCount()));
        ui->friendState->setOnlineState(StateEnum::NONE);
    }

    ui->messageList->clear(targetId);
    m_historyManager->loadChatHistoryFromFile(targetId);
}

void MainWindow::addMessage_toList(const Message &message)
{
    showMessage_toList(message);
    storeMessageToFile(message);
}

void MainWindow::showMessage_toList(const Message &message)
{
    QString chatId = message.getChatId();
    if(ui->chatList->currentIndex().data(FriendsModel::IdRole).toString()==chatId)//接收的消息和当前聊天对象是同一个才在窗口显示
    {
        QString time = message.getTime();
        ui->messageList->addTime_toList(chatId,time);
        ui->messageList->addMessage(message);
    }
    else m_chat_model->add_unreadMsgNum(chatId);
}


void MainWindow::handle_message(QDataStream &in)
{
    QString textMessage;
    QString id;
    QString msgTime;
    in>>id>>textMessage>>msgTime;
    // qDebug()<<id<<textMessage;


    Message msg(Message::Text, textMessage, msgTime, m_friendList[id], id);
    addMessage_toList(msg);
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
        m_chat_model->removeItem(userId);
    }

    // 添加新用户
    for (const QString &userId : usersToAdd) {
        // m_friendList[userId].setUserName(newUserList[userId]);
        m_friendList.insert(userId,User(newUserList[userId], userId));
        QPair<QString,QString> lastMessage = m_historyManager->getLastMessage(userId);
        QString avatarPath = new_idAvatar[userId].size()?m_historyManager->storeImage(newUserList[userId],new_idAvatar[userId]):"";
        m_chat_model->addChat_toList(newUserList[userId],
                                   userId,
                                   lastMessage.first,
                                   lastMessage.second,
                                   0,
                                   avatarPath);
    }


    m_friends_model->clear();
    const auto &keys = m_friendList.keys();
    for(auto &it:keys){
        QString avatarPath = new_idAvatar[it].size()?m_historyManager->storeImage(m_friendList[it].getUserName(),new_idAvatar[it]):"";
        m_friendList[it].setAvatarPath(avatarPath);

        m_friends_model->addFriends_ToList(m_friendList[it].getUserName(),
                                         it,
                                         StateEnum::onlineState_type(m_friendList[it].getOnlineState()?0:1),
                                         m_friendList[it].getAvatarPath());
    }

    // 默认选择第一行
    // if(ui->chatList->model()->rowCount() > 0){
    //     QString fristId = ui->chatList->model()->index(0, 0).data(FriendsModel::IdRole).toString();
    //     switch_chatUser(fristId);
    // }
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
            m_friends_model->updateOnlineState(it,m_friendList[it].getOnlineState()?StateEnum::ONLINE:StateEnum::OFFLINE);
        }
    }
    //更新当前聊天对象的在线状态
    QModelIndex index = ui->chatList->currentIndex();
    if(index.row()!=-1){
        QString id = index.data(FriendsModel::IdRole).toString();
        if(m_friendList.contains(id))
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
    QString chatId = ui->chatList->currentIndex().data(FriendsModel::IdRole).toString();
    if (!chatId.isEmpty()) {
        qDebug() << "当前聊天对象id:" << chatId;
    }else return;

    Packet data;
    if(m_friendList.contains(chatId)){
        data = Packet(CommonEnum::CHAT, chatId, ui->lineEditMessage->toPlainText());
    }else{
        QString type("TEXT");
        data = Packet(CommonEnum::GROUP_CHAT, chatId, type, ui->lineEditMessage->toPlainText());
    }
    m_dataTransfer->sendData(data);

    QString textMessage = ui->lineEditMessage->toPlainText();
    Message msg(Message::Text, textMessage, getCurrentTime(), m_user->toUser(), chatId);
    addMessage_toList(msg);
    ui->lineEditMessage->clear(); // 清空输入框
}

void MainWindow::sendImage() {//发送图片的槽函数
    QString chatId = ui->chatList->currentIndex().data(FriendsModel::IdRole).toString();
    if (chatId.isEmpty())
        return;
    QString imagePath = QFileDialog::getOpenFileName(this, "Select image File", "", "image Files (*.jpg *.png)");
    if(imagePath.isEmpty())
        return;
    QFile imageFile(imagePath);
    if (!imageFile.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开图片文件";
        return;
    }

    // 读取图片数据
    QByteArray imageData = imageFile.readAll();
    imageFile.close();

    Packet data;
    if(m_friendList.contains(chatId)){
        data = Packet(CommonEnum::IMAGE,chatId, imageData);
    }else{
        QString type("IMAGE");
        data = Packet(CommonEnum::GROUP_CHAT, chatId, type, imageData);
    }
    m_dataTransfer->sendData(data);


    Message msg(Message::Image, imagePath, getCurrentTime(), m_user->toUser(), chatId);
    addMessage_toList(msg);

}



