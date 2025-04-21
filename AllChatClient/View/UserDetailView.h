#ifndef USERDETAILVIEW_H
#define USERDETAILVIEW_H

#include <QWidget>

#include <Model/user.h>

namespace Ui {
class UserDetailView;
}


class UserDetailView : public QWidget
{
    Q_OBJECT
public:
    explicit UserDetailView(QWidget *parent = nullptr);
    ~UserDetailView();

    struct UserInfo{
        QString name;
        QString id;
        bool state;
        QString avatarPath;
    };

    void showAvatar(const QString &path);
    void showUserInfo(const QString &id,const User &user);
    void hideUserInfo();
signals:
    void showMessage(const QString &id);
    void deleteFriend(const QString &id);
private:
    Ui::UserDetailView *ui;

    UserInfo m_userInfo;
};

#endif // USERDETAILVIEW_H
