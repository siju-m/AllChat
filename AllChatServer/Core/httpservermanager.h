#ifndef HTTPSERVERMANAGER_H
#define HTTPSERVERMANAGER_H

#include "Core/database.h"
#include <QHttpServer>


class HttpServerManager
{
public:
    HttpServerManager();
    ~HttpServerManager();

    void startServer(quint16 port);

private:
    QHttpServer *m_http;
    DataBase *m_dataBase;
};

#endif // HTTPSERVERMANAGER_H
