#include "datatransfer.h"

#include <QMessageBox>

DataTransfer* DataTransfer::m_instance = nullptr;

DataTransfer::DataTransfer(QObject *parent):
    QObject(parent),
    m_socket(new QTcpSocket(this)),
    m_currentDataLength(0),
    m_receivedBytes(0),
    m_currentReceivingState(WaitingForHeader)
{

    connect(m_socket, &QTcpSocket::readyRead, this, &DataTransfer::onReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &DataTransfer::onDisconnected);
    ConnectServer();
}

void DataTransfer::ConnectServer() {
    //clash开着的时候局域网ip无法连接到服务器
    QString host = "127.0.0.1"; // 服务器 IP
    quint16 port = 12345; // 服务器端口号

    m_socket->connectToHost(host, port); // 连接到服务器

    if (!m_socket->waitForConnected(3000)) {
        // QMessageBox::critical(this, "Error", "连接服务器失败!");
        qDebug()<<"连接服务器失败.";
    } else {
        qDebug()<<"已经成功连接服务器.";
    }
}

void DataTransfer::onDisconnected()
{
    qDebug()<<"和服务器断开连接";
}

void DataTransfer::handle_Data(QByteArray data)
{
    QDataStream in(data);
    in.setVersion(QDataStream::Qt_5_15);

    CommonEnum::message_type type;
    in >> type;
    switch(type){
    case CommonEnum::LOGIN_SUCCESS:
    case CommonEnum::LOGIN_FAILED:{
        emit loginResult(type);
        if(type == CommonEnum::LOGIN_SUCCESS){
            emit handleData(data);
        }
    }break;
    case CommonEnum::REGISTER_SUCCESS:
    case CommonEnum::REGISTER_FAILED:{
        emit registResult(type);
    }break;
    default:{
        emit handleData(data);
    }
        break;
    }
}

void DataTransfer::onReadyRead()
{

    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_5_15);
    while(!in.atEnd()){
        if (m_currentReceivingState == WaitingForHeader) {
            in >> m_currentDataLength;
            // qDebug()<<"数据长度："<<m_currentDataLength;
            if (m_currentDataLength <= 0 || m_currentDataLength > 10 * 1024 * 1024) {
                qWarning() << "Invalid data length:" << m_currentDataLength;
                resetState();
                return ;
            }
            m_dataBuffer.clear();
            m_dataBuffer.reserve(m_currentDataLength);
            m_currentReceivingState = ReceivingData;
        }
        if (m_currentReceivingState == ReceivingData) {
            int bytesToRead = qMin(m_socket->bytesAvailable(), m_currentDataLength - m_receivedBytes);
            QByteArray chunk = m_socket->read(bytesToRead);
            m_dataBuffer.append(chunk);
            m_receivedBytes += chunk.size();
            // qDebug()<<"数据大小："<<m_receivedBytes;
            if (m_receivedBytes == m_currentDataLength) {
                QByteArray data = m_dataBuffer;
                resetState();
                handle_Data(data);
                // emit handleData(data);
            }
        }
    }
    return ;
}

void DataTransfer::resetState()
{
    m_currentReceivingState = WaitingForHeader;
    m_currentDataLength = 0;
    m_receivedBytes = 0;
    m_dataBuffer.clear();

}

void DataTransfer::sendData(const Packet &pkt)
{
    QByteArray packet = pkt.getPacket();
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out<<static_cast<qint32>(packet.size());
    out.writeRawData(packet.constData(),packet.size());
    m_socket->write(data);
    m_socket->flush();
}
