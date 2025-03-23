#include "mainwindow.h"

#include <QImageReader>

#include <QSqlDatabase>

#include "View/imageviewer.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    socket(new QTcpSocket(this)),
    m_avatarPath(""),
    m_sideBar_btnGroup(new QButtonGroup(this)),
    m_dataTransfer(new DataTransfer(this))
{

    ui->setupUi(this);
    setWindowTitle("AllChat");
    // 绑定按钮槽函数
    connect(ui->btnSend, &QPushButton::clicked, this, &MainWindow::onSendClicked);
    connect(ui->btnImage, &QPushButton::clicked, this, &MainWindow::sendImage);
    connect(ui->btnUpdateAvatar,&QPushButton::clicked, this,[=](){
        m_updateAvatar = new UpdateAvatar(this);
        connect(m_updateAvatar,&UpdateAvatar::send_updateAvatar,this,&MainWindow::send_updateAvatar);
        connect(m_updateAvatar,&UpdateAvatar::setAvatar,this,&MainWindow::setAvatar);
        m_updateAvatar->setAvatarPath(m_avatarPath);
        m_updateAvatar->exec();
    });

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(m_dataTransfer,&DataTransfer::handleData,this,&MainWindow::handleData);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);

    initMessageList();
    initFriendsList();
    initChatList();
    initAddFriends();
    initSideBar();
    initFriendApplyList();
}

MainWindow::~MainWindow() {
    delete ui;
    delete socket;
}

//todo 封装到messageListView里面
void MainWindow::initMessageList()
{
    message_model = new MessageModel(this);
    message_delegate = new MessageDelegate(this);

    /*使用listView在消息比较多的情况下内存低、可以流畅滚动
    用widget的话消息一多就比较占用内存*/
    ui->messageList->setModel(message_model);
    ui->messageList->setItemDelegate(message_delegate);
    ui->messageList->setVerticalScrollMode(QListView::ScrollPerPixel); // 平滑滚动
    ui->messageList->setResizeMode(QListView::Adjust);                 // 自动调整项大小
    // 设置ListView属性
    ui->messageList->setSpacing(message_delegate->getListViewSpacing());          // 项间距
    // ui->messageList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);// 始终显示垂直滚动条
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
    friends_delegate = new FriendsDelegate(this);

    ui->friendList->setModel(friends_model);
    ui->friendList->setItemDelegate(friends_delegate);

    ui->friendList->setVerticalScrollMode(QListView::ScrollPerPixel); // 平滑滚动
    ui->friendList->setResizeMode(QListView::Adjust);                 // 自动调整项大小
    ui->friendList->setSelectionMode(QAbstractItemView::SingleSelection);  // 单选
    ui->friendList->setSelectionBehavior(QAbstractItemView::SelectItems);

    // connect(ui->friendList, &QListView::clicked, this, [=](const QModelIndex &index){
    //     QString id = index.data(FriendsModel::IdRole).toString();
    //     loadChatHistoryFromFile(id);
    // });
}

void MainWindow::initChatList()
{
    chat_model = new ChatModel(this);
    chat_delegate = new ChatDelegate(this);

    ui->chatList->setModel(chat_model);
    ui->chatList->setItemDelegate(chat_delegate);

    ui->chatList->setVerticalScrollMode(QListView::ScrollPerPixel); // 平滑滚动
    ui->chatList->setResizeMode(QListView::Adjust);                 // 自动调整项大小
    ui->chatList->setSelectionMode(QAbstractItemView::SingleSelection);  // 单选
    ui->chatList->setSelectionBehavior(QAbstractItemView::SelectItems);

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
        QByteArray data = getPacket(CommonEnum::ADD_FRIEND,id);
        sendData(data);
    });
}

