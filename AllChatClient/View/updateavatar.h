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

    void loadUserAvatar();
    void onSuccessSetAvatar();

protected:
    // 鼠标事件实现窗口拖动
    void mousePressEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void chooseAvatar();
    void setThisAvatar(const QString &path);
    void onUpdateAvatar();
    void showAvatar(const QPixmap &pixmap);

signals:
    void sendUpdateAvatar(const QPixmap &path);
    void setAvatar(const QPixmap &pix);//向外部传输路径

private:
    Ui::UpdateAvatar *ui;
    QPixmap m_pix;
};

#endif // UPDATEAVATAR_H
