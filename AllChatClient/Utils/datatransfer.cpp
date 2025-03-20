#include "datatransfer.h"

DataTransfer::DataTransfer(QObject *parent):
    QObject(parent),
    m_currentDataLength(0),
    m_receivedBytes(0),
    m_currentReceivingState(WaitingForHeader)
{}

QByteArray DataTransfer::receiveData(QTcpSocket *socket)
{

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_15);
    while(!in.atEnd()){
        if (m_currentReceivingState == WaitingForHeader) {
            in >> m_currentDataLength;
            qDebug()<<"数据长度："<<m_currentDataLength;
            if (m_currentDataLength <= 0 || m_currentDataLength > 10 * 1024 * 1024) {
                qWarning() << "Invalid data length:" << m_currentDataLength;
                resetState();
                return QByteArray();
            }
            m_dataBuffer.clear();
            m_dataBuffer.reserve(m_currentDataLength);
            m_currentReceivingState = ReceivingData;
        }
        if (m_currentReceivingState == ReceivingData) {
            int bytesToRead = qMin(socket->bytesAvailable(), m_currentDataLength - m_receivedBytes);
            QByteArray chunk = socket->read(bytesToRead);
            m_dataBuffer.append(chunk);
            m_receivedBytes += chunk.size();
            qDebug()<<"数据大小："<<m_receivedBytes;
            if (m_receivedBytes == m_currentDataLength) {
                QByteArray data = m_dataBuffer;
                resetState();
                emit handleData(data);
            }
        }
    }
    return QByteArray();
}
// QByteArray DataTransfer::receiveData(QTcpSocket* socket) {
//     while (socket->bytesAvailable() > 0) {
//         if (m_state == WaitingForHeader) {
//             if (socket->bytesAvailable() < sizeof(qint32)) return {};

//             QDataStream in(socket);
//             in.setVersion(QDataStream::Qt_5_15);
//             in >> m_expectedSize;

//             if (m_expectedSize <= 0 || m_expectedSize > 10*1024*1024) {
//                 qWarning() << "Invalid size:" << m_expectedSize;
//                 reset();
//                 return {};
//             }

//             m_buffer.clear();
//             m_buffer.reserve(m_expectedSize);
//             m_state = ReceivingData;
//         }

//         if (m_state == ReceivingData) {
//             const qint64 bytesToRead = qMin(
//                 socket->bytesAvailable(),
//                 m_expectedSize - m_buffer.size()
//                 );

//             m_buffer.append(socket->read(bytesToRead));

//             if (m_buffer.size() == m_expectedSize) {
//                 QByteArray completeData = m_buffer;
//                 reset();
//                 return completeData;
//             }
//         }
//     }
//     return {};
// }

void DataTransfer::resetState()
{
    m_currentReceivingState = WaitingForHeader;
    m_currentDataLength = 0;
    m_receivedBytes = 0;
    m_dataBuffer.clear();

}
// void DataTransfer::reset() {
//     m_state = WaitingForHeader;
//     m_expectedSize = 0;
//     m_buffer.clear();
// }
