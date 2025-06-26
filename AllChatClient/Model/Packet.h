#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QDataStream>
#include <QIODevice>

#include <Core/configmanager.h>

class Packet
{
public:
    Packet();
    template <typename... Args>
    Packet(Args&&... args) {
        QDataStream out(&m_packet, QIODevice::WriteOnly);
        out.setVersion(ConfigManager::dataStreamVersion());
        (void)(out << ... << std::forward<Args>(args));
    }
    const QByteArray& getPacket() const;
private:
    QByteArray m_packet;
};

#endif // PACKET_H
