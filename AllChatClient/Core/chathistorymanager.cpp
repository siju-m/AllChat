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
#include "Core/contactmanager.h"

ChatHistoryManager *ChatHistoryManager::m_instance;
std::mutex ChatHistoryManager::m_mutex;

ChatHistoryManager::ChatHistoryManager(QObject *parent)
    : QObject{parent},
    m_user(CurrentUser::getInstance()),
    m_contact_mg(ContactManager::getInstance())
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
    if(!m_contact_mg->isInGroup(chatId))
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
    query.bindValue(":message_type", msg.getTypeString());
    query.bindValue(":timestamp", msg.getDateTime());

    if (!query.exec()) {
        qWarning() << "消息添加失败：" << query.lastError().text();
    }
}

QString ChatHistoryManager::saveImage(const QString &imageName, const QByteArray &imageData)
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
    QString imageFilePath = QString("%1/%2/%3.%4").arg(filePath).arg("image").arg(imageName).arg(format);
    image.save(imageFilePath);
    return imageFilePath;
}

QString ChatHistoryManager::storeAvatar(QString userName, const QByteArray &imageData)
{
    QString imageName = userName.append("_avatar");
    return saveImage(imageName, imageData);
}

QString ChatHistoryManager::storeImage(const QByteArray &imageData)
{
    QString imageName = QUuid::createUuid().toString(QUuid::WithoutBraces);
    return saveImage(imageName, imageData);
    // if(imageData.isEmpty()) return QString();
    // QString filePath = getChatHistoryFilePath();

    // QImage image;
    // image.loadFromData(imageData);
    // QBuffer buffer;
    // buffer.setData(imageData);
    // buffer.open(QIODevice::ReadOnly);  // 以只读模式打开
    // QImageReader reader(&buffer);
    // reader.setDecideFormatFromContent(true);
    // QString format = QString(reader.format());//读取图片后缀名

    // QDir dir(QString("%1/%2").arg(filePath).arg("image"));
    // if (!dir.exists()) {
    //     dir.mkpath(".");
    // }
    // //如果名字为空就用uid，有的话就是头像
    // //todo 可以给图片分类，根据类别命名
    // imageName = imageName.isEmpty()?QUuid::createUuid().toString(QUuid::WithoutBraces):imageName.append("_avatar");
    // QString imageFilePath = QString("%1/%2/%3.%4").arg(filePath).arg("image").arg(imageName).arg(format);
    // image.save(imageFilePath);
    // return imageFilePath;
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
            else if(m_contact_mg->isFriend(senderId))
            {
                sender = m_contact_mg->friendById(senderId);
            }
            else if(m_contact_mg->isStranger(senderId))
            {
                sender = m_contact_mg->strangerById(senderId);
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
}
