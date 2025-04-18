#include "creategroup.h"
#include "Core/datatransfer.h"
#include "Delegate/friendsdelegate.h"

#include <QGraphicsDropShadowEffect>
#include <QPushButton>
#include <QScrollBar>
#include <QStringListModel>
#include <qboxlayout.h>

CreateGroup::CreateGroup(FriendsModel *model, QWidget *parent)
    : QDialog{parent}
    , m_model(model)
{
    this->resize(400, 300);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setAttribute(Qt::WA_TranslucentBackground);     //窗口透明
    this->setMinimumSize(250, 200);

    initFrame();


    initButtons();
    connect(m_btnConfirm, &QPushButton::clicked, this, &CreateGroup::createGroup);
    connect(m_btnCancel, &QPushButton::clicked, this, &CreateGroup::close);

    initFriendList();

    QWidget *btnGroup = new QWidget(this);
    QHBoxLayout *layout2 = new QHBoxLayout(btnGroup);
    layout2->addStretch();
    layout2->addWidget(m_btnConfirm);
    layout2->addWidget(m_btnCancel);

    QVBoxLayout *layout3 = new QVBoxLayout(m_frame);
    layout3->addWidget(m_friendList);
    layout3->addWidget(btnGroup);
}

void CreateGroup::initFrame()
{
    // 布局
    QVBoxLayout *layout1 = new QVBoxLayout(this);
    m_frame = new QFrame(this);
    //设置投影效果
    QGraphicsDropShadowEffect *windowShadow;        //阴影效果
    windowShadow = new QGraphicsDropShadowEffect(this);
    windowShadow->setBlurRadius(30);
    windowShadow->setColor(QColor(0, 0, 0));
    windowShadow->setOffset(0, 0);
    m_frame->setGraphicsEffect(windowShadow);
    layout1->addWidget(m_frame);
    layout1->setContentsMargins(20, 20, 20, 20);
    m_frame->setStyleSheet("QFrame{ border-radius:10px; background-color: rgb(241, 241, 241);}");
}

void CreateGroup::initButtons()
{
    m_btnConfirm = new QPushButton("确定", this);
    m_btnCancel = new QPushButton("取消", this);
    QString style("QPushButton{"
                  "font: 10pt '微软雅黑';"
                  "color: white;"
                  "background-color: rgb(53, 93, 144);"
                  "color: rgb(255, 255, 255);"
                  "border-radius: 8px;"
                  "border-style: none;"
                  "border-width: 1px;"
                  "padding: 5px 10px 5px 10px;"
                  "}"

                  /**鼠标停留在按钮上的样式**/
                  "QPushButton::hover{	"
                  "color: #FFFFFF;"
                  "background-color: rgb(93, 125, 166);"
                  "border-color: #2f3640;"
                  "}"

                  /**鼠标按压下去的样式**/
                  "QPushButton::pressed,QPushButton::checked{"
                  "color: #FFFFFF;"
                  "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #273c75, stop:1 #487eb0);"
                  "}"

                  /**按钮失能情况下样式**/
                  "QPushButton::disabled{"
                  "color: #FFFFFF;"
                  "background-color: #dcdde1;"
                  "border-color: #dcdde1;"
                  "}");
    m_btnConfirm->setStyleSheet(style);
    m_btnCancel->setStyleSheet(style);


}

void CreateGroup::initFriendList()
{
    m_friendList = new QListView(this);
    FriendsDelegate *friends_delegate = new FriendsDelegate(this);
    m_friendList->setModel(m_model);
    m_friendList->setItemDelegate(friends_delegate);
    m_friendList->setVerticalScrollMode(QListView::ScrollPerPixel); // 平滑滚动
    m_friendList->setResizeMode(QListView::Adjust);                 // 自动调整项大小
    m_friendList->setSelectionMode(QAbstractItemView::MultiSelection);

    QScrollBar *verticalScrollBar = m_friendList->verticalScrollBar();
    verticalScrollBar->setSingleStep(5); // 垂直滚动步长
    verticalScrollBar->setPageStep(100);  // 垂直滚动一页的距离
}

void CreateGroup::createGroup()
{
    QModelIndexList selectedIndexes = m_friendList->selectionModel()->selectedIndexes();
    QVector<QString> ids;
    for(const auto &it : selectedIndexes){
        QString id = m_model->data(it, FriendsModel::IdRole).toString();
        ids << id;
    }
    // emit sendCreateGroup(ids);
    // 请求数据在内部进行
    DataTransfer::getInstance()->sendData(Packet(CommonEnum::CreateGroup, ids));
    this->close();
}
