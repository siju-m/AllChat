#ifndef MESSAGESENDERVIEW_H
#define MESSAGESENDERVIEW_H

#include "Core/chathistorymanager.h"
#include "Core/currentuser.h"
#include "Core/datatransfer.h"
#include "Model/message.h"

#include <QPlainTextEdit>
#include <QPushButton>
#include <QWidget>

class MessageSenderView : public QWidget
{
    Q_OBJECT
public:
    explicit MessageSenderView(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void initUI();
    void onSendClicked();
    void sendFile();
    void sendImage();

signals:
    void sendMsg(const Message &msg);

private:
    int m_topBtnSize;
    QPushButton* m_btnFile;
    QPushButton* m_btnImage;
    QPushButton* m_btnSend;
    QPlainTextEdit* m_msgEdit;

    CurrentUser* m_user;
    DataTransfer* m_dataTransfer;
    ChatHistoryManager* m_historyManager;
    ContactManager *m_contact_mg;
};

#endif // MESSAGESENDERVIEW_H
