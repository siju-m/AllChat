#include "datatransfer.h"

#include "configmanager.h"

DataTransfer::DataTransfer(QObject *parent):
    QObject(parent),
    m_currentDataLength(0),
    m_receivedBytes(0),
    m_currentReceivingState(WaitingForHeader)
{}

QByteArray DataTransfer::receiveData(QTcpSocket *socket)
{

    QDataStream in(socket);
    in.setVersion(ConfigManager::dataStreamVersion());
    while(!in.atEnd()){
        if (m_currentReceivingState == WaitingForHeader) {
            in >> m_currentDataLength;
            // qDebug()<<"数据长度："<<m_currentDataLength;
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
            // qDebug()<<"数据大小："<<m_receivedBytes;
            if (m_receivedBytes == m_currentDataLength) {
                QByteArray data = m_dataBuffer;
                resetState();
                emit handleData(data,socket);
            }
        }
    }
    return QByteArray();
}

void DataTransfer::resetState()
{
    m_currentReceivingState = WaitingForHeader;
    m_currentDataLength = 0;
    m_receivedBytes = 0;
    m_dataBuffer.clear();

}
