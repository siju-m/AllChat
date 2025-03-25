#ifndef MESSAGELISTVIEW_H
#define MESSAGELISTVIEW_H

#include <QListView>
#include <QScrollBar>
#include <QTimer>
#include <QWheelEvent>

class MessageListView : public QListView {
    Q_OBJECT

public:
    MessageListView(QWidget *parent = nullptr);
protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void showAndHideScrollBars();

    void hideScrollBars();

private:
    QTimer *hideTimer;
};


#endif // MESSAGELISTVIEW_H
