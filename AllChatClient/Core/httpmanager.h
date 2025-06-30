#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <mutex>

class HttpManager : public QObject
{
    Q_OBJECT
public:
    static HttpManager *getInstance(){
        if(m_instance == nullptr){
            std::lock_guard<std::mutex> locker(m_mutex);
            if(m_instance == nullptr)
                m_instance = new HttpManager();
        }
        return m_instance;
    }

    void getAvatar(const QString &userId);
    void getUserInfo(const QString &userId);
    void getOnlineUsers();

signals:
    void avatarReceived(const QString &userId, const QPixmap &avatar);
    void userInfoReceived(const QJsonObject &userInfo);
    void onlineUsersReceived(const QJsonArray &userList);
    void requestFailed(const QString &error);

private:
    HttpManager(const HttpManager &obj) = delete;
    HttpManager& operator=(const HttpManager& obj) = delete;
    explicit HttpManager(QObject *parent = nullptr);

    void onReplyFinished(QNetworkReply *reply);

private:
    static HttpManager *m_instance;
    static std::mutex m_mutex;
    QNetworkAccessManager m_manager;
};
#endif // HTTPMANAGER_H
