#include "messagepacket.h"

MessagePacket::MessagePacket() {}

inline QJsonObject MessagePacket::mapToJson(QMap<QString, QVariant> map)
{
    return QJsonObject::fromVariantMap(map);
}
