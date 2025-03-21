#include "imageviewer.h"


#include <QTimer>

ImageViewer::ImageViewer(const QPixmap &image, QWidget *parent)
    : QDialog(parent), m_scaleFactor(1.0) {

        // 窗口设置
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setMinimumSize(200, 200);

    // 初始化图片
    m_originalImage = image;
    m_currentImage = image;

    // 图片标签
    m_imageLabel = new QLabel(this);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    updateImageDisplay();

    // 关闭按钮（更美观的样式）
    QPushButton *closeBtn = new QPushButton("×", this);
    closeBtn->setStyleSheet(
        "QPushButton {"
        "  background: #ff4444;"
        "  color: white;"
        "  border-radius: 12px;"
        "  min-width: 24px;"
        "  min-height: 24px;"
        "  font-size: 18px;"
        "}"
        "QPushButton:hover { background: #ff6666; }"
        );
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::close);

    // 创建滚动区域
    scrollArea = new QScrollArea(this);
    scrollArea->viewport()->installEventFilter(this); // 监听视口事件
    scrollArea->setWidgetResizable(true);
    scrollArea->setAlignment(Qt::AlignCenter);  // 关键：设置居中对齐
    scrollArea->setWidget(m_imageLabel);

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(closeBtn, 0, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(scrollArea);

    // 初始适应窗口
    QTimer::singleShot(0, this, &ImageViewer::fitToWindow);
}

bool ImageViewer::eventFilter(QObject *obj, QEvent *event) {
    if (obj == scrollArea->viewport() && event->type() == QEvent::Wheel) {
        wheelEvent(static_cast<QWheelEvent*>(event)); // 转发到自定义处理
        return true; // 阻止事件继续传递
    }
    return QDialog::eventFilter(obj, event);
}

bool ImageViewer::event(QEvent *event) {
    return QDialog::event(event);
}

void ImageViewer::updateImageDisplay() {
    QSize newSize = m_originalImage.size() * m_scaleFactor;
    qreal dpr = m_imageLabel->devicePixelRatio();//高dpi适配
    m_currentImage = m_originalImage.scaled(
        newSize*dpr,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );
    m_currentImage.setDevicePixelRatio(dpr);
    m_imageLabel->setPixmap(m_currentImage);
    m_imageLabel->resize(newSize); // 必须调整QLabel尺寸
}

void ImageViewer::saveImage()
{
    QString path = QFileDialog::getSaveFileName(
        this, "保存图片", QDir::homePath(), "PNG Files (*.png);;JPEG Files (*.jpg)"
        );
    if (!path.isEmpty()) m_originalImage.save(path);
}

void ImageViewer::resetZoom()
{
    m_scaleFactor = 1.0;
    updateImageDisplay();
}

void ImageViewer::zoomIn()
{
    m_scaleFactor = qMin(m_scaleFactor * 1.2, 10.0);
    updateImageDisplay();
}

void ImageViewer::zoomOut()
{
    m_scaleFactor = qMax(m_scaleFactor * 0.8, 0.1);
    updateImageDisplay();
}

void ImageViewer::fitToWindow()
{
    if (m_imageLabel->width() <= 0 || m_imageLabel->height() <= 0) return;

    QSize targetSize = m_originalImage.size().scaled(
        m_imageLabel->size(), Qt::KeepAspectRatio
        );
    m_scaleFactor = qMin(
        targetSize.width() / (qreal)m_originalImage.width(),
        targetSize.height() / (qreal)m_originalImage.height()
        );
    updateImageDisplay();
}

void ImageViewer::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->globalPosition().toPoint();
        m_bDragging = true;
    }
    QDialog::mousePressEvent(event);
}

void ImageViewer::mouseMoveEvent(QMouseEvent *event)  {
    if (m_bDragging) {
        QPoint delta = event->globalPosition().toPoint() - m_dragStartPosition;
        move(pos() + delta);
        m_dragStartPosition = event->globalPosition().toPoint();
    }
}

void ImageViewer::mouseReleaseEvent(QMouseEvent *event) {
    m_bDragging = false;
    QDialog::mouseReleaseEvent(event);
}

void ImageViewer::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_Space:
        resetZoom();
        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    }
}

void ImageViewer::contextMenuEvent(QContextMenuEvent *event)  {
    QMenu menu(this);
    menu.addAction("保存图片", this, &ImageViewer::saveImage);
    menu.addAction("重置缩放", this, &ImageViewer::resetZoom);
    menu.addAction("适应窗口", this, &ImageViewer::fitToWindow);
    menu.exec(event->globalPos());
}

void ImageViewer::wheelEvent(QWheelEvent *event) {
    const double zoomStep = 0.2; // 每次滚轮步长缩放20%
    const QPointF mousePos = event->position(); // 鼠标相对窗口的位置

    // 计算鼠标在图片坐标系中的位置（相对比例）
    QPointF imgPos = m_imageLabel->mapFromParent(mousePos);
    double xRatio = imgPos.x() / m_imageLabel->width();
    double yRatio = imgPos.y() / m_imageLabel->height();

    // 计算缩放方向
    const int delta = event->angleDelta().y();
    if (delta > 0) {
        m_scaleFactor *= (1.0 + zoomStep);
    } else if (delta < 0) {
        m_scaleFactor *= (1.0 - zoomStep);
    }

    // 限制缩放范围 (0.1x ~ 10x)
    m_scaleFactor = qBound(0.1, m_scaleFactor, 10.0);

    // 应用缩放并保持鼠标位置不变
    updateImageDisplay();
    adjustScrollPosition(xRatio, yRatio);

    event->accept();
}

void ImageViewer::adjustScrollPosition(double xRatio, double yRatio) {
    QScrollArea *scrollArea = findChild<QScrollArea*>();
    if (!scrollArea || m_imageLabel->width() == 0 || m_imageLabel->height() == 0) return;

    // 计算相对于图片尺寸的目标位置
    const int targetX = xRatio * m_imageLabel->width();
    const int targetY = yRatio * m_imageLabel->height();

    // 计算视口可见区域的中心偏移
    const int viewportWidth = scrollArea->viewport()->width();
    const int viewportHeight = scrollArea->viewport()->height();

    // 关键：保持鼠标点位于视口同一相对位置
    scrollArea->horizontalScrollBar()->setValue(targetX - viewportWidth * xRatio);
    scrollArea->verticalScrollBar()->setValue(targetY - viewportHeight * yRatio);
}
