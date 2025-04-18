#ifndef DATATRANSFER_H
#define DATATRANSFER_H

#include "Utils/CustomTypes.h"
#include <QByteArray>
#include <QObject>
#include <QTcpSocket>
#include <QCoreApplication>
#include <Model/Packet.h>

enum ReceivingState {
    WaitingForHeader, // 等待接收数据头
    ReceivingData     // 接收数据中
};

class DataTransfer : public QObject
{
    Q_OBJECT
public:
    DataTransfer(const DataTransfer& obj) = delete;
    DataTransfer& operator=(const DataTransfer& obj) = delete;
    static DataTransfer* getInstance(){
        if(m_instance == nullptr)
            m_instance = new DataTransfer(qApp);
        return m_instance;
    }

    void onReadyRead(); // 处理服务器返回的数据
    void resetState();

    void ConnectServer();
    void sendData(const Packet &pkt);
    void onDisconnected();      // 处理断开连接

    void handle_Data(QByteArray data);

signals:
    void handleData(QByteArray data);
    void loginResult(CommonEnum::message_type result);
    void registResult(CommonEnum::message_type result);

private:

    explicit DataTransfer(QObject *parent = nullptr);
    static DataTransfer *m_instance;

    QTcpSocket *m_socket;

    qint32 m_currentDataLength ;    // 数据长度
    qint32 m_receivedBytes ;        // 已接收字节数
    QByteArray m_dataBuffer;           // 用于暂存接收到的数据
    ReceivingState m_currentReceivingState; // 当前状态初始化为等待数据头

};

#endif // DATATRANSFER_H
