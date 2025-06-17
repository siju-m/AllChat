#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include "Model/message.h"
#include <QAbstractListModel>
#include <QPixmap>
#include <QVector>
#include <QTime>



enum MessageType { Text, Image ,Time, File, NONE};
//消息数据
class MessageModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit MessageModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    void addMessage(const Message &msg);
    void addOlderMessage(const Message &messages);
    void addTimeMessage(const QString &time);
    void addOlderTimeMessage(const QString &time);

    enum Roles {
        TypeRole = Qt::UserRole + 1,
        ContentRole,
        IsOutgoingRole,
        UserNameRole,
        AvatarRole,
        TimeRole
    };

    void clear();

    QString get_lastTime();
    QString get_olderTime();

private:
    MessageType getType(Message::MessageType type);
    void loadAvatar(const QString &avatarPath);
    void loadImage(const QString &imagePath);

private:

    struct ModelMessage {
        MessageType type;
        QString content;
        bool isOutgoing;
        QString userName;
        QString avatarPath;
        QString time;
    };

    QVector<ModelMessage> m_messages;
    QHash<QString,QString> m_temp_lastMsgTime;//存聊天对象消息时间的临时变量，避免一直排序

    QHash<QString, QPixmap> avatarCache;
    QHash<QString, QPixmap> imageCache;
};

#endif // MESSAGEMODEL_H
