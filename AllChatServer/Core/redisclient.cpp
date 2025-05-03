#include "redisclient.h"

RedisClient::RedisClient(QObject *parent)
    : QObject{parent}
{
    connectToRedis();
}

RedisClient::~RedisClient()
{
    disconnectFromRedis();
}

void RedisClient::exacuteCommand(QString cmd)
{
    redisReply* pReplySet = (redisReply*)redisCommand(m_context, cmd.toStdString().c_str());
    qDebug() << "set cmd: " << cmd;
    qDebug() << "set reply: " << pReplySet->str;
    freeReplyObject(pReplySet);
}

void RedisClient::addForwardMessage(const QString &toUser, const QByteArray &msg) {
    QString key = QString("forward:%1").arg(toUser);
    // 超过7天自动删除
    redisCommand(m_context, "set %s %b EX 604800", key.toUtf8().data(), msg.data(), msg.size());
}

QByteArray RedisClient::getForwardMessages(const QString &toUser) {
    QByteArray messages;
    QString key = QString("forward:%1").arg(toUser);
    redisReply *reply = (redisReply *)redisCommand(m_context, "get %s", key.toUtf8().data());

    messages = QByteArray(reply->str, reply->len);
    qDebug()<< messages << messages.isEmpty();

    freeReplyObject(reply);
    return messages;
}

void RedisClient::deleteForwardMessages(const QString &toUser) {
    QString key = QString("forward:%1").arg(toUser);
    redisCommand(m_context, "DEL %s", key.toUtf8().data());
}

void RedisClient::connectToRedis(const QString &host, int port)
{
    m_context = redisConnect(host.toStdString().c_str(), port);
    if (m_context != nullptr && m_context->err)
    {
        qDebug() << "Error connecting to Redis: " << m_context->errstr;
    }
    else
    {
        emit redisConnected();
    }
    qDebug() << "连接redis";
}

void RedisClient::disconnectFromRedis()
{
    if (m_context != nullptr)
    {
        redisFree(m_context);
        m_context = nullptr;
        emit redisDisconnected();
        qDebug() << "和redis断开连接";
    }
}
