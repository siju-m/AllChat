#ifndef USERDETAILVIEW_H
#define USERDETAILVIEW_H

#include <QWidget>

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
    void showUserInfo(const QString &name,const QString &id,const bool &state,const QString &avatarPath);
signals:
    void showMessage(const QString &id);
    void deleteFriend(const QString &id);
private:
    Ui::UserDetailView *ui;

    UserInfo m_userInfo;
};

#endif // USERDETAILVIEW_H
