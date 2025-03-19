#include "mainwindow.h"

#include <QImageReader>

#include <QSqlDatabase>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    socket(new QTcpSocket(this)),
    m_avatarPath("")
{

    ui->setupUi(this);
    setWindowTitle("AllChat");

    // 绑定按钮槽函数
    connect(ui->btnSend, &QPushButton::clicked, this, &MainWindow::onSendClicked);
    connect(ui->btnImage, &QPushButton::clicked, this, &MainWindow::sendImage);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);

    initMessageList();
    initFriendsList();

    //添加好友

    connect(ui->addFriends,&QPushButton::clicked,this,[=](){
        add_friends.exec();
    });
    connect(&add_friends,&AddFriends::send_slelectByName,this,&MainWindow::send_slelectByName);
    connect(&add_friends,&AddFriends::sendData,this,[=](const QString &id){
        this->sendData(CommonEnum::ADD_FRIEND,id);
    });

    connect(ui->messageGroup,&QAction::triggered,this,[=](){
        ui->stackedList->setCurrentIndex(0);
        qDebug()<<ui->stackedList->currentIndex();
    });
    connect(ui->friendsGroup,&QAction::triggered,this,[=](){
        ui->stackedList->setCurrentIndex(1);
        qDebug()<<ui->stackedList->currentIndex();
    });
    ui->stackedList->setCurrentIndex(0);//默认显示聊天列表
    initFriendApplyList();
}

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

    ui->friendsList->setModel(friends_model);
    ui->friendsList->setItemDelegate(friends_delegate);

    ui->friendsList->setVerticalScrollMode(QListView::ScrollPerPixel); // 平滑滚动
    ui->friendsList->setResizeMode(QListView::Adjust);                 // 自动调整项大小
    ui->friendsList->setSelectionMode(QAbstractItemView::SingleSelection);  // 单选
    ui->friendsList->setSelectionBehavior(QAbstractItemView::SelectItems);

    connect(ui->friendsList, &QListView::clicked, this, [=](const QModelIndex &index){
        QString id = index.data(FriendsModel::IdRole).toString();
        loadChatHistoryFromFile(id);
    });
}

void MainWindow::initFriendApplyList()
{
    friendsApply_model = new FriendsModel(this);
    friendsApply_delegate = new ApplyDelegate(this);
    ui->friendApplyList->setModel(friendsApply_model);
    ui->friendApplyList->setItemDelegate(friendsApply_delegate);
    // friendsApply_model->addFriends_ToList("1","1","1");

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

void MainWindow::handle_addFriend(QDataStream &in)
{
    QString senderName,senderId;
    in>>senderName>>senderId;
    qDebug()<<senderName<<senderId;
    //test
    // QMessageBox::StandardButton reply;
    // reply = QMessageBox::question(this, "好友请求", "用户："+senderName+"请求添加好友，是否同意?",
    //                               QMessageBox::Yes | QMessageBox::No);

    // send_addFriend_result(reply == QMessageBox::Yes,senderId);
    friendsApply_model->addFriends_ToList(senderName,senderId,"请求添加好友");

}

void MainWindow::send_addFriend_result(QString id)
{
    sendData(CommonEnum::AGREE_FRIEND,id);
}

void MainWindow::handle_addFriend_result(QDataStream &in)
{
    QString senderName,senderId;
    in>>senderName>>senderId;
    //todo 提醒已经同意添加
    // friends_model->addFriends_ToList(senderName,senderId,"未实现");
    // userList[senderId] = senderName;
}


MainWindow::~MainWindow() {
    delete ui;
    delete socket;
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
    sendData(CommonEnum::message_type::REGISTER,username,password);
}

void MainWindow::loginUser(const QString &username, const QString &password) {
    this->m_userName = username;
    ui->statusBar->showMessage("服务器IP：127.0.0.1 服务器端口：12345 用户名："+m_userName);

    sendData(CommonEnum::message_type::LOGIN,username,password);
    qDebug()<<"登录";

}

void MainWindow::send_slelectByName(const QString &username)
{
    if(!username.isEmpty())
        sendData(CommonEnum::FIND_NEW_FRIEND,username);
}

void MainWindow::onReadyRead() {
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_15);

    while (!in.atEnd()) {
        if(currentReceivingState == WaitingForHeader)
            in >> messageType;
        qDebug()<<messageType;
        switch(messageType){
        case CommonEnum::IMAGE:{
            //接收图片
            receiveImage(in);
        }break;
        case CommonEnum::USER_LIST:{
            handle_userList(in);
            qDebug()<<"用户列表更新";
        }break;
        case CommonEnum::CHAT:{
            handle_message(in);
        }break;
        case CommonEnum::LOGIN_SUCCESS:
        case CommonEnum::LOGIN_FAILED:{
            if(currentReceivingState == WaitingForHeader)
                emit loginResult(messageType);
            if(messageType == CommonEnum::LOGIN_SUCCESS){
                handle_userAvatar(in);
            }
        }break;
        case CommonEnum::REGISTER_SUCCESS:
        case CommonEnum::REGISTER_FAILED:{
            emit registResult(messageType);
        }break;
        case CommonEnum::ADD_FRIEND:{
            handle_addFriend(in);
        }break;
        case CommonEnum::AGREE_FRIEND:{
            handle_addFriend_result(in);
        }break;
        case CommonEnum::NEW_FRIEND_REULT:{
            handle_selectByName_reuslt(in);
        }break;
        case CommonEnum::ONLINE_LIST:{
            handle_onlineFriendsList(in);
        }break;
        default:qDebug() << "未知消息类型!";break;
        }

    }
}

