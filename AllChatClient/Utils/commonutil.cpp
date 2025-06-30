#include "commonutil.h"

#include <QPainter>
#include <QPainterPath>

#include <Core/avatarmanager.h>


QPixmap CommonUtil::getRoundPix(const QString &path)
{
    QPixmap pixmap(path);
    if (pixmap.isNull()) {
        return QPixmap();
    }

    // 步骤 1：从原图中心裁剪出正方形
    int side = qMin(pixmap.width(), pixmap.height());
    QRect squareRect(
        (pixmap.width() - side) / 2,
        (pixmap.height() - side) / 2,
        side,
        side
        );
    QPixmap squarePixmap = pixmap.copy(squareRect);

    // 步骤 2：创建一个透明背景的目标图
    QPixmap roundedPixmap(side, side);
    roundedPixmap.fill(Qt::transparent);

    // 步骤 3：绘制圆形头像
    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPainterPath qpath;
    qpath.addEllipse(0, 0, side, side);  // 画一个真正的圆
    painter.setClipPath(qpath);
    painter.drawPixmap(0, 0, squarePixmap);

    return roundedPixmap;
}

QPixmap CommonUtil::getRoundPix(const QPixmap &pixmap)
{
    if (pixmap.isNull()) {
        return QPixmap();
    }

    // 步骤 1：从原图中心裁剪出正方形
    int side = qMin(pixmap.width(), pixmap.height());
    QRect squareRect(
        (pixmap.width() - side) / 2,
        (pixmap.height() - side) / 2,
        side,
        side
        );
    QPixmap squarePixmap = pixmap.copy(squareRect);

    // 步骤 2：创建一个透明背景的目标图
    QPixmap roundedPixmap(side, side);
    roundedPixmap.fill(Qt::transparent);

    // 步骤 3：绘制圆形头像
    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPainterPath qpath;
    qpath.addEllipse(0, 0, side, side);  // 画一个真正的圆
    painter.setClipPath(qpath);
    painter.drawPixmap(0, 0, squarePixmap);

    return roundedPixmap;
}

QPixmap CommonUtil::getSquarePix(const QString &path, int size)
{
    QPixmap pixmap(path);
    if (pixmap.isNull()) {
        return QPixmap();
    }

    // 步骤 1：从原图中心裁剪出正方形
    int side = qMin(pixmap.width(), pixmap.height());
    QRect squareRect(
        (pixmap.width() - side) / 2,
        (pixmap.height() - side) / 2,
        side,
        side
        );
    QPixmap squarePixmap = pixmap.copy(squareRect);
    if(side>size){
        return squarePixmap.scaled(
            QSize(size, size),
            Qt::KeepAspectRatioByExpanding, // 保持比例并扩展至目标尺寸
            Qt::SmoothTransformation       // 平滑处理)
            );
    }else
        return squarePixmap;

}

QPixmap CommonUtil::getSquarePix(const QPixmap &pixmap, int size)
{
    int side = qMin(pixmap.width(), pixmap.height());
    QRect squareRect(
        (pixmap.width() - side) / 2,
        (pixmap.height() - side) / 2,
        side,
        side
        );
    QPixmap squarePixmap = pixmap.copy(squareRect);
    if(side>size){
        return squarePixmap.scaled(
            QSize(size, size),
            Qt::KeepAspectRatioByExpanding, // 保持比例并扩展至目标尺寸
            Qt::SmoothTransformation       // 平滑处理)
            );
    }else
        return squarePixmap;
}

// void CommonUtil::bindModelToAvatarUpdates(QAbstractListModel* model, std::function<QString(int row)> getIdForRow, int avatarRole)
// {
//     connect(AvatarManager::getInstance(), &AvatarManager::avatarUpdated, model, [=](const QString &id) {
//         for (int row = 0; row < model->rowCount(); ++row) {
//             if (getIdForRow(row) == id) {
//                 QModelIndex idx = model->index(row);
//                 emit model->dataChanged(idx, idx, {row});
//             }
//         }
//     });
// }


CommonUtil::CommonUtil() {}
