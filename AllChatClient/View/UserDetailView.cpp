#include "UserDetailView.h"
#include "ui_UserDetailView.h"

#include <QPainter>
#include <QPainterPath>

UserDetailView::UserDetailView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserDetailView)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->btnToMessage,&QPushButton::clicked,this,[=](){
        emit showMessage(m_userInfo.id);
    });
    connect(ui->btnToDelete,&QPushButton::clicked,this,[=](){
        emit deleteFriend(m_userInfo.id);
    });
}

UserDetailView::~UserDetailView()
{
    delete ui;
}

void UserDetailView::showAvatar(const QString &path)
{
    QPixmap pixmap(path);
    if(pixmap.isNull()){
        return;
    }
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

void UserDetailView::showUserInfo(const QString &id, const User &user)
{
    QString name = user.getUserName();
    QString avatarPath = user.getAvatarPath();
    bool state = user.getOnlineState();
    this->show();
    this->setEnabled(true);
    m_userInfo = {name,id,state,avatarPath};
    showAvatar(avatarPath);
    QFont font;
    font.setFamily("Arial");  // 设置字体
    font.setPointSize(12);    // 设置字号
    ui->name->setFont(font);
    ui->name->setText(name);
    font.setPointSize(8);
    ui->id->setFont(font);
    ui->id->setText("UID: "+id);
    ui->id->setStyleSheet("color: gray;");
    StateEnum::onlineState_type onlineState = (StateEnum::onlineState_type)(state?0:1);
    ui->onlineState->setOnlineState(onlineState,{1,6,15,15});
    QString stateText = "      ";
    stateText.append(state?"在线":"离线");
    ui->onlineState->setText(stateText);
}