void MainWindow::receiveImage(QDataStream &in)
{
    //处理图片接收
    if (currentReceivingState == WaitingForHeader) {
        if (socket->bytesAvailable() < static_cast<qint64>(sizeof(qint32) * 2)) return; // 数据不足，等待下次读取
        in >> currentDataLength;
        // qDebug()<<messageType << currentDataLength;
        if (currentDataLength <= 0 || currentDataLength > 10 * 1024 * 1024) {
            qWarning() << "Invalid data length:" << currentDataLength;
            resetState();
            return;
        }
        dataBuffer.clear();
        dataBuffer.reserve(currentDataLength);
        currentReceivingState = ReceivingData;
    }
    if (currentReceivingState == ReceivingData) {
        int bytesToRead = qMin(socket->bytesAvailable(), currentDataLength - receivedBytes);
        QByteArray chunk = socket->read(bytesToRead);
        dataBuffer.append(chunk);
        receivedBytes += chunk.size();
        if (receivedBytes == currentDataLength) {
            // qDebug()<<messageType;
            // qDebug()<<dataBuffer.size();
            QDataStream stream(dataBuffer);
            QByteArray imageData;
            QString id;
            stream >> id>> imageData;
            // qDebug()<<"来自："<<id<<imageData.size();

            QString imagePath = storeImageToFile(id,m_userList[id].userName,imageData);
            if(ui->friendsList->currentIndex().data(FriendsModel::IdRole).toString()==id)
                addImage_toList(imagePath,m_userList[id].userName==m_userName,m_userList[id].userName,m_userList[id].userName==m_userName?m_avatarPath:m_userList[id].avatarPath);
            resetState();
        }
    }
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

void MainWindow::storeMessageToFile(const QString &targetId, const QString &sender, const QString &message) {
    QString filePath = getChatHistoryFilePath();
    filePath+=QString("/%1_%2.txt").arg(targetId).arg(m_userName);
    qDebug()<<filePath;
    QFile file(filePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        //以json格式存储，适合结构化数据
        QJsonObject obj;
        obj["name"] = sender;
        obj["message"] = message;
        obj["time"] = QTime::currentTime().toString();
        QJsonObject format;
        format["kinds"] = "text";
        format["data"] = obj;
        QJsonDocument doc(format);
        QTextStream out(&file);
        out << doc.toJson(QJsonDocument::Compact) << Qt::endl;
    }
}

QString MainWindow::storeImageToFile(const QString &targetId, const QString &sender,const QByteArray &imageData)
{
    QString filePath = getChatHistoryFilePath();
    QString textFilePath = QString("%1/%2_%3.txt").arg(filePath).arg(targetId).arg(m_userName);

    // QImage image;
    // image.loadFromData(imageData);
    // QBuffer buffer;
    // buffer.setData(imageData);
    // buffer.open(QIODevice::ReadOnly);  // 以只读模式打开
    // QImageReader reader(&buffer);
    // reader.setDecideFormatFromContent(true);
    // QString format = QString(reader.format());
    // QDir dir(QString("%1/%2").arg(filePath).arg("image"));
    // if (!dir.exists()) {
    //     dir.mkpath(".");
    // }
    // QString imageName = QUuid::createUuid().toString(QUuid::WithoutBraces);
    // QString imageFilePath = QString("%1/%2/%3.%4").arg(filePath).arg("image").arg(imageName).arg(format);
    // image.save(imageFilePath);
    QString imageFilePath = storeImage("",imageData);

    //todo 在文字消息记录中把图片路径存进去，之后使用图片全部都用路径，避免直接传递图片占用内存
    QFile file(textFilePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        //以json格式存储，适合结构化数据
        QJsonObject obj;
        obj["name"] = sender;
        obj["imagePath"] = imageFilePath;
        obj["time"] = QTime::currentTime().toString();
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

void MainWindow::loadChatHistoryFromFile(QString targetId) {
    //更新好友信息
    ui->chatPartner->setText(m_userList[targetId].userName);
    ui->friendState->setText(m_userList[targetId].state?"在线":"离线");

    QString filePath = getChatHistoryFilePath();//因为多个个客户端会运行在同一台机器上，需要接收端id加发送端用户名来作为文件名
    filePath+=QString("/%1_%2.txt").arg(targetId).arg(m_userName);
    QFile file(filePath);
    message_model->clear();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8());
            QJsonObject format = doc.object();
            QJsonObject obj = format["data"].toObject();
            bool isOutgoing = obj["name"].toString()==m_userName;
            if(format["kinds"].toString()=="text"){
                addMessage_toList( obj["message"].toString(),isOutgoing,obj["name"].toString(),isOutgoing?m_avatarPath:m_userList[targetId].avatarPath);
            }else if(format["kinds"].toString()=="image"){
                addImage_toList(obj["imagePath"].toString(),isOutgoing,obj["name"].toString(),isOutgoing?m_avatarPath:m_userList[targetId].avatarPath);
            }

        }
        file.close();
    }
}

