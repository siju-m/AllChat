#include "chatlistview.h"
#include "Core/currentuser.h"
#include "Delegate/chatdelegate.h"

#include <QScrollBar>
#include <QTimer>

ChatListView::ChatListView(QWidget* parent):QListView(parent) {
    initChatList();
}

void ChatListView::addChatItem(const QString &userName, const QString &id, const QString &lastMessage, const QString &lastMessageTime, const int &unreadMsgNum)
{
    m_chat_model->addChat_toList(userName, id, lastMessage, lastMessageTime, unreadMsgNum);
}

void ChatListView::removeChatItem(const QString &id)
{
    m_chat_model->removeItem(id);
}

int ChatListView::row(const QString &id)
{
    return m_chat_model->get_rowById(id);
}

void ChatListView::clear()
{
    m_chat_model->clear();
}

void ChatListView::updateLastMessage(const QString &targetId,const QString &message,const QString &lastMessageTime)
{
    m_chat_model->updateLastMessage(targetId, message, lastMessageTime);
}

void ChatListView::addUnreadMsgNum(const QString &chatId)
{
    m_chat_model->add_unreadMsgNum(chatId);
}

void ChatListView::initChatList()
{
    m_chat_model = new ChatModel(this);
    ChatDelegate *chat_delegate = new ChatDelegate(this);

    this->setModel(m_chat_model);
    this->setItemDelegate(chat_delegate);

    this->setVerticalScrollMode(QListView::ScrollPerPixel); // 平滑滚动
    this->setResizeMode(QListView::Adjust);                 // 自动调整项大小
    this->setSelectionMode(QAbstractItemView::SingleSelection);  // 单选
    this->setSelectionBehavior(QAbstractItemView::SelectItems);

    this->setUniformItemSizes(true);//启用统一项尺寸优化

    QScrollBar *verticalScrollBar = this->verticalScrollBar();
    verticalScrollBar->setSingleStep(10); // 垂直滚动步长
    verticalScrollBar->setPageStep(100);  // 垂直滚动一页的距离

    connect(this, &QListView::clicked, this, &ChatListView::onItemClicked);
    connect(m_chat_model, &ChatModel::sortEnd, this, &ChatListView::onModelSortEnd);
}

void ChatListView::onItemClicked(const QModelIndex &index)
{
    QString id = index.data(ChatModel::IdRole).toString();
    if(CurrentUser::getInstance()->get_currentChatId() != id){
        // qDebug() << id;
        // loadChatHistoryFromFile(id);
        emit chatItemClicked(id);
        m_chat_model->set_currentChatId(id);
        m_chat_model->clear_unreadMsgNum(id);
        CurrentUser::getInstance()->set_currentChatId(id);
    }
}

void ChatListView::onModelSortEnd()
{
    //将当前聊天对象的选择设为排序后的当前聊天对象
    //QTimer的作用 使得排序完成后的其他事件先处理，再更新选择状态
    QTimer::singleShot(10, this, [=]() {
        if(this->model()->rowCount() > 0){
            int row = m_chat_model->get_currentChatRow();
            QModelIndex index = this->model()->index(row, 0);
            if(this->currentIndex() == index)
                return;
            this->setCurrentIndex(index);
        }
    });
}
