#include "updateavatar.h"
#include "Core/avatarmanager.h"
#include "Core/currentuser.h"
#include "View/Components/tipsbox.h"
#include "ui_updateavatar.h"

#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <windows.h>

#include <Utils/commonutil.h>


UpdateAvatar::UpdateAvatar(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UpdateAvatar)
    , m_pix(QPixmap())
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);         //无边框
    this->setAttribute(Qt::WA_TranslucentBackground);     //窗口透明
    //设置投影效果
    QGraphicsDropShadowEffect *windowShadow;        //阴影效果
    windowShadow = new QGraphicsDropShadowEffect(this);
    windowShadow->setBlurRadius(30);
    windowShadow->setColor(QColor(0, 0, 0));
    windowShadow->setOffset(0, 0);
    ui->frame->setGraphicsEffect(windowShadow);
    connect(ui->btnClose, &QPushButton::clicked, this, &UpdateAvatar::close);

    ui->btnUpdateAvatar->setEnabled(false);
    connect(ui->btnSelectAvatar, &QPushButton::clicked, this, &UpdateAvatar::chooseAvatar);
    connect(ui->btnUpdateAvatar, &QPushButton::clicked, this, &UpdateAvatar::onUpdateAvatar);
}

UpdateAvatar::~UpdateAvatar()
{
    delete ui;
}

void UpdateAvatar::loadUserAvatar()
{
    AvatarManager::getInstance()->getAvatar(CurrentUser::getInstance()->get_userId(), [=](const QPixmap &pix){
        if(pix.isNull()){
            return;
        }
        showAvatar(pix);
    });
}

void UpdateAvatar::setThisAvatar(const QString &path)
{
    QPixmap pixmap(path);
    if(pixmap.isNull()){
        return;
    }
    m_pix = pixmap;
    showAvatar(pixmap);
}

void UpdateAvatar::onUpdateAvatar()
{
    emit sendUpdateAvatar(m_pix);
}

void UpdateAvatar::onSuccessSetAvatar()
{
    TipsBox::showNotice("成功更换头像!", SA_SUCCESS, this);
    emit setAvatar(m_pix);
    AvatarManager::getInstance()->updateAvatar(CurrentUser::getInstance()->get_userId(), m_pix);
}

void UpdateAvatar::showAvatar(const QPixmap &pixmap)
{
    QPixmap roundedPixmap = CommonUtil::getRoundPix(pixmap);

    ui->avatar->setPixmap(roundedPixmap);
}

void UpdateAvatar::chooseAvatar()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Select image File", "", "image Files (*.jpg *.png)");
    if(imagePath.isEmpty()) return;
    setThisAvatar(imagePath);
    ui->btnUpdateAvatar->setEnabled(true);
}

void UpdateAvatar::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
#ifdef Q_OS_WIN
        ReleaseCapture();
        SendMessage(HWND(window()->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
#else
        window()->windowHandle()->startSystemMove();
#endif
        event->accept();
    } else {
        QWidget::mousePressEvent(event);
    }
}

void UpdateAvatar::closeEvent(QCloseEvent *event)
{
    ui->btnUpdateAvatar->setEnabled(false);
    QDialog::closeEvent(event);
}
