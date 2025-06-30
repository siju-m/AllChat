#ifndef AVATARMANAGER_H
#define AVATARMANAGER_H

#include "Core/httpmanager.h"
#include <QCache>
#include <QHash>
#include <QNetworkDiskCache>
#include <QObject>
#include <QReadWriteLock>

class AvatarManager : public QObject
{
    Q_OBJECT
public:
    //回调函数
    static AvatarManager *getInstance(){
        if(m_instance == nullptr){
            std::lock_guard<std::mutex> locker(m_mutex);
            if(m_instance == nullptr){
                m_instance = new AvatarManager();
            }
        }
        return m_instance;
    }
    using AvatarCallback = std::function<void(const QPixmap &)>;

    void getAvatar(const QString &id, AvatarCallback callback);
    QPixmap *getAvatar(const QString &id);

    void updateAvatar(const QString &userId, const QPixmap &pix);

    void refreshAvatar(const QString &userId);

private:
    AvatarManager(const AvatarManager &obj) = delete;
    AvatarManager& operator=(const AvatarManager &obj) = delete;
    explicit AvatarManager(QObject *parent = nullptr);

    void onAvatarReceived(const QString &userId, const QPixmap &avatar);

    void saveToDiskCache(const QString &id, const QPixmap &pixmap);
    QPixmap loadFromDiskCache(const QString &id);
    void updateMemoryCache(const QString &id, const QPixmap &pixmap);

    QString avatarPath();

signals:
    void avatarUpdated(const QString &id);

private:
    static AvatarManager *m_instance;
    static std::mutex m_mutex;
    QCache<QString, QPixmap> m_avatar_list;
    QSet<QString> m_fetchingSet;
    QHash<QString, QList<AvatarCallback>> m_callbacks;
    HttpManager *m_http_manager;
    QReadWriteLock m_diskLock;
};

#endif // AVATARMANAGER_H
