#ifndef ADDFRIENDS_H
#define ADDFRIENDS_H

#include <QDialog>
#include "Model\friendsmodel.h"
#include "Delegate\strangerdelegate.h"
#include <QObject>

namespace Ui {
class AddFriends;
}

class AddFriends : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriends(QWidget *parent = nullptr);
    ~AddFriends();

    void updateListView(QMap<QString,QString> id_name);
protected:
    void closeEvent(QCloseEvent *event) override;
signals:
    void send_slelectByName(const QString &username);
    void sendData(const QString &id);
public slots:
    void slelect_byName();
private:
    Ui::AddFriends *ui;

    FriendsModel *m_stranger_model;
    StrangerDelegate *m_stranger_delegate;

    void initStrangerList();
};

#endif // ADDFRIENDS_H
