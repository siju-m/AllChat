#ifndef SEARCHRESULTLIST_H
#define SEARCHRESULTLIST_H

#include "Model/friendsmodel.h"
#include "Model/user.h"
#include <QListView>
#include <QObject>

class SearchResultList : public QListView
{
    Q_OBJECT
public:
    SearchResultList(QWidget *parent=nullptr);

    void clear();
    void addItem(const User &user);

signals:
    void clickedItem(const QString &id);

private:
    FriendsModel *m_frd_model;
};

#endif // SEARCHRESULTLIST_H
