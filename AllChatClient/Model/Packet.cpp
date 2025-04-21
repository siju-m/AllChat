#include "Packet.h"

Packet::Packet()
{

}

const QByteArray &Packet::getPacket() const
{
    return m_packet;
}
