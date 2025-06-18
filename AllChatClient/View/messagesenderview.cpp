#include "messagesenderview.h"
#include "Utils/CustomTypes.h"
#include <QFileDialog>
#include <QShortcut>
#include <qboxlayout.h>

MessageSenderView::MessageSenderView(QWidget *parent, ChatHistoryManager* historyManager)
    : QWidget{parent},
    m_topBtnSize(25),
    m_btnFile(new QPushButton(this)),
    m_btnImage(new QPushButton(this)),
    m_btnSend(new QPushButton(this)),
    m_msgEdit(new QPlainTextEdit(this)),
    m_user(CurrentUser::getInstance()),
    m_dataTransfer(DataTransfer::getInstance()),
    m_historyManager(historyManager),
    m_friendList(m_user->getFriendList())
{
    initUI();

    this->installEventFilter(this);
    this->setMaximumHeight(200);

    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return), this);
    connect(shortcut, &QShortcut::activated, this, &MessageSenderView::onSendClicked);
    connect(m_btnFile, &QPushButton::clicked, this, &MessageSenderView::sendFile);
    connect(m_btnImage, &QPushButton::clicked, this, &MessageSenderView::sendImage);
    connect(m_btnSend, &QPushButton::clicked, this, &MessageSenderView::onSendClicked);
}

bool MessageSenderView::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this && event->type() == QEvent::MouseButtonPress) {
        m_msgEdit->setFocus();  // 点击空白区域时，让 plainTextEdit 获得焦点
    }
    return QWidget::eventFilter(watched, event);
}

void MessageSenderView::initUI()
{
    m_btnImage->setFixedSize(m_topBtnSize, m_topBtnSize);
    m_btnImage->setStyleSheet(R"(
    /**正常情况下样式**/
    QPushButton{
        border: none;
        background: transparent;
        border-image: url(:/Icon/images_black.png);
    }

    /**鼠标停留在按钮上的样式**/
    QPushButton::hover{
        border-image: url(:/Icon/images_blue.png);
    }
    )");
    m_btnFile->setFixedSize(m_topBtnSize, m_topBtnSize);
    m_btnFile->setStyleSheet(R"(
    /**正常情况下样式**/
    QPushButton{
        border: none;
        background: transparent;
        border-image: url(:/Icon/file.png);
    }

    /**鼠标停留在按钮上的样式**/
    QPushButton::hover{
        border-image: url(:/Icon/file_hover.png);
    }
    )");
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(m_btnImage);
    topLayout->addWidget(m_btnFile);
    topLayout->addStretch();
    topLayout->setContentsMargins(0,0,0,0);


    m_btnSend->setStyleSheet(R"(
    /**正常情况下样式**/
    QPushButton{
        font: 10pt "微软雅黑";
        color: white;
        background-color: rgb(53, 93, 144);
        color: rgb(255, 255, 255);
        border-radius: 8px;
        border-style: none;
        border-width: 1px;
        padding: 5px;
    }

    /**鼠标停留在按钮上的样式**/
    QPushButton::hover{
        color: #FFFFFF;
        background-color: rgb(93, 125, 166);
        border-color: #2f3640;
    }

    /**鼠标按压下去的样式**/
    QPushButton::pressed,QPushButton::checked{
        color: #FFFFFF;
        background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #273c75, stop:1 #487eb0);
    }

    /**按钮失能情况下样式**/
    QPushButton::disabled{
        color: #FFFFFF;
        background-color: #dcdde1;
        border-color: #dcdde1;
    }
    )");
    m_btnSend->setText("发送");
    QHBoxLayout* btmLayout = new QHBoxLayout();
    btmLayout->addStretch();
    btmLayout->addWidget(m_btnSend);
    btmLayout->setContentsMargins(0,0,0,0);

    m_msgEdit->setStyleSheet(R"(
    QPlainTextEdit{
    background-color: rgb(241, 241, 241);
    border:none;
    }
    QScrollBar:vertical {
        border: none;
        background: #f0f0f0;
        width: 10px;
        margin: 0px 0px 0px 0px;
    }
    QScrollBar::handle:vertical {
        background: #888;
        min-height: 20px;
        border-radius: 5px;
    }

    QScrollBar::handle:vertical:hover {
        background: #666;
    }

    QScrollBar::handle:vertical:pressed {
        background: #444;
    }

    QScrollBar::add-line:vertical,
    QScrollBar::sub-line:vertical {
        background: none;
        border: none;
    }

    QScrollBar::add-page:vertical,
    QScrollBar::sub-page:vertical {
        background: none;
    }
    )");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addLayout(topLayout);
    layout->addWidget(m_msgEdit);
    layout->addLayout(btmLayout);
}

void MessageSenderView::onSendClicked() {//发送按钮的槽函数
    if(m_msgEdit->toPlainText().isEmpty())
        return;
    QString chatId = m_user->get_currentChatId();
    if (!chatId.isEmpty()) {
        qDebug() << "当前聊天对象id:" << chatId;
    }else return;

    Packet data;
    if(m_friendList.contains(chatId)){
        data = Packet(CommonEnum::CHAT, chatId, m_msgEdit->toPlainText());
    }else{
        QString type("TEXT");
        data = Packet(CommonEnum::GROUP_CHAT, chatId, type, m_msgEdit->toPlainText());
    }
    m_dataTransfer->sendData(data);

    QString textMessage = m_msgEdit->toPlainText();
    Message msg(Message::Text, textMessage, Message::getCurrentTime(), m_user->toUser(), chatId);
    emit sendMsg(msg);
    m_msgEdit->clear(); // 清空输入框
}

void MessageSenderView::sendImage() {//发送图片的槽函数
    QString chatId = m_user->get_currentChatId();
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


    imagePath = m_historyManager->storeImage("", imageData);
    Message msg(Message::Image, imagePath, Message::getCurrentTime(), m_user->toUser(), chatId);
    emit sendMsg(msg);

}

void MessageSenderView::sendFile()
{
    QString chatId = m_user->get_currentChatId();
    if (chatId.isEmpty())
        return;
    qDebug() << "发送文件 接收对象：" << chatId;
    QString filePath = QFileDialog::getOpenFileName(this, "Select File", "", "All Files (*.*)");
    if(filePath.isEmpty())
        return;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件";
        return;
    }

    QFileInfo fileinfo(filePath);
    QString fileName = fileinfo.fileName();
    QByteArray fileData = file.readAll();
    file.close();

    Packet data(CommonEnum::PRIVATE_FILE, chatId, fileName, fileData);
    m_dataTransfer->sendData(data);

    QString path = m_historyManager->storeFile(fileName, fileData);
    Message msg(Message::File, path, Message::getCurrentTime(), m_user->toUser(), chatId);
    emit sendMsg(msg);
}


