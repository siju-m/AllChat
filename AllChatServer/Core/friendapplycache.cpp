#include "FriendApplyCache.h"
#include "qdebug.h"

FriendApplyCache::FriendApplyCache(redisContext* context)
    : m_context(context) {}

FriendApplyCache::~FriendApplyCache() {}

QString FriendApplyCache::makePairKey(const QString& from, const QString& to) {
    return from + "|" + to;
}

bool FriendApplyCache::applyFriend(const QString& from, const QString& to) {
    QString pairKey = makePairKey(from, to);

    // 尝试添加到全局去重集合
    redisReply* reply = (redisReply*)redisCommand(m_context,
                                                    "SADD friend:apply:set %s", pairKey.toUtf8().data());

    bool added = (reply && reply->integer == 1);
    freeReplyObject(reply);

    if (added) {
        // 添加到两个方向的集合
        redisCommand(m_context, "SADD friend:apply:sent:%s %s", from.toUtf8().data(), to.toUtf8().data());
        redisCommand(m_context, "SADD friend:apply:recv:%s %s", to.toUtf8().data(), from.toUtf8().data());
    }

    return added;
}

bool FriendApplyCache::hasAlreadyApplied(const QString& from, const QString& to) {
    QString pairKey = makePairKey(from, to);
    redisReply* reply = (redisReply*)redisCommand(m_context,
                                                    "SISMEMBER friend:apply:set %s", pairKey.toUtf8().data());

    bool exists = (reply && reply->integer == 1);
    freeReplyObject(reply);
    return exists;
}

QStringList FriendApplyCache::getSentList(const QString& user) {
    redisReply* reply = (redisReply*)redisCommand(m_context,
                                                    "SMEMBERS friend:apply:sent:%s", user.toUtf8().data());

    QStringList result;
    if (reply && reply->type == REDIS_REPLY_ARRAY) {
        for (size_t i = 0; i < reply->elements; ++i) {
            result << QString::fromUtf8(reply->element[i]->str);
        }
    }
    freeReplyObject(reply);
    return result;
}

QStringList FriendApplyCache::getReceivedList(const QString& user) {
    QByteArray userUtf8 = user.toUtf8();
    if (!m_context || m_context->err) {
        qDebug() << "Redis连接失败:" << m_context->errstr;
        return QStringList();
    }
    redisReply* reply = (redisReply*)redisCommand(m_context,
                                                    "SMEMBERS friend:apply:recv:%s", userUtf8.constData());

    QStringList result;
    if (reply && reply->type == REDIS_REPLY_ARRAY) {
        for (size_t i = 0; i < reply->elements; ++i) {
            result << QString::fromUtf8(reply->element[i]->str);
        }
    }
    freeReplyObject(reply);
    return result;
}

void FriendApplyCache::removeApply(const QString& from, const QString& to) {
    QString pairKey = makePairKey(from, to);

    redisCommand(m_context, "SREM friend:apply:set %s", pairKey.toUtf8().data());
    redisCommand(m_context, "SREM friend:apply:sent:%s %s", from.toUtf8().data(), to.toUtf8().data());
    redisCommand(m_context, "SREM friend:apply:recv:%s %s", to.toUtf8().data(), from.toUtf8().data());
}
