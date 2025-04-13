#include "addfriends.h"
#include "View/Components/tipsbox.h"
#include "ui_addfriends.h"

#include <QGraphicsDropShadowEffect>
#include <QMessageBox>

#include <Core/currentuser.h>

AddFriends::AddFriends(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddFriends)
{
    ui->setupUi(this);
    this->setFixedSize(600,450);
    setWindowFlag(Qt::FramelessWindowHint);         //无边框
    setAttribute(Qt::WA_TranslucentBackground);     //窗口透明
    //设置投影效果
    QGraphicsDropShadowEffect *windowShadow;        //阴影效果
    windowShadow = new QGraphicsDropShadowEffect(this);
    windowShadow->setBlurRadius(30);
    windowShadow->setColor(QColor(0, 0, 0));
    windowShadow->setOffset(0, 0);
    ui->frame->setGraphicsEffect(windowShadow);

    connect(ui->titleBar, &DialogTitleBar::closeWindow, this, &AddFriends::close);

    connect(ui->btnSelect,&QPushButton::clicked,this,&AddFriends::slelect_byName);
    initStrangerList();

}

AddFriends::~AddFriends()
{
    delete ui;
}

void AddFriends::updateListView(QMap<QString, QString> id_name,QMap<QString,QString> id_avatar)
{
    m_stranger_model->clear();
    QList<QString> ids = id_name.keys();
    QMap<QString, User> &m_friendList = CurrentUser::getInstance()->getFriendList();
    for(auto &id:ids){
        //在列表显示
        m_stranger_model->addFriends_ToList(id_name[id],id,m_friendList.contains(id),id_avatar[id]);
    }
}

void AddFriends::closeEvent(QCloseEvent *event)
{
    ui->nameInputEdit->clear();
    m_stranger_model->clear();
    QDialog::closeEvent(event);
}

void AddFriends::slelect_byName()
{
    QString selectName =ui->nameInputEdit->text();
    emit send_slelectByName(selectName);
}

void AddFriends::initStrangerList()
{
    m_stranger_model = new StrangerModel(this);
    ui->strangerList->setModel(m_stranger_model);
    m_stranger_delegate = new StrangerDelegate(this);
    ui->strangerList->viewport()->setMouseTracking(true);
    ui->strangerList->setItemDelegate(m_stranger_delegate);
    ui->strangerList->setVerticalScrollMode(QListView::ScrollPerPixel); // 平滑滚动
    ui->strangerList->setResizeMode(QListView::Adjust);                 // 自动调整项大小
    ui->strangerList->setSelectionMode(QAbstractItemView::SingleSelection);  // 单选
    ui->strangerList->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->strangerList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(m_stranger_delegate,&StrangerDelegate::applyClicked,this,[=](const QString &id,const int &row){
        emit sendData(id);
        m_stranger_model->removeItem(row);
        // QMessageBox::information(this, "成功", "已发送好友申请!");
        TipsBox::showNotice("已发送好友申请!", SA_SUCCESS, this);
    });
    connect(m_stranger_delegate,&StrangerDelegate::sendClicked,this,[=](const QString &id,const int &row){
        emit showMessage(id);
        close();
    });
}

