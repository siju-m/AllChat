#ifndef ADDFRIENDS_H
#define ADDFRIENDS_H

#include <QDialog>
#include "Delegate\strangerdelegate.h"
#include <QObject>
#include <Model/strangermodel.h>

namespace Ui {
class AddFriends;
}

class AddFriends : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriends(QWidget *parent = nullptr);
    ~AddFriends();

    void updateListView(QMap<QString,QString> id_name,QMap<QString,QString> id_avatar);
protected:
    void closeEvent(QCloseEvent *event) override;
signals:
    void send_slelectByName(const QString &username);
    void sendData(const QString &id);
    void showMessage(const QString &id);
public slots:
    void slelect_byName();
private:
    Ui::AddFriends *ui;

    //todo 需要判断搜索出来的是不是好友
    //是就显示发消息按钮，不是就添加按钮
    StrangerModel *m_stranger_model;
    StrangerDelegate *m_stranger_delegate;

    void initStrangerList();
};

#endif // ADDFRIENDS_H
