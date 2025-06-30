#include "httpmanager.h"

#include <QJsonDocument>
#include <qjsonarray.h>
#include <qjsonobject.h>

HttpManager* HttpManager::m_instance;
std::mutex HttpManager::m_mutex;

HttpManager::HttpManager(QObject *parent) :
    QObject(parent)
{
    connect(&m_manager, &QNetworkAccessManager::finished, this, &HttpManager::onReplyFinished);
}

void HttpManager::getAvatar(const QString &userId)
{
    QUrl url("http://127.0.0.1:8088/avatar/" + userId);
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::User, QVariant::fromValue(QString("avatar:%1").arg(userId)));
    m_manager.get(request);
}

void HttpManager::onReplyFinished(QNetworkReply *reply)
{
    QString type = reply->request().attribute(QNetworkRequest::User).toString();
    QByteArray data = reply->readAll();

    if (reply->error()) {
        emit requestFailed(reply->errorString());
        reply->deleteLater();
        return;
    }

    if (type.startsWith("avatar:")) {
        QString userId = type.section(":", 1);
        QPixmap avatar;
        avatar.loadFromData(data);
        emit avatarReceived(userId, avatar);
    } else if (type == "userinfo") {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        emit userInfoReceived(doc.object());
    } else if (type == "onlinelist") {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        emit onlineUsersReceived(doc.object().value("onlineUsers").toArray());
    }

    reply->deleteLater();
}
