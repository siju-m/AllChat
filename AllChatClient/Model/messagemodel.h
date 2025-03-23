#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include <QPixmap>
#include <QVector>
#include <QTime>

enum MessageType { Text, Image ,Time};

struct Message {
    MessageType type;
    QString text;
    QString imagePath;
    bool isOutgoing;
    QString userName;
    QString avatarPath;
    QString time;
};

//消息数据
class MessageModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit MessageModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    void addTextMessage(const QString &text, bool isOutgoing, const QString &userName, const QString &avatarPath, const QString &time);
    void addImageMessage(const QString &imagePath, bool isOutgoing, const QString &userName, const QString &avatarPath, const QString &time);
    void addTimeMessage(const QString &time);

    enum Roles {
        TypeRole = Qt::UserRole + 1,
        TextRole,
        ImageRole,
        IsOutgoingRole,
        UserNameRole,
        AvatarRole,
        TimeRole
    };

    void clear();

    void update_lastTempTime(const QString &targetId,const QString &lastMessageTime);
    QString get_lastTempTime(const QString &targetId);


private:
    QVector<Message> m_messages;
    QHash<QString,QString> m_temp_lastMsgTime;//存聊天对象消息时间的临时变量，避免一直排序
};

#endif // MESSAGEMODEL_H
