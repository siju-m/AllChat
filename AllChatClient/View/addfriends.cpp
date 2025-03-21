#include "addfriends.h"
#include "ui_addfriends.h"

#include <QMessageBox>

AddFriends::AddFriends(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddFriends)
{
    ui->setupUi(this);
    connect(ui->btnSelect,&QPushButton::clicked,this,&AddFriends::slelect_byName);
    initStrangerList();
}

AddFriends::~AddFriends()
{
    delete ui;
}

void AddFriends::updateListView(QMap<QString, QString> id_name)
{
    m_stranger_model->clear();
    QList<QString> ids = id_name.keys();
    for(auto &id:ids){
        //在列表显示
        m_stranger_model->addFriends_ToList(id_name[id],id,"1","");
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
    m_stranger_model = new FriendsModel(this);
    ui->strangerList->setModel(m_stranger_model);
    m_stranger_delegate = new StrangerDelegate(this);
    ui->strangerList->setItemDelegate(m_stranger_delegate);
    ui->strangerList->setVerticalScrollMode(QListView::ScrollPerPixel); // 平滑滚动
    ui->strangerList->setResizeMode(QListView::Adjust);                 // 自动调整项大小
    ui->strangerList->setSelectionMode(QAbstractItemView::SingleSelection);  // 单选
    ui->strangerList->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->strangerList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(m_stranger_delegate,&StrangerDelegate::applyClicked,this,[=](const QString &id,const int &row){
        emit sendData(id);
        m_stranger_model->removeItem(row);
        QMessageBox::information(this, "成功", "已发送好友申请!");
    });
}