void MainWindow::initSideBar()
{
    m_sideBar_btnGroup->addButton(ui->switchMessageList,0);
    m_sideBar_btnGroup->addButton(ui->switchFriendsList,1);
    m_sideBar_btnGroup->addButton(ui->switchaddList,2);
    connect(m_sideBar_btnGroup,&QButtonGroup::buttonClicked,this,[=](QAbstractButton *button){
        int index = m_sideBar_btnGroup->id(button);
        ui->stackedList->setCurrentIndex(index);
    });

    ui->stackedList->setCurrentIndex(0);//默认显示聊天列表
    m_sideBar_btnGroup->button(0)->setChecked(true);
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

void MainWindow::initFriendApplyList()
{
    friendsApply_model = new FriendsModel(this);
    friendsApply_delegate = new ApplyDelegate(this);
    ui->friendApplyList->setModel(friendsApply_model);
    ui->friendApplyList->setItemDelegate(friendsApply_delegate);

    connect(friendsApply_delegate,&ApplyDelegate::applyResult,this,[=](const QString &id,const int &row){
        send_addFriend_result(id);
        friendsApply_model->removeItem(row);
    });
}

void MainWindow::handle_selectByName_reuslt(QDataStream &in)
{
    QMap<QString,QString> id_names;
    in>>id_names;
    //更新查询后的列表，后面考虑改为信号与槽
    add_friends.updateListView(id_names);
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
    in>>senderName>>senderId;
    qDebug()<<senderName<<senderId;
    friendsApply_model->addFriends_ToList(senderName,senderId,"请求添加好友","");
}

void MainWindow::send_addFriend_result(QString id)
{
    QByteArray data = getPacket(CommonEnum::AGREE_FRIEND,id);
    sendData(data);
}

void MainWindow::handle_addFriend_result(QDataStream &in)
{
    QString senderName,senderId;
    in>>senderName>>senderId;
    if(ui->chatList->currentIndex().data(FriendsModel::IdRole).toString()==senderId)//接收的消息和当前聊天对象是同一个才在窗口显示
        addMessage_toList("我们已成功添加好友，现在可以开始聊天啦~",senderId,m_userId,getCurrentTime());
    storeMessageToFile(senderId,m_friendList[senderId].userName,"我们已成功添加好友，现在可以开始聊天啦~",getCurrentTime());
}

void MainWindow::ConnectServer() {
    //clash开着的时候局域网ip无法连接到服务器
    QString host = "127.0.0.1"; // 服务器 IP
    quint16 port = 12345; // 服务器端口号

    socket->connectToHost(host, port); // 连接到服务器

    if (!socket->waitForConnected(3000)) {
        QMessageBox::critical(this, "Error", "连接服务器失败!");
    } else {
        qDebug()<<"已经成功连接服务器.";

    }
}

void MainWindow::registerUser(const QString &username, const QString &password) {
    if (username.isEmpty() || password.isEmpty()) return;
    // sendData(CommonEnum::message_type::REGISTER,username,password);
    QByteArray data = getPacket(CommonEnum::message_type::REGISTER,username,password);
    sendData(data);
}

void MainWindow::loginUser(const QString &username, const QString &password) {
    this->m_userName = username;
    ui->statusBar->showMessage("服务器IP：127.0.0.1 服务器端口：12345 用户名："+m_userName);

    // sendData(CommonEnum::message_type::LOGIN,username,password);
    QByteArray data = getPacket(CommonEnum::message_type::LOGIN,username,password);
    sendData(data);
    qDebug()<<"登录";

}

void MainWindow::send_slelectByName(const QString &username)
{
    if(!username.isEmpty()){
        // sendData(CommonEnum::FIND_NEW_FRIEND,username);
        QByteArray data = getPacket(CommonEnum::FIND_NEW_FRIEND,username);
        sendData(data);
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

    QByteArray packet = getPacket(CommonEnum::UPDATE_AVATAR,imageData);
    sendData(packet);
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
    storeImage(m_userName,imageData);
    m_avatarPath = path;
    showAvatar(m_avatarPath);
    update();//通知重新绘制头像
}

void MainWindow::onReadyRead() {
    m_dataTransfer->receiveData(socket);
}

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

QString MainWindow::getChatHistoryFilePath() {
    // 获取项目根目录
    QString projectRootPath = QCoreApplication::applicationDirPath();

    // 确保聊天记录的目录存在
    QDir dir(projectRootPath + "/chat_history");
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return dir.path();
}

void MainWindow::storeMessageToFile(const QString &targetId, const QString &sender, const QString &message, const QString &msgTime) {
    //更新聊天列表的最新消息
    //todo 添加未读消息数量
    chat_model->updateLastMessage(targetId,message,msgTime);

    QString filePath = getChatHistoryFilePath();
    filePath+=QString("/%1_%2.txt").arg(targetId).arg(m_userName);
    // qDebug()<<filePath;
    QFile file(filePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        //以json格式存储，适合结构化数据
        QJsonObject obj;
        obj["name"] = sender;
        obj["message"] = message;
        obj["time"] = msgTime;
        QJsonObject format;
        format["kinds"] = "text";
        format["data"] = obj;
        QJsonDocument doc(format);
        QTextStream out(&file);
        out << doc.toJson(QJsonDocument::Compact) << Qt::endl;
    }
}

QString MainWindow::storeImageToFile(const QString &targetId, const QString &sender,const QByteArray &imageData, const QString &msgTime){
    chat_model->updateLastMessage(targetId,"图片",msgTime);

    QString filePath = getChatHistoryFilePath();
    QString textFilePath = QString("%1/%2_%3.txt").arg(filePath).arg(targetId).arg(m_userName);

    QString imageFilePath = storeImage("",imageData);

    QFile file(textFilePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        //以json格式存储，适合结构化数据
        QJsonObject obj;
        obj["name"] = sender;
        obj["imagePath"] = imageFilePath;
        obj["time"] = msgTime;
        QJsonObject format;
        format["kinds"] = "image";
        format["data"] = obj;
        QJsonDocument doc(format);
        QTextStream out(&file);
        out << doc.toJson(QJsonDocument::Compact) << Qt::endl;
    }
    return imageFilePath;
}

QString MainWindow::storeImage(QString imageName, const QByteArray &imageData)
{
    QString filePath = getChatHistoryFilePath();

    QImage image;
    image.loadFromData(imageData);
    QBuffer buffer;
    buffer.setData(imageData);
    buffer.open(QIODevice::ReadOnly);  // 以只读模式打开
    QImageReader reader(&buffer);
    reader.setDecideFormatFromContent(true);
    QString format = QString(reader.format());//读取图片后缀名

    QDir dir(QString("%1/%2").arg(filePath).arg("image"));
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    //如果名字为空就用uid，有的话就是头像
    //todo 可以给图片分类，根据类别命名
    imageName = imageName.isEmpty()?QUuid::createUuid().toString(QUuid::WithoutBraces):imageName.append("_avatar");
    QString imageFilePath = QString("%1/%2/%3.%4").arg(filePath).arg("image").arg(imageName).arg(format);
    image.save(imageFilePath);
    return imageFilePath;
}

QPair<QString, QString> MainWindow::getLastMessage(const QString &targetId)
{
    QString filePath = getChatHistoryFilePath();//因为多个个客户端会运行在同一台机器上，需要接收端id加发送端用户名来作为文件名
        filePath+=QString("/%1_%2.txt").arg(targetId).arg(m_userName);
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // qDebug() << "没有聊天记录";
            return {"",""};
        }
        QString lastLine;
        QTextStream in(&file);
        while (!in.atEnd()) {
            lastLine = in.readLine();
        }
        file.close();
        QJsonDocument doc = QJsonDocument::fromJson(lastLine.toUtf8());
        QJsonObject format = doc.object();
        QJsonObject obj = format["data"].toObject();
        // 此时 lastLine 就是文件的最后一行文本
        // qDebug() << "最后一行:" << lastLine;
        QString lastMessage;
        QString lastMessageTime = obj["time"].toString();
        if(format["kinds"].toString()=="text"){
            lastMessage = obj["message"].toString();
        }else if(format["kinds"].toString()=="image"){
            lastMessage = "图片";
        }

        return {lastMessage,lastMessageTime};
}

void MainWindow::loadChatHistoryFromFile(QString targetId) {
    //更新好友信息
    ui->chatPartner->setText(m_friendList[targetId].userName);
    ui->friendState->setOnlineState(m_friendList[targetId].state?StateEnum::ONLINE:StateEnum::OFFLINE);

    QString filePath = getChatHistoryFilePath();//因为多个个客户端会运行在同一台机器上，需要接收端id加发送端用户名来作为文件名
    filePath+=QString("/%1_%2.txt").arg(targetId).arg(m_userName);
    QFile file(filePath);
    message_model->clear();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        // QString lastTime = "";//用于比较消息间的时间差距
        while (!in.atEnd()) {
            QString line = in.readLine();
            QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8());
            QJsonObject format = doc.object();
            QJsonObject obj = format["data"].toObject();

            bool isOutgoing = obj["name"].toString()==m_userName;
            QString senderId = isOutgoing?m_userId:targetId;
            QString time = obj["time"].toString();
            if(format["kinds"].toString()=="text"){
                QString message = obj["message"].toString();
                addMessage_toList(message, targetId, senderId, time);
            }else if(format["kinds"].toString()=="image"){
                QString imagePath = obj["imagePath"].toString();
                addImage_toList(imagePath, targetId, senderId, time);
            }

        }
        file.close();
    }
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
    bool isOutgoing = (m_userId==senderId);
    QString userName = isOutgoing?m_userName:m_friendList[chatId].userName;
    QString avatarPath = isOutgoing?m_avatarPath:m_friendList[chatId].avatarPath;
    message_model->addTextMessage(text,isOutgoing,userName,avatarPath,time);
    ui->messageList->scrollToBottom(); // 自动滚动到底部
}

