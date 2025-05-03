#ifndef REDISCLIENT_H
#define REDISCLIENT_H

#include <QObject>
#include <QDebug>

extern "C" {
#include <hiredis/hiredis.h>
}

class RedisClient : public QObject
{
    Q_OBJECT
public:
    explicit RedisClient(QObject *parent = nullptr);
    ~RedisClient();
    void exacuteCommand(QString cmd);

    // 转发内容（m_forward_contents）
    void addForwardMessage(const QString &toUser, const QByteArray &msg);
    QByteArray getForwardMessages(const QString &toUser);
    void deleteForwardMessages(const QString &toUser);

private:
    void connectToRedis(const QString &host = "127.0.0.1", int port = 6379);
    void disconnectFromRedis();

signals:
    void redisConnected();
    void redisDisconnected();

private:
    redisContext *m_context;

};

#endif // REDISCLIENT_H
