#ifndef CHATLISTVIEW_H
#define CHATLISTVIEW_H

#include "Model/chatmodel.h"
#include <QListView>

class ChatListView : public QListView
{
    Q_OBJECT
public:
    ChatListView(QWidget* parent=nullptr);

    void addChatItem(const QString &userName,const QString &id,const QString &lastMessage,const QString &lastMessageTime,const int &unreadMsgNum);
    void removeChatItem(const QString &id);

    int row(const QString &id);
    void clear();

    void updateLastMessage(const QString &targetId,const QString &message,const QString &lastMessageTime);
    void addUnreadMsgNum(const QString &chatId);

private:
    void initChatList();
    void onItemClicked(const QModelIndex &index);
    void onModelSortEnd();

signals:
    void chatItemClicked(const QString &id);

private:
    ChatModel *m_chat_model;
};

#endif // CHATLISTVIEW_H