void MainWindow::addImage_toList(const QString &imagePath,const QString &chatId,const QString &senderId,const QString &time)
{
    //因为最新的聊天消息和时间都存在聊天对象里面，并且需要判断是谁发送的消息，所以需要两个id
    //把自己和对方的最新消息都存在对方里
    addTime_toList(chatId,time);
    bool isOutgoing = (m_userId==senderId);
    QString userName = isOutgoing?m_userName:m_friendList[chatId].userName;
    QString avatarPath = isOutgoing?m_avatarPath:m_friendList[chatId].avatarPath;
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
        QPair<QString,QString> lastMessage = getLastMessage(userId);
        QString avatarPath = new_idAvatar[userId].size()?storeImage(newUserList[userId],new_idAvatar[userId]):"";
        chat_model->addChat_toList(newUserList[userId],
                                   userId,
                                   lastMessage.first,
                                   lastMessage.second,
                                   0,
                                   avatarPath);
    }

    const auto &keys = m_friendList.keys();
    for(auto &it:keys){
        m_friendList[it].avatarPath = new_idAvatar[it].size()?storeImage(m_friendList[it].userName,new_idAvatar[it]):"";
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
            if(m_friendList.contains(it))
                m_friendList[it].state=onlineList.contains(it);
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
    in >>m_userId>> imageData;
    if(imageData.isEmpty()) return;
    // qDebug()<<"存入头像"<<imageData.size();
    QString avatarPath = storeImage(m_userName,imageData);
    m_avatarPath = avatarPath;
    showAvatar(m_avatarPath);
}

