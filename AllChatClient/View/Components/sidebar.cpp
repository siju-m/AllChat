#include "sidebar.h"

#include <QBuffer>
#include <QVBoxLayout>

#include "Core/avatarmanager.h"
#include "Core/currentuser.h"
#include "Core/datatransfer.h"
#include "Model/Packet.h"
#include "Utils/CustomTypes.h"

#include <Core/chathistorymanager.h>
#include <Utils/commonutil.h>
#include <qtconcurrentrun.h>

SideBar::SideBar(QWidget *parent)
    : QWidget{parent},
    m_avatar(new AvatarLabel(this)),
    m_btn_switch_msgs(new QToolButton(this)),
    m_btn_switch_frds(new QToolButton(this)),
    m_sideBar_btnGroup(new QButtonGroup(this)),
    m_updateAvatar(new UpdateAvatar(this))
{
    this->setFixedWidth(50);
    QFrame *frame = new QFrame(this);
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(frame);
    mainLayout->setContentsMargins(0,0,0,0);

    this->setStyleSheet(R"(
    QFrame{
        border-right: 1px solid gray;
    }
    QToolButton{
        border-top: 3px outset transparent;
        border-bottom: 7px outset transparent;
        border-right: 3px outset transparent;
        border-left: 3px outset transparent;
    }
    QToolButton:hover{
        background-color: rgb(205, 205, 205);
    }
    QToolButton:pressed,QToolButton:checked{
        border-left: 3px outset rgb(93, 95, 97);
        background-color: rgb(228, 228, 228);
    }
    )");

    m_avatar->setFixedSize(40, 40);
    m_avatar->setScaledContents(true);
    m_avatar->setStyleSheet(R"(
    border-radius: 20px; /* 圆形效果，需与 QLabel 的宽度一致 */
    border: 2px solid #cccccc; /* 可选，添加边框 */
    background-color: #cccccc;
    )");

    m_btn_switch_msgs->setFixedSize(45, 40);
    m_btn_switch_msgs->setIconSize(QSize(30, 30));
    m_btn_switch_msgs->setIcon(QIcon(":/Icon/comment.png"));
    m_btn_switch_msgs->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_btn_switch_msgs->setCheckable(true);
    m_btn_switch_frds->setFixedSize(45, 40);
    m_btn_switch_frds->setIconSize(QSize(30, 30));
    m_btn_switch_frds->setIcon(QIcon(":/Icon/users.png"));
    m_btn_switch_frds->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_btn_switch_frds->setCheckable(true);

    QVBoxLayout *layout = new QVBoxLayout(frame);
    layout->addWidget(m_avatar);
    layout->setAlignment(m_avatar, Qt::AlignHCenter);
    layout->addWidget(m_btn_switch_msgs);
    layout->addWidget(m_btn_switch_frds);
    layout->addStretch();
    layout->setContentsMargins(2,2,2,2);

    m_sideBar_btnGroup->addButton(m_btn_switch_msgs,0);
    m_sideBar_btnGroup->addButton(m_btn_switch_frds,1);
    connect(m_sideBar_btnGroup,&QButtonGroup::buttonClicked,this,[=](QAbstractButton *button){
        int index = m_sideBar_btnGroup->id(button);
        emit curIndex(index);
    });
    m_sideBar_btnGroup->button(0)->setChecked(true);


    connect(m_updateAvatar,&UpdateAvatar::sendUpdateAvatar,this,&SideBar::onSendUpdateAvatar);
    connect(m_updateAvatar,&UpdateAvatar::setAvatar,this,&SideBar::onSetAvatar);
    connect(this, &SideBar::successSetAvatar, m_updateAvatar, &UpdateAvatar::onSuccessSetAvatar);
    connect(m_avatar,&AvatarLabel::clicked, this,[=](){
        m_updateAvatar->loadUserAvatar();
        m_updateAvatar->exec();
    });
}

void SideBar::showAvatar()
{
    AvatarManager::getInstance()->getAvatar(CurrentUser::getInstance()->get_userId(), [=](const QPixmap &pix){
        if(pix.isNull()){
            return;
        }
        QPixmap roundedPixmap = CommonUtil::getRoundPix(pix);
        m_avatar->setPixmap(roundedPixmap);
    });


}

void SideBar::onSetAvatar(const QPixmap &pixmap)
{
    QPixmap roundedPixmap = CommonUtil::getRoundPix(pixmap);
    m_avatar->setPixmap(roundedPixmap);
    // setAvatar(roundedPixmap);
}

void SideBar::switchToChat()
{
    m_sideBar_btnGroup->button(0)->click();
}

void SideBar::showApplyRedDot()
{
    QLabel *redDot = m_btn_switch_frds->findChild<QLabel*>("redDot");
    if(!redDot){
        // 创建一个 QLabel 当作红点
        redDot = new QLabel(m_btn_switch_frds);
        redDot->setObjectName("redDot");
        redDot->setFixedSize(10, 10);
        redDot->move(m_btn_switch_frds->width()-15, m_btn_switch_frds->height()-12); // 右下角

        // 设置红点样式
        redDot->setStyleSheet("background-color: red; border-radius: 5px;");
        redDot->raise();
        redDot->show();
    }else{
        redDot->raise();
        redDot->show();
    }
}

void SideBar::hideApplyRedDot()
{
    QLabel *redDot = m_btn_switch_frds->findChild<QLabel*>("redDot");
    if(redDot){
        redDot->hide();
    }
}

void SideBar::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    showAvatar();
}

void SideBar::onSendUpdateAvatar(const QPixmap &pixmap)
{
    QPixmap pix = CommonUtil::getSquarePix(pixmap);
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    pix.save(&buffer, "PNG");

    Packet packet(CommonEnum::UPDATE_AVATAR,imageData);
    DataTransfer::getInstance()->sendData(packet);


    // QString newPath = ChatHistoryManager::getInstance()->storeAvatar(CurrentUser::getInstance()->get_userName(), imageData);
    // CurrentUser::getInstance()->set_avatarPath(newPath);
}
