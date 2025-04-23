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
    // QString filePath = getChatFilePath(msg.getChatId());

    // filePath+=QString("/%1_%2.txt").arg(msg.getChatId()).arg(m_user->get_userName());
    // // qDebug()<<filePath;
    // QFile file(filePath);
    // if (file.open(QIODevice::Append | QIODevice::Text)) {
    //     //以json格式存储，适合结构化数据
    //     QTextStream out(&file);
    //     out << msg.jsonData() << Qt::endl;
    // }

    QSqlQuery query;
    query.prepare("INSERT INTO messages (conversation_id, sender_id, content, message_type, timestamp) "
                  "VALUES (:conversation_id, :sender_id, :content, :message_type, :timestamp)");
    query.bindValue(":conversation_id", msg.getChatId());
    query.bindValue(":sender_id", msg.getSenderId());
    query.bindValue(":content", msg.getData());
    query.bindValue(":message_type", msg.getType_string());
    query.bindValue(":timestamp", msg.getDateTime());

    if (!query.exec()) {
        qWarning() << "好友添加失败：" << query.lastError().text();
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

void ChatHistoryManager::loadChatHistoryFromFile(QString targetId)
{
    //更新好友信息
    // QString filePath = getChatFilePath(targetId);//因为多个个客户端会运行在同一台机器上，需要接收端id加发送端用户名来作为文件名

    // filePath+=QString("/%1_%2.txt").arg(targetId).arg(m_user->get_userName());
    // QFile file(filePath);
    // if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     QTextStream in(&file);
    //     // QString lastTime = "";//用于比较消息间的时间差距
    //     while (!in.atEnd()) {
    //         QString line = in.readLine();
    //         QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8());
    //         QJsonObject format = doc.object();
    //         QJsonObject obj = format["data"].toObject();

    //         QString senderId = obj["id"].toString();
    //         QString time = obj["time"].toString();

    //         User sender;
    //         if(senderId == m_user->get_userId())
    //         {
    //             sender = m_user->toUser();
    //         }
    //         else if(m_user->getFriendList().contains(senderId))
    //         {
    //             sender = m_user->getFriendList()[senderId];
    //         }
    //         else if(m_user->getStrangerList().contains(senderId))
    //         {
    //             sender = m_user->getStrangerList()[senderId];
    //         }

    //         if(format["kinds"].toString()=="text"){
    //             QString message = obj["message"].toString();
    //             Message msg(Message::Text, message, time, sender, targetId);
    //             emit addMessage_toList(msg);
    //             // emit addMessage_toList(message, targetId, senderId, time);
    //         }else if(format["kinds"].toString()=="image"){
    //             QString imagePath = obj["imagePath"].toString();
    //             Message msg(Message::Image, imagePath, time, sender, targetId);
    //             emit addMessage_toList(msg);
    //             // emit addImage_toList(imagePath, targetId, senderId, time);
    //         }

    //     }
    //     file.close();
    // }
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
    QString filePath = getChatFilePath(targetId);//因为多个个客户端会运行在同一台机器上，需要接收端id加发送端用户名来作为文件名
    filePath+=QString("/%1_%2.txt").arg(targetId).arg(m_user->get_userName());
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

// QString ChatHistoryManager::getFirstMessageTime(const QString &targetId)
// {
//     QString filePath = getChatFilePath(targetId);//因为多个个客户端会运行在同一台机器上，需要接收端id加发送端用户名来作为文件名
//     filePath+=QString("/%1_%2.txt").arg(targetId).arg(m_user->get_userName());
//     QFile file(filePath);
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         return "";
//     }
//     QString firstLine;
//     QTextStream in(&file);
//     firstLine = in.readLine();

//     file.close();
//     QJsonDocument doc = QJsonDocument::fromJson(firstLine.toUtf8());
//     QJsonObject format = doc.object();
//     QJsonObject obj = format["data"].toObject();
//     QString firstMessageTime = obj["time"].toString();

//     return firstMessageTime;
// }
