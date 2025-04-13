#include "input_box.h"

Input_box::Input_box(QString icon, QWidget* parent) : QLineEdit{ parent }
{
    this->icon = QPixmap(icon);
    this->resize(308, 58);
    setTextMargins(25, 0, 25, 0);

    QFont font = this->font();
    font.setPointSize(15);
    setFont(font);

    QPalette palette = this->palette();
    palette.setColor(QPalette::PlaceholderText, Qt::gray);
    this->setPalette(palette);

    palette.setColor(QPalette::Text, Qt::black);
    this->setPalette(palette);

    palette.setColor(QPalette::Base, Qt::transparent);
    this->setPalette(palette);

    this->setFrame(QFrame::NoFrame);
}

void Input_box::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setViewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());

    crop_corner();
    draw_ico_image();
    QLineEdit::paintEvent(event);
}

void Input_box::crop_corner()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(0, 0, width(), height(), 10, 10);
    painter.setClipPath(path);
    painter.setPen(Qt::NoPen);
    QBrush Brush(QColor(238, 238, 238, 255));
    painter.setBrush(Brush);
    painter.drawRect(0, 0, width(), height());
}

void Input_box::draw_ico_image()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QRect rect(width() * 0.85, height() / 4, height() / 2, height() / 2);
    painter.drawPixmap(rect, icon);
}
