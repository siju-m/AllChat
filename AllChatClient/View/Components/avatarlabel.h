#ifndef AVATARLABEL_H
#define AVATARLABEL_H

#include <QLabel>
#include <QObject>

class AvatarLabel : public QLabel
{
    Q_OBJECT
public:
    AvatarLabel(QWidget* parent = nullptr) ;

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // AVATARLABEL_H
