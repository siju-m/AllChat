#include "httpservermanager.h"

#include <QCoreApplication>

HttpServerManager::HttpServerManager() :
    m_http(new QHttpServer()),
    m_dataBase(DataBase::getInstance())
{
}

HttpServerManager::~HttpServerManager()
{
    m_http->deleteLater();
}

void HttpServerManager::startServer(quint16 port)
{
    // 获取头像接口
    m_http->route("/avatar/<arg>", [this](const QString &userId) {
        QByteArray avatar = m_dataBase->getAvatar(userId);
        if (avatar.isEmpty())
            return QHttpServerResponse("Avatar not found", QHttpServerResponder::StatusCode::NotFound);

        QHttpServerResponse response("image/png", avatar);
        response.setHeader("Cache-Control", "no-cache");
        return response;
    });

    // 查询用户信息接口
    // m_http->route("/user/<arg>", [this](const QString &userId) {
    //     QString name = m_dataBase->selectNameById(userId);
    //     bool isOnline = m_onlineUsers.contains(userId);
    //     QJsonObject obj{
    //         {"userId", userId},
    //         {"username", name},
    //         {"online", isOnline}
    //     };
    //     return QHttpServerResponse(QJsonDocument(obj).toJson(), "application/json");
    // });

    // // 在线用户列表接口
    // m_http->route("/online", [this]() {
    //     QJsonArray arr;
    //     for (const auto &id : m_onlineUsers) {
    //         QJsonObject obj{
    //             {"userId", id},
    //             {"username", m_dataBase->selectNameById(id)}
    //         };
    //         arr.append(obj);
    //     }
    //     QJsonObject result{
    //         {"onlineUsers", arr}
    //     };
    //     return QHttpServerResponse(QJsonDocument(result).toJson(), "application/json");
    // });

    // 可选：远程关闭服务器
    m_http->route("/shutdown", [] {
        QMetaObject::invokeMethod(QCoreApplication::instance(), "quit", Qt::QueuedConnection);
        return QHttpServerResponse("Server shutting down...");
    });

    // 启动监听
    if (!m_http->listen(QHostAddress::Any, port)) {
        qWarning() << "HTTP服务启动失败，端口可能已被占用。";
    } else {
        qDebug() << "HTTP服务已启动，端口:" << port;
    }
}
