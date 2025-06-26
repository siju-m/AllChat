#ifndef COMMONUTIL_H
#define COMMONUTIL_H

#include <QDateTime>
#include <QPixmap>


class CommonUtil
{
public:
    static QPixmap getRoundPix(const QString &path);
    static QPixmap getSquarePix(const QString &path, int size=512);


    inline static QString getCurrentTime() {
        return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }
private:
    CommonUtil();
};

#endif // COMMONUTIL_H
