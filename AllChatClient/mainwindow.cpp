#include "mainwindow.h"
#include "Core/contactmanager.h"
#include "Delegate/applydelegate.h"
#include "Delegate/chatdelegate.h"
#include "Delegate/friendsdelegate.h"
#include "View/Components/sidebar.h"
#include "View/Components/tipsbox.h"
#include "View/creategroup.h"
#include "Utils/commonutil.h"

#include <QImageReader>
#include <QMenu>
#include <QAction>
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
    m_historyManager(ChatHistoryManager::getInstance()),
    m_dataTransfer(DataTransfer::getInstance()),
    m_contact_manager(ContactManager::getInstance()),
    m_msgSenderView(new MessageSenderView(this)),
    m_search_result_list(new SearchResultList(this))
{
    ui->setupUi(this);
    this->setWindowTitle("AllChat");
    this->resize(1000, 700);
    this->setWindowIcon(QIcon(":/Icon/app.png"));
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::Window);//去除标题栏但仍可调整大小

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


    m_create_group = new CreateGroup(m_friends_model, this);
    // 弹出创建群聊窗口
    connect(ui->chatSearch, &SearchBar::createGroup, m_create_group, &CreateGroup::exec);

    // 聊天窗口划到顶部加载更早的消息
    connect(ui->messageList, &MessageListView::scrolledToTop, this, [=](){
        m_historyManager->loadChatHistoryFromFile(m_user->get_currentChatId());
    });

    initTray(); // 初始化托盘

    ui->chatView->layout()->addWidget(m_msgSenderView);
    connect(m_msgSenderView, &MessageSenderView::sendMsg, this, &MainWindow::addMessage_toList);

    connect(m_contact_manager, &ContactManager::clearModel, this, &MainWindow::onClearModel);
    connect(m_contact_manager, &ContactManager::addChatModel, m_chat_model, &ChatModel::addChat_toList);
    connect(m_contact_manager, &ContactManager::addFriendModel, m_friends_model, &FriendsModel::addFriends_ToList);

    connect(ui->btn_applyList, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->apply_page);
        hideApplyRedDot();
    });

    ui->stackedList->addWidget(m_search_result_list);
    connect(ui->chatSearch, &SearchBar::search, this, &MainWindow::onSearchFrd);
    connect(ui->chatSearch, &SearchBar::hasClear, this, [=](){
        ui->stackedList->setCurrentWidget(ui->chat_page);
    });
    connect(m_search_result_list, &SearchResultList::clickedItem, this, &MainWindow::switch_chatUser);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::handleData(QByteArray data)
{
    QDataStream in(data);
    in.setVersion(ConfigManager::dataStreamVersion());

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
            handleUserList(in);
        }break;
        case CommonEnum::CHAT:{
            // qDebug()<<"CHAT";
            handle_message(in);
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
        case CommonEnum::ONLINE_STATE:{
            // handle_onlineFriendsList(in);
            handleFrdOnlineState(in);
        }break;
        case CommonEnum::ONLINE_LIST:{
            handleFrdOnlineList(in);
        }break;
        case CommonEnum::UPDATE_AVATAR_RESULT:{
            bool result;
            in>>result;
            // qDebug()<<result;
            if(result) emit m_side_bar->successSetAvatar();
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
        case CommonEnum::PRIVATE_FILE:{
            handle_privateFile(in);
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
    ui->friendList->setEditTriggers(QAbstractItemView::NoEditTriggers);


    ui->friendList->setUniformItemSizes(true);//启用统一项尺寸优化

    connect(ui->friendList, &QListView::clicked, this, [=](const QModelIndex &index){
        QString id = index.data(FriendsModel::IdRole).toString();
        ui->friendInfo->showUserInfo(id,m_contact_manager->friendById(id));
        ui->stackedWidget->setCurrentWidget(ui->info_page);
    });
    connect(ui->friendInfo,&UserDetailView::showMessage,this,&MainWindow::switch_chatUser);
    connect(ui->friendInfo,&UserDetailView::deleteFriend,this,[=](const QString &id){
        if(ConfirmBox::question(this, "删除", "确认删除", "确定", "取消")){
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

    QScrollBar *verticalScrollBar = ui->chatList->verticalScrollBar();
    verticalScrollBar->setSingleStep(10); // 垂直滚动步长
    verticalScrollBar->setPageStep(100);  // 垂直滚动一页的距离

    connect(ui->chatList, &QListView::clicked, this, [=](const QModelIndex &index){
        QString id = index.data(ChatModel::IdRole).toString();
        if(m_user->get_currentChatId() != id){
            // qDebug() << id;
            loadChatHistoryFromFile(id);
            m_chat_model->set_currentChatId(id);
            m_chat_model->clear_unreadMsgNum(id);
            m_user->set_currentChatId(id);
        }
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
            }
        });
    });
}

void MainWindow::initAddFriends()
{
    connect(ui->chatSearch,&SearchBar::addFriend,this,[=](){
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
    m_side_bar = new SideBar(this);
    ui->horizontalLayout_4->insertWidget(0,m_side_bar);
    ui->stackedList->setCurrentIndex(0);//默认显示聊天列表
    connect(m_side_bar, &SideBar::curIndex, this, &MainWindow::onSwitchView);
}

void MainWindow::initFriendApplyList()
{
    m_apply_model = new StrangerModel(this);
    ApplyDelegate *apply_delegate = new ApplyDelegate(this);
    ui->friendApplyList->setModel(m_apply_model);
    ui->friendApplyList->setItemDelegate(apply_delegate);
    ui->friendApplyList->viewport()->setMouseTracking(true);

    connect(apply_delegate, &ApplyDelegate::applyResult, this, &MainWindow::onAddFriendReply);
}

void MainWindow::initHistoryManager()
{
    connect(m_historyManager, &ChatHistoryManager::addMessage_toList, ui->messageList, &MessageListView::addOlderMessage);
}

void MainWindow::handle_selectByName_reuslt(QDataStream &in)
{
    QMap<QString,QString> id_names;
    QMap<QString,QByteArray> id_avatar;
    QMap<QString,QString> id_avatarPath;
    in>>id_names>>id_avatar;
    const auto &keys = id_names.keys();
    for(auto &id:keys){
        id_avatarPath[id] = m_historyManager->storeAvatar(id_names[id],id_avatar[id]);
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
        m_contact_manager->removeFriend(friendId);
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

void MainWindow::handle_addFriend(QDataStream &in)
{
    showApplyRedDot();
    QString senderName,senderId;
    QByteArray avatar;
    in>>senderName>>senderId>>avatar;
    QString avatarPath = m_historyManager->storeAvatar(senderName,avatar);
    // qDebug()<<senderName<<senderId;
    m_apply_model->addFriends_ToList(senderName,senderId,false,avatarPath);
}

void MainWindow::onAddFriendReply(const QString &id, const int &row)
{
    Packet data(CommonEnum::AGREE_FRIEND,id);
    m_dataTransfer->sendData(data);
    m_apply_model->removeItem(row);
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
        sender = m_contact_manager->friendById(agreeId);
        senderId = agreeId;
    }
    Message msg(Message::Text, "我们已成功添加好友，现在可以开始聊天啦~", CommonUtil::getCurrentTime(), sender, senderId);
    addMessage_toList(msg);

}


void MainWindow::send_slelectByName(const QString &username)
{
    if(!username.isEmpty()){
        Packet data(CommonEnum::FIND_NEW_FRIEND,username);
        m_dataTransfer->sendData(data);
    }
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
    m_historyManager->storeAvatar(m_user->get_userName(),imageData);
    m_user->set_avatarPath(path);
    // showAvatar(m_user->get_avatarPath());
    m_side_bar->onSetAvatar(m_user->get_avatarPath());
    update();//通知重新绘制头像
}

void MainWindow::switch_chatUser(const QString &id)
{
    int row = m_chat_model->get_rowById(id);
    QModelIndex index = ui->chatList->model()->index(row, 0);
    m_side_bar->switchToChat();
    emit ui->chatList->clicked(index);
    ui->chatList->setCurrentIndex(index);
}


void MainWindow::handle_createGroup(QDataStream &in)
{
    QString groupId, groupName;
    int count;
    in >> groupId >> groupName >> count;
    m_contact_manager->addGroupId(groupId);
    m_contact_manager->addGroup(groupId, Group(groupId, groupName, count));
    m_chat_model->addChat_toList(groupName, groupId, "", "", 0);
}

void MainWindow::handle_GroupList(QDataStream &in)
{
    QByteArray groups;
    in >> groups;
    QDataStream data(&groups, QIODevice::ReadOnly);
    data.setVersion(ConfigManager::dataStreamVersion());

    qsizetype size;
    data >> size;
    QString id, name;
    int count;

    for(qsizetype i=0; i<size; ++i){
        data >> id >> name >> count;
        m_contact_manager->addGroupId(id);
        m_contact_manager->addGroup(id, Group(id, name, count));
        QPair<QString,QString> lastMessage = m_historyManager->getLastMessage(id);
        m_chat_model->addChat_toList(name, id, lastMessage.first, lastMessage.second, 0);
    }
}

void MainWindow::handle_groupChat(QDataStream &in)
{
    QString groupId, senderId;
    in >> groupId >> senderId;
    User sender;
    if(m_contact_manager->isFriend(senderId))
    {
        sender = m_contact_manager->friendById(senderId);
    }
    else if(m_contact_manager->isStranger(senderId))
    {
        sender = m_contact_manager->strangerById(senderId);
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

        QString path = m_historyManager->storeImage(image);
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
    data.setVersion(ConfigManager::dataStreamVersion());
    qsizetype size;
    data >> size;
    for(qsizetype i = 0; i < size; ++i)
    {
        QString id;
        QString name;
        QByteArray avatar;
        data >> id >> name >> avatar;
        QString path = m_historyManager->storeAvatar(name, avatar);
        User user(name, id, path);
        m_contact_manager->addStranger(id, user);
    }
}

void MainWindow::handle_privateFile(QDataStream &in)
{
    QString senderId, fileName, msgTime;
    QByteArray data;
    in >> senderId >> fileName >> data >> msgTime;
    QString filePath = m_historyManager->storeFile(fileName, data);
    // qDebug() << filePath;
    Message msg(Message::File, filePath, msgTime, m_contact_manager->friendById(senderId), senderId);
    addMessage_toList(msg);
}

void MainWindow::initTray()
{
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":/Icon/app.png")); // 设置托盘图标
    m_trayIcon->setToolTip("我的Qt应用");

    // 创建托盘菜单
    m_trayMenu = new QMenu(this);
    QAction *restoreAction = new QAction("显示窗口", this);
    QAction *quitAction = new QAction("退出", this);
    m_trayMenu->addAction(restoreAction);
    m_trayMenu->addAction(quitAction);

    m_trayIcon->setContextMenu(m_trayMenu);

    // 信号与槽
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);

    m_trayIcon->show(); // 显示托盘图标
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        showNormal(); // 单击还原
    }
}

void MainWindow::showSysMsg(const Message &msg)
{
    QString name = msg.getSenderName();
    QString text = msg.getPlainText();
    QString avatarPath = msg.getAvatarPath();


    QPixmap roundedPixmap = CommonUtil::getRoundPix(avatarPath);

    QPixmap pixmap = roundedPixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation); // 强制缩放为 48x48
    QIcon scaledIcon(pixmap);

    if(!avatarPath.isEmpty()){
        m_trayIcon->showMessage(name, text, scaledIcon, 2000);
    }else{
        m_trayIcon->showMessage(name, text, QSystemTrayIcon::Information, 2000);
    }
}

void MainWindow::showApplyRedDot()
{
    m_side_bar->showApplyRedDot();

    QLabel *redDot = ui->btn_applyList->findChild<QLabel*>("redDot");
    if(!redDot){
        // 创建一个 QLabel 当作红点
        redDot = new QLabel(ui->btn_applyList);
        redDot->setObjectName("redDot");
        redDot->setFixedSize(10, 10);
        redDot->move(205, 5); // 右上角

        // 设置红点样式
        redDot->setStyleSheet("background-color: red; border-radius: 5px;");
        redDot->raise();
        redDot->show();
    }else{
        redDot->raise();
        redDot->show();
    }
}

void MainWindow::hideApplyRedDot()
{
    m_side_bar->hideApplyRedDot();
    QLabel *redDot = ui->btn_applyList->findChild<QLabel*>("redDot");
    if(redDot){
        redDot->hide();
    }
}

void MainWindow::onSearchFrd(const QString &name)
{
    m_search_result_list->clear();
    const QVector<User> &friendList = m_contact_manager->friendList().values();
    for(auto it = friendList.begin(); it!=friendList.end(); ++it){
        if(it->getUserName().contains(name)){
            m_search_result_list->addItem(*it);
        }
    }
    ui->stackedList->setCurrentWidget(m_search_result_list);
}

void MainWindow::onSwitchView(int index)
{
    if (index >= 0 && index < ui->stackedList->count())
        ui->stackedList->setCurrentIndex(index);
    if (index >= 0 && index < ui->stackedWidget->count())
        ui->stackedWidget->setCurrentIndex(index);
}

void MainWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event); // 调用基类实现

    // showAvatar(m_user->get_avatarPath());
    m_side_bar->onSetAvatar(m_user->get_avatarPath());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_trayIcon->isVisible()) {
        if(ConfirmBox::question(this, "退出", "需要最小化？", "确定", "直接关闭")){
            hide();
            event->ignore(); // 忽略关闭事件
        }
    }
}

