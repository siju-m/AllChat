#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QDataStream>
#include <QIODevice>

class Packet
{
public:
    Packet();
    template <typename... Args>
    Packet(Args&&... args) {
        QDataStream out(&m_packet, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_15);
        (void)(out << ... << std::forward<Args>(args));
    }
    const QByteArray& getPacket() const;
private:
    QByteArray m_packet;
};

#endif // PACKET_H
