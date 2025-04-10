#ifndef INPUT_BOX_H
#define INPUT_BOX_H

#include <QLineEdit>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

class Input_box : public QLineEdit
{
    Q_OBJECT
public:
    explicit Input_box(QString icon, QWidget* parent = nullptr);

    // QString text();
protected:
    void paintEvent(QPaintEvent *event);

public:
    void crop_corner();
    void draw_ico_image();

private:
    QPixmap icon;
};

#endif // INPUT_BOX_H
