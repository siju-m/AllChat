#include "updateavatar.h"
#include "View/Components/tipsbox.h"
#include "ui_updateavatar.h"

#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <windows.h>


UpdateAvatar::UpdateAvatar(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UpdateAvatar)
    , m_avatarPath("")
{
    ui->setupUi(this);
    // this->setWindowFlags(Qt::CustomizeWindowHint);
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

    connect(ui->btnSelectAvatar,&QPushButton::clicked,this,&UpdateAvatar::chooseAvatar);
    connect(ui->btnUpdateAvatar,&QPushButton::clicked,this,[=](){
        emit send_updateAvatar(m_avatarPath);
    });
    connect(this,&UpdateAvatar::toSetAvatar,this,[=](){
        // QMessageBox::information(this, "成功", "成功更换头像!");
        // TipsBox *box = new TipsBox("成功更换头像!", SA_SUCCESS, 2, this);
        // box->show();
        TipsBox::showNotice("成功更换头像!", SA_SUCCESS, this);
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
