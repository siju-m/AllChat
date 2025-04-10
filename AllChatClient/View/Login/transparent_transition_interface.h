#ifndef TRANSPARENT_TRANSITION_INTERFACE_H
#define TRANSPARENT_TRANSITION_INTERFACE_H

#include <QWidget>
#include <QPainter>
#include <QIntValidator>
#include <QRegularExpressionValidator>
#include <QPropertyAnimation>
#include "hollow_button.h"

class Transparent_transition_interface : public QWidget
{
    Q_OBJECT
public:
    explicit Transparent_transition_interface(QString large_text, QString small_text, QString btn_text, QWidget *parent = nullptr);
    void draw_text();
    void draw_text2();
    Hollow_button *button;

protected:
    void paintEvent(QPaintEvent* event);

private:
    QString large_text;
    QString small_text;
};

#endif // TRANSPARENT_TRANSITION_INTERFACE_H
