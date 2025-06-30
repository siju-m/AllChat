#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "View/Components/avatarlabel.h"
#include "View/updateavatar.h"
#include <QButtonGroup>
#include <QToolButton>
#include <QWidget>

class SideBar : public QWidget
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = nullptr);

    void showAvatar();
    void onSetAvatar(const QPixmap &pixmap);
    void switchToChat();

    void showApplyRedDot();
    void hideApplyRedDot();

protected:
    void showEvent(QShowEvent* event) override;

private:
    void onSendUpdateAvatar(const QPixmap &pixmap);

signals:
    void curIndex(int index);
    void successSetAvatar();

private:
    AvatarLabel *m_avatar;
    QToolButton *m_btn_switch_msgs;
    QToolButton *m_btn_switch_frds;
    QButtonGroup *m_sideBar_btnGroup;//管理侧边栏按钮
    UpdateAvatar *m_updateAvatar;

    QPair<QString, QPixmap> m_last_avatar;
};

#endif // SIDEBAR_H