void MainWindow::handle_message(QDataStream &in)
{
    QString textMessage;
    QString id;
    in >> id>> textMessage;
    if(ui->friendsList->currentIndex().data(FriendsModel::IdRole).toString()==id)//接收的消息和当前聊天对象是同一个才在窗口显示
        addMessage_toList(textMessage,m_userList[id].userName==m_userName,m_userList[id].userName,m_userList[id].userName==m_userName?m_avatarPath:m_userList[id].avatarPath);
    storeMessageToFile(id,m_userList[id].userName,textMessage);
}



void MainWindow::addMessage_toList(const QString &text,const bool &isOutgoing,const QString &userName,const QString &avatarPath)
{
    message_model->addTextMessage(text,isOutgoing,userName,avatarPath);
    ui->messageList->scrollToBottom(); // 自动滚动到底部
}

void MainWindow::addImage_toList(const QString &imagePath,const bool &isOutgoing,const QString &userName,const QString &avatarPath)
{
    message_model->addImageMessage(imagePath,isOutgoing,userName,avatarPath);
    ui->messageList->scrollToBottom(); // 自动滚动到底部
}




void MainWindow::updateUserList(const QMap<QString, QString> &newUserList,const QMap<QString,QByteArray> &new_idAvatar) {

    QSet<QString> newUserSet;
    const auto &newKeys = newUserList.keys();
    for(auto &it:newKeys)
        newUserSet<<it;
    QSet<QString> currentUserSet;
    const auto &ctKeys = m_userList.keys();
    for(auto &it:ctKeys)
        currentUserSet<<it;
    // 计算需要删除的用户
    QSet<QString> usersToRemove = currentUserSet - newUserSet;
    // 计算新增的用户
    QSet<QString> usersToAdd = newUserSet - currentUserSet;

    // 删除用户
    for (const QString &userId : usersToRemove) {
        m_userList.remove(userId);
        for (int i = 0; i < friends_model->rowCount(); ++i) {
            QString id = friends_model->data(friends_model->index(i,0),FriendsModel::IdRole).toString();
            if (id == userId) {
                friends_model->removeItem(i);
                break;
            }
        }
    }

    // 添加新用户
    for (const QString &userId : usersToAdd) {
        m_userList[userId].userName = newUserList[userId];
        friends_model->addFriends_ToList(newUserList[userId],userId,"还没实现");
        qDebug()<<"添加了"<<userId;
    }

    const auto &keys = m_userList.keys();
    for(auto &it:keys){
        m_userList[it].avatarPath = storeImage(m_userList[it].userName,new_idAvatar[it]);
    }

    // 默认选择第一行
    if(ui->friendsList->model()->rowCount() > 0){
        QModelIndex firstIndex = ui->friendsList->model()->index(0, 0);
        ui->friendsList->setCurrentIndex(firstIndex);
        QString id = ui->friendsList->currentIndex().data(FriendsModel::IdRole).toString();
        loadChatHistoryFromFile(id);
        ui->friendsList->selectionModel()->select(firstIndex, QItemSelectionModel::Select);
    }

}

void MainWindow::handle_userList(QDataStream &in)
{
    if (currentReceivingState == WaitingForHeader) {
        if (socket->bytesAvailable() < static_cast<qint64>(sizeof(qint32) * 2)) return; // 数据不足，等待下次读取
        in >> currentDataLength;
        qDebug()<<messageType << currentDataLength;
        if (currentDataLength <= 0 || currentDataLength > 10 * 1024 * 1024) {
            qWarning() << "Invalid data length:" << currentDataLength;
            resetState();
            return;
        }
        dataBuffer.clear();
        dataBuffer.reserve(currentDataLength);
        currentReceivingState = ReceivingData;
    }
    if (currentReceivingState == ReceivingData) {
        int bytesToRead = qMin(socket->bytesAvailable(), currentDataLength - receivedBytes);
        QByteArray chunk = socket->read(bytesToRead);
        dataBuffer.append(chunk);
        receivedBytes += chunk.size();
        if (receivedBytes == currentDataLength) {
            QDataStream stream(dataBuffer);
            QMap<QString,QString> id_name ;
            QMap<QString,QByteArray> id_avatar;
            stream>>id_name>>id_avatar;
            updateUserList(id_name,id_avatar);
            resetState();
        }
    }
}

