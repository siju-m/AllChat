#include "chathistorymanager.h"

#include <QBuffer>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QImageReader>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>

ChatHistoryManager::ChatHistoryManager(QObject *parent)
    : QObject{parent},
    m_user(CurrentUser::getInstance())
{}

void ChatHistoryManager::initDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(getChatHistoryFilePath() + "/history.db");

    if (!db.open()) {
        qWarning() << "数据库打开失败：" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    // 创建表 messages
    QString createTable = "CREATE TABLE IF NOT EXISTS messages ("
                          "message_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                          "conversation_id TEXT NOT NULL, "
                          "sender_id TEXT NOT NULL, "
                          "content TEXT, "
                          "message_type TEXT CHECK(message_type IN ('text', 'image', 'file')) NOT NULL DEFAULT 'text', "
                          "status TEXT CHECK(status IN ('sent', 'delivered', 'read', 'deleted')) NOT NULL DEFAULT 'sent', "
                          "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP "
                          ");";
    if (!query.exec(createTable)) {
        qWarning() << "创建聊天记录表失败：" << query.lastError().text();
    }
}

QString ChatHistoryManager::getChatHistoryFilePath() {
    // 获取项目根目录
    QString projectRootPath = QCoreApplication::applicationDirPath();

    // 确保聊天记录的目录存在
    QDir dir(projectRootPath + "/chat_history/"+m_user->get_userId());
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return dir.path();
}

QString ChatHistoryManager::getChatFilePath(QString chatId)
{
    QString filePath = getChatHistoryFilePath();
    if(!m_user->getGroupsIdList().contains(chatId))
    {
        filePath += "/Private";
    }
    else
    {
        filePath += "/Group";
    }
    QDir dir(filePath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return dir.path();
}

void ChatHistoryManager::addHistoryToFile(Message &msg)
{
    QSqlQuery query;
    query.prepare("INSERT INTO messages (conversation_id, sender_id, content, message_type, timestamp) "
                  "VALUES (:conversation_id, :sender_id, :content, :message_type, :timestamp)");
    query.bindValue(":conversation_id", msg.getChatId());
    query.bindValue(":sender_id", msg.getSenderId());
    query.bindValue(":content", msg.getContent());
    query.bindValue(":message_type", msg.getType_string());
    query.bindValue(":timestamp", msg.getDateTime());

    if (!query.exec()) {
        qWarning() << "消息添加失败：" << query.lastError().text();
    }
}

QString ChatHistoryManager::storeImage(QString imageName, const QByteArray &imageData)
{
    if(imageData.isEmpty()) return QString();
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

QString ChatHistoryManager::storeFile(QString fileName, const QByteArray &fileData)
{
    if(fileData.isEmpty()) return QString();
    QString filePath = getChatHistoryFilePath();

    QDir dir(filePath += "/File");
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    filePath += ("/"+fileName);

    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    file.write(fileData);
    return filePath;
}

void ChatHistoryManager::loadChatHistoryFromFile(QString targetId)
{
    QSqlQuery query;
    qulonglong messageId;
    if(m_last_messageId.first.isEmpty() || m_last_messageId.first != targetId)
    {
        // qDebug() << "重新加载";
        m_last_messageId.first = targetId;

        query.prepare("SELECT MAX(message_id) AS last_message_id FROM messages;");
        if(query.exec())
        {
            query.next();
            messageId = query.value(0).toULongLong()+1;
        }
        else
        {
            qDebug() << "最大messageId查询失败";
            return;
        }
    }
    else
    {
        // qDebug() << "继续加载";
        messageId = m_last_messageId.second;
    }

    query.prepare("SELECT *"
                  "FROM messages "
                  "WHERE conversation_id = :conversation_id "
                  "AND message_id < :message_id "
                  "ORDER BY message_id DESC "
                  "LIMIT 15;");
    query.bindValue(":conversation_id", targetId);
    query.bindValue(":message_id", messageId);

    if (query.exec())
    {
        QVector<Message> messages;
        while(query.next())
        {
            QString message = query.value(3).toString();
            QString senderId = query.value(2).toString();
            User sender;
            if(senderId == m_user->get_userId())
            {
                sender = m_user->toUser();
            }
            else if(m_user->getFriendList().contains(senderId))
            {
                sender = m_user->getFriendList()[senderId];
            }
            else if(m_user->getStrangerList().contains(senderId))
            {
                sender = m_user->getStrangerList()[senderId];
            }
            QString time = query.value(6).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

            QString type = query.value(4).toString();
            Message msg;
            if(type == "text")
            {
                msg = Message(Message::Text, message, time, sender, targetId);
            }
            else if(type == "image")
            {
                msg = Message(Message::Image, message, time, sender, targetId);
            }
            else if(type == "file")
            {
                msg = Message(Message::File, message, time, sender, targetId);
            }
            m_last_messageId.second = query.value(0).toULongLong();
            // qDebug() << m_last_messageId.second;
            messages << msg;
        }
        emit addMessage_toList(messages);
    }
    else
    {
        qWarning() << "聊天记录查询失败：" << query.lastError().text();
    }
}

QPair<QString, QString> ChatHistoryManager::getLastMessage(const QString &targetId)
{
    QSqlQuery query;
    qulonglong messageId;

    query.prepare("SELECT MAX(message_id) AS last_message_id FROM messages;");
    if(query.exec())
    {
        query.next();
        messageId = query.value(0).toULongLong()+1;
    }
    else
    {
        qDebug() << "最大messageId查询失败";
        return {};
    }
    query.prepare("SELECT *"
                  "FROM messages "
                  "WHERE conversation_id = :conversation_id "
                  "AND message_id < :message_id "
                  "ORDER BY message_id DESC "
                  "LIMIT 1;");
    query.bindValue(":conversation_id", targetId);
    query.bindValue(":message_id", messageId);

    if(query.exec())
    {
        if(query.next())
        {
            QString message = query.value(3).toString();
            QString time = query.value(6).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            return {message, time};
        }
    }
    else
    {
        qDebug() << "查询最新消息失败";
    }
    return {};
    // QString filePath = getChatFilePath(targetId);//因为多个个客户端会运行在同一台机器上，需要接收端id加发送端用户名来作为文件名
    // filePath+=QString("/%1_%2.txt").arg(targetId).arg(m_user->get_userName());
    // QFile file(filePath);
    // if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     // qDebug() << "没有聊天记录";
    //     return {"",""};
    // }
    // QString lastLine;
    // QTextStream in(&file);
    // while (!in.atEnd()) {
    //     lastLine = in.readLine();
    // }
    // file.close();
    // QJsonDocument doc = QJsonDocument::fromJson(lastLine.toUtf8());
    // QJsonObject format = doc.object();
    // QJsonObject obj = format["data"].toObject();
    // // 此时 lastLine 就是文件的最后一行文本
    // // qDebug() << "最后一行:" << lastLine;
    // QString lastMessage;
    // QString lastMessageTime = obj["time"].toString();
    // if(format["kinds"].toString()=="text"){
    //     lastMessage = obj["message"].toString();
    // }else if(format["kinds"].toString()=="image"){
    //     lastMessage = "图片";
    // }

    // return {lastMessage,lastMessageTime};
}
