#ifndef MESSAGEPACKET_H
#define MESSAGEPACKET_H

#include <QJsonObject>
#include <QObject>

class MessagePacket : QObject
{
    Q_OBJECT
public:
    MessagePacket();

    QJsonObject mapToJson(QMap<QString, QVariant> map);
};

#endif // MESSAGEPACKET_H
