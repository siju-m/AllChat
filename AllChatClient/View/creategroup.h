#ifndef CREATEGROUP_H
#define CREATEGROUP_H

#include "Model/friendsmodel.h"
#include <QDialog>
#include <QFrame>
#include <QListView>

class CreateGroup : public QDialog
{
    Q_OBJECT
public:
    explicit CreateGroup(FriendsModel *model, QWidget *parent = nullptr);

signals:
    // void sendCreateGroup(QVector<QString> ids);

private:
    void initFrame();
    void initButtons();
    void initFriendList();

    void createGroup();

private:
    QFrame *m_frame;
    QPushButton *m_btnConfirm;
    QPushButton *m_btnCancel;
    QListView *m_friendList;
    FriendsModel *m_model;
};

#endif // CREATEGROUP_H
