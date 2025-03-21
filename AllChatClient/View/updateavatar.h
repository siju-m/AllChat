#ifndef UPDATEAVATAR_H
#define UPDATEAVATAR_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>

namespace Ui {
class UpdateAvatar;
}

class UpdateAvatar : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateAvatar(QWidget *parent = nullptr);
    ~UpdateAvatar();

    void setAvatarPath(const QString &path);
signals:
    void send_updateAvatar(const QString &path);
    void setAvatar(const QString &path);//向外部传输路径
    void toSetAvatar();//通知内部可以传输路径
private:
    Ui::UpdateAvatar *ui;
    QString m_avatarPath;
    void showAvatar(const QPixmap &pixmap);
private slots:
    void chooseAvatar();
};

#endif // UPDATEAVATAR_H