void MainWindow::receiveImage(QDataStream &in)
{
    QByteArray imageData;
    QString id;
    QString msgTime;
    in >> id>> imageData>>msgTime;

    QString imagePath = m_historyManager->storeImage(imageData);
    Message msg(Message::Image, imagePath, msgTime, m_contact_manager->friendById(id), id);
    addMessage_toList(msg);
}

void MainWindow::onClearModel()
{
    m_chat_model->clear();
    m_friends_model->clear();
}

void MainWindow::storeMessageToFile(const Message &msg) {
    //更新聊天列表的最新消息
    //todo 添加未读消息数量
    m_chat_model->updateLastMessage(msg.getChatId(), msg.getPlainText(), msg.getTime());

    Message message = msg;
    m_historyManager->addHistoryToFile(message);
}

void MainWindow::loadChatHistoryFromFile(QString targetId) {
    //更新好友信息
    if(!m_contact_manager->isInGroup(targetId)){
        ui->chatPartner->setText(m_contact_manager->friendById(targetId).getUserName());
        ui->friendState->setOnlineState(m_contact_manager->friendById(targetId).getOnlineState()?StateEnum::ONLINE:StateEnum::OFFLINE);
    }else{
        int row = m_chat_model->get_rowById(targetId);
        QString name = ui->chatList->model()->index(row, 0).data(ChatModel::UserNameRole).toString();
        ui->chatPartner->setText(QString(name + "（ %1 ）").arg(m_contact_manager->groupById(targetId).memberCount()));
        ui->friendState->setOnlineState(StateEnum::NONE);
    }

    ui->messageList->clear();
    m_historyManager->loadChatHistoryFromFile(targetId);
}

