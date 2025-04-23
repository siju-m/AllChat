#ifndef MESSAGELISTVIEW_H
#define MESSAGELISTVIEW_H

#include "Model/message.h"
#include <QListView>
#include <QScrollBar>
#include <QTimer>
#include <QWheelEvent>

#include <Model/messagemodel.h>

class MessageListView : public QListView {
    Q_OBJECT

public:
    MessageListView(QWidget *parent = nullptr);

    void initMessageList();
    void initScorllBar();
    void clear(const QString &chatId);
    void clear();
    void addTime_toList(const QString &chatId,const QString &time);
    bool compareTime(const QString &pastTime,const QString &lastTime);
    // void addTextMessage(const QString &text, bool isOutgoing, const QString &userName, const QString &avatarPath, const QString &time);
    // void addImageMessage(const QString &imagePath, bool isOutgoing, const QString &userName, const QString &avatarPath, const QString &time);
    void addMessage(const Message &message);
    void addOlderMessage(const QVector<Message> &messages);

signals:
    void scrolledToTop();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void showAndHideScrollBars();

    void hideScrollBars();

    void checkScrollTop(int value);

private:
    QTimer *hideTimer;
    MessageModel *m_message_model;//存储消息数据

    // 初次加载历史记录
    bool m_isFirstLoad = true;
};


#endif // MESSAGELISTVIEW_H
