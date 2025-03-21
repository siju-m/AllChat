#include "updateavatar.h"
#include "ui_updateavatar.h"


UpdateAvatar::UpdateAvatar(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UpdateAvatar)
    , m_avatarPath("")
{
    ui->setupUi(this);
    connect(ui->btnSelectAvatar,&QPushButton::clicked,this,&UpdateAvatar::chooseAvatar);
    connect(ui->btnUpdateAvatar,&QPushButton::clicked,this,[=](){
        emit send_updateAvatar(m_avatarPath);
    });
    connect(this,&UpdateAvatar::toSetAvatar,this,[=](){
        QMessageBox::information(this, "成功", "成功更换头像!");
        emit setAvatar(m_avatarPath);
    });
}

UpdateAvatar::~UpdateAvatar()
{
    delete ui;
}

void UpdateAvatar::setAvatarPath(const QString &path)
{
    QPixmap pixmap(path);
    if(pixmap.isNull()){
        return;
    }
    m_avatarPath = path;
    showAvatar(pixmap);
}

void UpdateAvatar::showAvatar(const QPixmap &pixmap)
{
    QPixmap roundedPixmap(pixmap.size());
    roundedPixmap.fill(Qt::transparent);

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPainterPath painterPath;
    painterPath.addEllipse(0, 0, pixmap.width(), pixmap.height());
    painter.setClipPath(painterPath);
    painter.drawPixmap(0, 0, pixmap);

    ui->avatar->setPixmap(roundedPixmap);
}

void UpdateAvatar::chooseAvatar()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Select image File", "", "image Files (*.jpg *.png)");
    if(imagePath.isEmpty()) return;
    setAvatarPath(imagePath);
}