void MainWindow::addMessage_toList(const Message &message)
{
    QString chatId = message.getChatId();
    if(!this->isVisible()){
        showSysMsg(message);
    }
    //接收的消息和当前聊天对象是同一个才在窗口显示
    if(m_user->get_currentChatId()==chatId){
        ui->messageList->addMessage(message);
    }
    else m_chat_model->add_unreadMsgNum(chatId);
    // showMessage_toList(message);
    storeMessageToFile(message);
}

void MainWindow::handle_message(QDataStream &in)
{
    QString textMessage;
    QString id;
    QString msgTime;
    in>>id>>textMessage>>msgTime;
    // qDebug()<<id<<textMessage;

    Message msg(Message::Text, textMessage, msgTime, m_contact_manager->friendById(id), id);
    addMessage_toList(msg);
}

void MainWindow::handleUserList(QDataStream &in)
{
    QMap<QString,QString> id_name ;
    QMap<QString,QByteArray> id_avatar;
    in>>id_name>>id_avatar;
    m_contact_manager->updateUserList(id_name,id_avatar);
}

void MainWindow::handleFrdOnlineList(QDataStream &in)
{
    QSet<QString> onlineList;
    in>>onlineList;
    for(const auto &id :onlineList){
        if(m_contact_manager->isFriend(id)){
            m_contact_manager->setFriendState(id, true);
            //test
            m_friends_model->updateOnlineState(id, StateEnum::ONLINE);
        }
    }
    //更新当前聊天对象的在线状态
    QModelIndex index = ui->chatList->currentIndex();
    if(index.row()!=-1){
        QString id = index.data(FriendsModel::IdRole).toString();
        if(m_contact_manager->isFriend(id))
            ui->friendState->setOnlineState(m_contact_manager->friendById(id).getOnlineState()?StateEnum::ONLINE:StateEnum::OFFLINE);
    }
}

void MainWindow::handleFrdOnlineState(QDataStream &in)
{
    QString id;
    bool state;
    in >> id >> state;
    if(m_contact_manager->isFriend(id)){
        m_contact_manager->setFriendState(id, state);
        //test
        m_friends_model->updateOnlineState(id,state?StateEnum::ONLINE:StateEnum::OFFLINE);
    }

    //更新当前聊天对象的在线状态
    QModelIndex index = ui->chatList->currentIndex();
    if(index.row()!=-1){
        QString curId = index.data(FriendsModel::IdRole).toString();
        if(curId == id && m_contact_manager->isFriend(id))
            ui->friendState->setOnlineState(state?StateEnum::ONLINE:StateEnum::OFFLINE);
    }
}



