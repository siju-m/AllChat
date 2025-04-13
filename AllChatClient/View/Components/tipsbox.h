#ifndef TIPSBOX_H
#define TIPSBOX_H

#include <QGraphicsDropShadowEffect>
#include <QWidget>
#include <QTimer>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QLabel>

// 设置提示的类型
enum MSG_TIP_TYPE
{
    SA_SUCCESS      ,    // 完成
    SA_FAILED       ,    // 错误
    SA_WARNING      ,    // 警告
    SA_TIPS         ,    // 提示
    SA_QUESTION          // 未知
};

class TipsBox : public QWidget
{
    Q_OBJECT

public:
    explicit TipsBox(const QString &text , const MSG_TIP_TYPE &type , const quint16 &duration , QWidget *parent = nullptr);
    ~TipsBox();

    static void showNotice(const QString &text , const MSG_TIP_TYPE &type , QWidget *parent = nullptr , const quint16 &duration = 2);
private:
    void ShadowEffectInit();
    void initAnimation(QWidget *parent);
private:
    QTimer *time;
    int m_nTipType;
    quint16 count = 0;
    QString m_text;
    QPropertyAnimation animation;
    QPropertyAnimation *fadeAnimation;
    int getWidX = 0 , getWidY = 0;

    QFrame *m_frame;
    QLabel *m_textLabel;
    QLabel *m_iconLabel;
};

#endif // TIPSBOX_H
