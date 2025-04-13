#ifndef CUSTOMTITLEBAR_H
#define CUSTOMTITLEBAR_H

#include <QLabel>
#include <QPushButton>
#include <QWidget>

class CustomTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit CustomTitleBar(QWidget *parent = nullptr);

signals:
    void minimizeClicked();
    void maximizeClicked();
    void closeClicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QLabel *m_titleLabel;
    QWidget *m_borderWidget;
    QPushButton *m_minimizeButton;
    QPushButton *m_maximizeButton;
    QPushButton *m_closeButton;

};

#endif // CUSTOMTITLEBAR_H
