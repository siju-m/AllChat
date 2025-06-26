#include "searchresultlist.h"
#include "Delegate/friendsdelegate.h"

SearchResultList::SearchResultList(QWidget *parent) :
    QListView(parent)
{
    m_frd_model = new FriendsModel(this);
    FriendsDelegate *friends_delegate = new FriendsDelegate(this);

    this->setModel(m_frd_model);
    this->setItemDelegate(friends_delegate);

    this->setVerticalScrollMode(QListView::ScrollPerPixel); // 平滑滚动
    this->setResizeMode(QListView::Adjust);                 // 自动调整项大小
    this->setSelectionMode(QAbstractItemView::SingleSelection);  // 单选
    this->setSelectionBehavior(QAbstractItemView::SelectItems);

    this->setUniformItemSizes(true);//启用统一项尺寸优化

    connect(this, &QListView::clicked, this, [=](const QModelIndex &index){
        QString id = index.data(FriendsModel::IdRole).toString();
        emit clickedItem(id);
    });
}

void SearchResultList::clear()
{
    m_frd_model->clear();
    update();
}

void SearchResultList::addItem(const User &user)
{
    m_frd_model->addFriends_ToList(user.getUserName(),
                                   user.getUserId(),
                                   user.getOnlineState()?StateEnum::ONLINE:StateEnum::OFFLINE,
                                   user.getAvatarPath());
}