void MainWindow::handle_onlineFriendsList(QDataStream &in)
{
    QSet<QString> onlineList;
    in>>onlineList;
    const auto userIdList = m_userList.keys();
    for(auto &it :userIdList){
        m_userList[it].state=onlineList.contains(it);
    }
    //更新当前聊天对象的在线状态
    QModelIndex index = ui->friendsList->currentIndex();
    QString id = index.data(FriendsModel::IdRole).toString();
    ui->friendState->setText(m_userList[id].state?"在线":"离线");
}

void MainWindow::handle_userAvatar(QDataStream &in)
{
    if (currentReceivingState == WaitingForHeader) {
        if (socket->bytesAvailable() < static_cast<qint64>(sizeof(qint32) * 2)) return; // 数据不足，等待下次读取
        in >> currentDataLength;
        qDebug()<<messageType << currentDataLength;
        if (currentDataLength <= 0 || currentDataLength > 10 * 1024 * 1024) {
            qWarning() << "Invalid data length:" << currentDataLength;
            resetState();
            return;
        }
        dataBuffer.clear();
        dataBuffer.reserve(currentDataLength);
        currentReceivingState = ReceivingData;
    }
    if (currentReceivingState == ReceivingData) {
        int bytesToRead = qMin(socket->bytesAvailable(), currentDataLength - receivedBytes);
        QByteArray chunk = socket->read(bytesToRead);
        dataBuffer.append(chunk);
        receivedBytes += chunk.size();
        if (receivedBytes == currentDataLength) {
            QDataStream stream(dataBuffer);
            // QByteArray imageData;
            // stream >> imageData;

            qDebug()<<"存入头像"<<dataBuffer.size();
            QString avatarPath = storeImage(m_userName,dataBuffer);
            m_avatarPath = avatarPath;
            // m_avatar.loadFromData(imageData);
            // storeImageToFile("1","1",dataBuffer);
            resetState();
        }
    }
}


void MainWindow::resetState() {//重置连续传输的变量状态
    currentReceivingState = WaitingForHeader;
    // messageType = "";
    currentDataLength = 0;
    receivedBytes = 0;
    dataBuffer.clear();
}

void MainWindow::onDisconnected() {//断开连接的槽函数
    qDebug()<<"和服务器断开连接";
}

void MainWindow::onSendClicked() {//发送按钮的槽函数
    if(socket->state() != QTcpSocket::ConnectedState || ui->lineEditMessage->toPlainText().isEmpty())
        return;
    QString userId = ui->friendsList->currentIndex().data(FriendsModel::IdRole).toString();
    if (!userId.isEmpty()) {
        qDebug() << "接收信息的用户id:" << userId;
    }else return;

    if(m_userList[userId].userName != m_userName){
        sendData(CommonEnum::message_type::CHAT,userId,ui->lineEditMessage->toPlainText());
    }
    QString textMessage = ui->lineEditMessage->toPlainText();
    addMessage_toList(textMessage,true,m_userName,m_avatarPath);
    // storeMessage(userId,userName,ui->lineEditMessage->text());//保存聊天记录在内存中
    storeMessageToFile(userId,m_userName,textMessage);//保存聊天记录在硬盘中
    ui->lineEditMessage->clear(); // 清空输入框
}

void MainWindow::sendImage() {//发送图片的槽函数
    QString userId = ui->friendsList->currentIndex().data(FriendsModel::IdRole).toString();
    if (!userId.isEmpty()) {
        qDebug() << "接收信息的用户id:" << userId;
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

    if(m_userList[userId].userName != m_userName){
        // 数据包封装
        QByteArray packet;
        QDataStream stream(&packet, QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Qt_5_15);

        // qDebug()<<imageData.size();
        stream << userId<< imageData;
        QByteArray packet1;
        QDataStream out(&packet1, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_15);
        out<<CommonEnum::IMAGE<<static_cast<qint32>(packet.size());
        out.writeRawData(packet.constData(), packet.size()); // 图片数据
        socket->write(packet1);
    }

    addImage_toList(imagePath,true,m_userName,m_avatarPath);
    storeImageToFile(userId,m_userName,imageData);
    socket->flush();
}

template<typename... Args>
void MainWindow::sendData(Args... args)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);

    // 使用 C++17 折叠表达式，将所有参数依次写入 stream
    (out << ... << args);

    socket->write(packet);
}
