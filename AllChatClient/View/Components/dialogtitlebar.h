#ifndef DIALOGTITLEBAR_H
#define DIALOGTITLEBAR_H

#include <QPushButton>
#include <QWidget>

class DialogTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit DialogTitleBar(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void closeWindow();

private:
    QPushButton *m_btnClose;
};

#endif // DIALOGTITLEBAR_H
