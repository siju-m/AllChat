#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QDialog>
#include <QEvent>
#include <QFileDialog>
#include <QGestureEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>

//图片放大窗口
class ImageViewer : public QDialog {
public:
    explicit ImageViewer(const QPixmap &image, QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

    bool event(QEvent *event) override;

    // 键盘交互
    void keyPressEvent(QKeyEvent *event) override ;

    // 右键菜单
    void contextMenuEvent(QContextMenuEvent *event) override ;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    void adjustScrollPosition(double xRatio, double yRatio);


    void updateImageDisplay();
private slots:
    void saveImage();

    void resetZoom();

    void zoomIn() ;

    void zoomOut() ;

    void fitToWindow();
private:
    QLabel *m_imageLabel;
    QPixmap m_originalImage;
    QPixmap m_currentImage;
    qreal m_scaleFactor;
    QScrollArea *scrollArea;

    QPoint m_lastMousePos;
    bool m_isDragging = false;
};

#endif // IMAGEVIEWER_H
