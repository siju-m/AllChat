#ifndef DATATRANSFER_H
#define DATATRANSFER_H

#include <QByteArray>
#include <QObject>
#include <QTcpSocket>

enum ReceivingState {
    WaitingForHeader, // 等待接收数据头
    ReceivingData     // 接收数据中
};

class DataTransfer : public QObject
{
    Q_OBJECT
public:
    DataTransfer(QObject *parent = nullptr);
    QByteArray receiveData(QTcpSocket *socket);
    void resetState();
private:
    qint32 m_currentDataLength ;    // 数据长度
    qint32 m_receivedBytes ;        // 已接收字节数
    QByteArray m_dataBuffer;           // 用于暂存接收到的数据
    ReceivingState m_currentReceivingState; // 当前状态初始化为等待数据头
    // enum ReceiveState { WaitingForHeader, ReceivingData };
    // ReceiveState m_state = WaitingForHeader;
    // qint32 m_expectedSize = 0;
    // QByteArray m_buffer;
    // void reset();
signals:
    void handleData(QByteArray data);
};

#endif // DATATRANSFER_H