void MainWindow::onDisconnected() {//断开连接的槽函数
    qDebug()<<"和服务器断开连接";
}

void MainWindow::onSendClicked() {//发送按钮的槽函数
    if(socket->state() != QTcpSocket::ConnectedState || ui->lineEditMessage->toPlainText().isEmpty())
        return;
    QString userId = ui->chatList->currentIndex().data(FriendsModel::IdRole).toString();
    if (!userId.isEmpty()) {
        qDebug() << "接收信息的用户id:" << userId;
    }else return;

    if(m_friendList[userId].userName != m_userName){
        QByteArray data = getPacket(CommonEnum::message_type::CHAT,userId,ui->lineEditMessage->toPlainText());
        sendData(data);
    }
    QString textMessage = ui->lineEditMessage->toPlainText();
    addMessage_toList(textMessage,userId,m_userId,getCurrentTime());
    storeMessageToFile(userId,m_userName,textMessage,getCurrentTime());//保存聊天记录在硬盘中
    ui->lineEditMessage->clear(); // 清空输入框

    //将当前聊天对象的选择设为排序后的第一个
    // if(ui->chatList->model()->rowCount() > 0){
    //     QModelIndex firstIndex = ui->chatList->model()->index(0, 0);
    //     ui->chatList->setCurrentIndex(firstIndex);
    // }
}

void MainWindow::sendImage() {//发送图片的槽函数
    QString userId = ui->chatList->currentIndex().data(FriendsModel::IdRole).toString();
    if (!userId.isEmpty()) {
        // qDebug() << "接收信息的用户id:" << userId;
    }else return;
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

    if(m_friendList[userId].userName != m_userName){
        // 数据包封装
        QByteArray packet = getPacket(CommonEnum::IMAGE,userId, imageData);
        sendData(packet);
    }

    addImage_toList(imagePath,userId,m_userId,getCurrentTime());
    storeImageToFile(userId,m_userName,imageData,getCurrentTime());
    socket->flush();

    // //将当前聊天对象的选择设为排序后的第一个
    // if(ui->chatList->model()->rowCount() > 0){
    //     QModelIndex firstIndex = ui->chatList->model()->index(0, 0);
    //     ui->chatList->setCurrentIndex(firstIndex);
    // }
}

template<typename... Args>
QByteArray MainWindow::getPacket(Args... args)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);

    // 使用 C++17 折叠表达式，将所有参数依次写入 stream
    (out << ... << args);

    return packet;
}

void MainWindow::sendData(QByteArray &packet)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out<<static_cast<qint32>(packet.size());
    out.writeRawData(packet.constData(),packet.size());
    socket->write(data);
}
