#ifndef CHATMODEL_H
#define CHATMODEL_H

#include <QAbstractListModel>

struct Chats{
    QString userName="";
    QString lastMessage="";
    QString lastMessageTime="";
    int unreadMsgNum=0;
    QString avatarPath="";
};

//聊天对象的数据
class ChatModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ChatModel(QObject *parent = nullptr);

    enum Roles {
        UserNameRole = Qt::UserRole + 1,
        IdRole,
        LastMessageRole,
        LastMessageTimeRole,
        UnreadMsgNumRole,
        AvatarRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void addChat_toList(const QString &userName,const QString &id,const QString &lastMessage,const QString &lastMessageTime,const int &unreadMsgNum, const QString &avatarPath="");
    void updateLastMessage(QString targetId,const QString &message,const QString &lastMessageTime);
    QString getLastMsgTime(const QString &targetId);
    void removeItem(const QString &targetId);
    void clear();

    void sortToFirst(const int &row);//把最新时间的聊天对象移到最前面
    void sortListByTime();//在添加对象之后给所有聊天对象排序
    bool compareByTime(const QString &a, const QString &b);//给sort的自定义比较函数

    int get_currentChatRow();
    void set_currentChatId(const QString &id);

    void add_unreadMsgNum(const QString &id);
    void clear_unreadMsgNum(const QString &id);//清除未读消息
signals:
    void sortEnd();
private:
    //本来是使用QMap的，但是外部的索引和内部对不上，因为QMap的插入顺序和实际顺序不一样
    QMap<QString,Chats> m_id_chats;
    QVector<QString> m_ids;
    QString m_current_chatId;

};

#endif // CHATMODEL_H
