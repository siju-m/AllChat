#ifndef FRIENDAPPLYCACHE_H
#define FRIENDAPPLYCACHE_H

#include <QString>
#include <QStringList>
#include <hiredis/hiredis.h>

class FriendApplyCache {
public:
    FriendApplyCache(redisContext* context);
    ~FriendApplyCache();

    // 添加好友申请，返回 true 表示是首次申请（成功添加）
    bool applyFriend(const QString& from, const QString& to);

    // 判断是否已申请过
    bool hasAlreadyApplied(const QString& from, const QString& to);

    // 获取发出的申请列表
    QStringList getSentList(const QString& user);

    // 获取收到的申请列表
    QStringList getReceivedList(const QString& user);

    // 删除好友申请（一般在取消/拒绝时）
    void removeApply(const QString& from, const QString& to);

private:
    redisContext* m_context;

    QString makePairKey(const QString& from, const QString& to);
};

#endif // FRIENDAPPLYCACHE_H
