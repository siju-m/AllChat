#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    Server server;
    if (!server.listen(QHostAddress::AnyIPv4, 12345)) {
        qCritical() << "服务启动失败:" << server.errorString();
        return -1;
    }
    qDebug() << "服务器绑定的地址:" << server.serverAddress() << "端口:" << server.serverPort();
    return app.exec();
}

