#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include <QPixmap>
#include <QVector>
#include <QTime>

enum MessageType { Text, Image };

struct Message {
    MessageType type;
    QString text;
    QString imagePath;
    bool isOutgoing;
    QString userName;
    QString avatarPath;
};

class MessageModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit MessageModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    void addTextMessage(const QString &text, bool isOutgoing, const QString &userName, const QString &avatarPath);
    void addImageMessage(const QString &imagePath, bool isOutgoing, const QString &userName, const QString &avatarPath);

    enum Roles {
        TypeRole = Qt::UserRole + 1,
        TextRole,
        ImageRole,
        IsOutgoingRole,
        UserNameRole,
        AvatarRole
    };

    void clear();

private:
    QVector<Message> m_messages;
};

#endif // MESSAGEMODEL_H
