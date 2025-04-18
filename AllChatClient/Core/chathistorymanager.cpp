#include "chathistorymanager.h"

#include <QBuffer>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QImageReader>
#include <QJsonDocument>
#include <QJsonObject>

ChatHistoryManager::ChatHistoryManager(QObject *parent)
    : QObject{parent},
    m_user(CurrentUser::getInstance())
{}

QString ChatHistoryManager::getChatHistoryFilePath() {
    // 获取项目根目录
    QString projectRootPath = QCoreApplication::applicationDirPath();

    // 确保聊天记录的目录存在
    QDir dir(projectRootPath + "/chat_history");
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return dir.path();
}

void ChatHistoryManager::addHistoryToFile(Message &msg)
{
    QString filePath = getChatHistoryFilePath();
    filePath+=QString("/%1_%2.txt").arg(msg.getChatId()).arg(m_user->get_userName());
    // qDebug()<<filePath;
    QFile file(filePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        //以json格式存储，适合结构化数据
        QTextStream out(&file);
        out << msg.jsonData() << Qt::endl;
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
    QString filePath = getChatHistoryFilePath();//因为多个个客户端会运行在同一台机器上，需要接收端id加发送端用户名来作为文件名
    filePath+=QString("/%1_%2.txt").arg(targetId).arg(m_user->get_userName());
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        // QString lastTime = "";//用于比较消息间的时间差距
        while (!in.atEnd()) {
            QString line = in.readLine();
            QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8());
            QJsonObject format = doc.object();
            QJsonObject obj = format["data"].toObject();

            QString senderId = obj["id"].toString();
            QString time = obj["time"].toString();
            if(format["kinds"].toString()=="text"){
                QString message = obj["message"].toString();
                emit addMessage_toList(message, targetId, senderId, time);
            }else if(format["kinds"].toString()=="image"){
                QString imagePath = obj["imagePath"].toString();
                emit addImage_toList(imagePath, targetId, senderId, time);
            }

        }
        file.close();
    }
}

QPair<QString, QString> ChatHistoryManager::getLastMessage(const QString &targetId)
{
    QString filePath = getChatHistoryFilePath();//因为多个个客户端会运行在同一台机器上，需要接收端id加发送端用户名来作为文件名
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
