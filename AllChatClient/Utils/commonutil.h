#ifndef COMMONUTIL_H
#define COMMONUTIL_H

#include <QAbstractListModel>
#include <QDateTime>
#include <QPixmap>


class CommonUtil : QObject
{
    Q_OBJECT
public:
    static QPixmap getRoundPix(const QString &path);
    static QPixmap getRoundPix(const QPixmap &pixmap);
    static QPixmap getSquarePix(const QString &path, int size=512);
    static QPixmap getSquarePix(const QPixmap &pixmap, int size=512);

    // static void bindModelToAvatarUpdates(QAbstractListModel* model, std::function<QString(int row)> getIdForRow, int avatarRole);

    inline static QString getCurrentTime() {
        return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }
private:
    CommonUtil();
};

#endif // COMMONUTIL_H
