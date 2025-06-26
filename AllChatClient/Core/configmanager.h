#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QDataStream>


class ConfigManager
{
public:
    inline static QDataStream::Version dataStreamVersion(){
        return QDataStream::Qt_6_0;
    }

private:
    ConfigManager();
};

#endif // CONFIGMANAGER_H
