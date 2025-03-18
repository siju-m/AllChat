#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    Server server;
    if (!server.listen(QHostAddress::AnyIPv4, 12345)) {
        qCritical() << "Failed to start server:" << server.errorString();
        return -1;
    }
    qDebug() << "服务器绑定的地址:" << server.serverAddress() << "端口:" << server.serverPort();
    qDebug() << "ChatServer running on port 12345.";
    return app.exec();
}
