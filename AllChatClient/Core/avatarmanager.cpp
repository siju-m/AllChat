#include "avatarmanager.h"
#include "Core/chathistorymanager.h"

#include <QBuffer>
#include <QDir>
#include <qtconcurrentrun.h>

AvatarManager* AvatarManager::m_instance;
std::mutex AvatarManager::m_mutex;

AvatarManager::AvatarManager(QObject *parent)
    : QObject{parent},
    m_http_manager(HttpManager::getInstance())
{
    // 最大缓存容量为50MB
    m_avatar_list.setMaxCost(50 * 1024 * 1024);

    connect(m_http_manager, &HttpManager::avatarReceived, this, &AvatarManager::onAvatarReceived);
}

void AvatarManager::getAvatar(const QString &id, AvatarCallback callback)
{
    if(m_avatar_list.contains(id)){
        callback(*m_avatar_list.object(id));
        return;
    }

    //好友更新头像时需要服务端通知来重新缓存好友图片
    QPointer<AvatarManager> safeThis = this;
    (void)QtConcurrent::run([=]() {
        if (!safeThis) return;
        QReadLocker locker(&m_diskLock);
        QPixmap diskPixmap = safeThis->loadFromDiskCache(id);

        QMetaObject::invokeMethod(safeThis, [=]() {
            if (!diskPixmap.isNull()) {
                m_avatar_list.insert(id, new QPixmap(diskPixmap), 1); // 提升到内存缓存
                callback(diskPixmap);
            }else{
                if(!m_fetchingSet.contains(id)){
                    m_fetchingSet.insert(id);
                    m_http_manager->getAvatar(id);
                }
                m_callbacks[id].append(callback);
            }
        }, Qt::QueuedConnection);

    });
}

QPixmap *AvatarManager::getAvatar(const QString &id)
{
    if(m_avatar_list.contains(id))
        return m_avatar_list.object(id);
    return nullptr;
}

void AvatarManager::updateAvatar(const QString &userId, const QPixmap &pix)
{
    if(m_avatar_list.contains(userId)){
        updateMemoryCache(userId, pix);
        saveToDiskCache(userId, pix);
    }
}

void AvatarManager::refreshAvatar(const QString &userId)
{
    m_http_manager->getAvatar(userId);
    emit avatarUpdated(userId);
}

void AvatarManager::onAvatarReceived(const QString &userId, const QPixmap &avatar)
{
    m_avatar_list.insert(userId, new QPixmap(avatar), 1);
    m_fetchingSet.remove(userId);

    auto callbacks = m_callbacks.take(userId);
    for (auto &cb : callbacks)
        cb(avatar);

    QPointer<AvatarManager> safeThis = this;
    (void)QtConcurrent::run([=]() {
        if (!safeThis) return;
        QWriteLocker locker(&m_diskLock);
        safeThis->saveToDiskCache(userId, avatar);
    });
}

void AvatarManager::saveToDiskCache(const QString &id, const QPixmap &pixmap)
{
    QString cacheDir = avatarPath();
    QDir().mkpath(cacheDir);

    QString filePath = cacheDir + "/" + id + ".png";
    pixmap.save(filePath, "PNG");

}

QPixmap AvatarManager::loadFromDiskCache(const QString &id)
{
    QString cacheDir = avatarPath();
    QString filePath = cacheDir + "/" + id + ".png";
    QPixmap pixmap;
    pixmap.load(filePath);

    if(pixmap.isNull())
        return QPixmap();
    return pixmap;
}

void AvatarManager::updateMemoryCache(const QString &id, const QPixmap &pixmap)
{
    int cost = pixmap.width() * pixmap.height() * pixmap.depth() / 8;
    // insert 会覆盖旧缓存，并自动管理内存
    m_avatar_list.insert(id, new QPixmap(pixmap), cost);
}

QString AvatarManager::avatarPath()
{
    return ChatHistoryManager::getInstance()->getChatHistoryFilePath() + "/avatars";
}
