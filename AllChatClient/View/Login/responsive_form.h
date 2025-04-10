#ifndef RESPONSIVE_FORM_H
#define RESPONSIVE_FORM_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QResizeEvent>
#include "Utils/CustomTypes.h"
#include "scroll_bar.h"
#include "transparent_transition_interface.h"
#include "registration_form.h"
#include "login_form.h"

class Responsive_form : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int animation_duration READ animation_duration WRITE setAnimation_duration FINAL)

public:
    explicit Responsive_form(QWidget *parent = nullptr);

    Scroll_bar* scroll_bar;
    Transparent_transition_interface *transparent_transition_interface;
    Transparent_transition_interface* transparent_transition_interface2;
    Registration_form* registration_form;
    Login_form* login_form;

    void build_animation();
    QPropertyAnimation* animation2;
    QPropertyAnimation* animation3;
    QPropertyAnimation* animation4;
    QPropertyAnimation* animation5;
    QPropertyAnimation* animation6;

    int animation_duration() const;
    void setAnimation_duration(int newAnimation_duration);
    void updateMask();
    void createRoundPath(QPainterPath& path);
    void crop_corner();

    void closeWindow(CommonEnum::message_type result);
    void goToLoginPage();
signals:
    void login(const QString &username, const QString &password);
    void regist(const QString &username, const QString &password);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event);

public slots:
    void setRightShow();
    void execute_animation(Hollow_button::AnimationState status);
    void onAnimation3Finished();
    void onAnimation4Finished();

private:
    QPoint m_dragStartPosition;
    QPoint m_startWindowPosition;
    int currentSequence = 1;
    bool animation_execute_duration = false;
    bool animation_restore_duration = false;
    int m_animation_duration = 600;
};

#endif // RESPONSIVE_FORM_H
