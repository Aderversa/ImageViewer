#include "ImageViewer.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QApplication>
#include <QScrollBar>
#include <QPainter>
#include <QMargins>
#include <QLabel>

ImageViewer::ImageViewer(const QString& filename, QWidget* parent)
    : QWidget(parent)
    , image(filename)
{
    QSize viewBeginSize(1000, 500);
    QImage tempImage = image.scaled(viewBeginSize, Qt::KeepAspectRatioByExpanding);
    zoomFactor = tempImage.size().width() / qreal(image.size().width());
    minZoomFactor = zoomFactor;
    QSize viewportSize = tempImage.size();
    m_viewport = QRect(QPoint(0, 0), viewportSize);
    resize(tempImage.size());
}

ImageViewer::~ImageViewer() {}

void ImageViewer::changeCursor(Qt::CursorShape shape)
{
    QCursor cursor;
    cursor.setShape(shape);
    QApplication::setOverrideCursor(cursor);
}

void ImageViewer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (zoomFactor > minZoomFactor)
            changeCursor(Qt::ClosedHandCursor);
        lastPos = event->pos();
        event->accept();
    }
    QWidget::mousePressEvent(event);
}

void ImageViewer::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        event->accept();
        QPoint diff = event->pos() - lastPos;
        diff = -diff;
        lastPos = event->pos();
        QImage tempImage = image.scaled(image.size() * zoomFactor);
        QPoint topLeft;
        topLeft.setX(qMax(0, qMin(tempImage.size().width() - size().width(), m_viewport.left() + diff.x())));
        topLeft.setY(qMax(0, qMin(tempImage.size().height() - size().height(), m_viewport.top() + diff.y())));
        m_viewport.setTopLeft(topLeft);
        update();
    }
    QWidget::mouseMoveEvent(event);
}

void ImageViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (zoomFactor == minZoomFactor)
            changeCursor(Qt::ArrowCursor);
        else
            changeCursor(Qt::OpenHandCursor);
        event->accept();
    }
    QWidget::mouseReleaseEvent(event);
}

void ImageViewer::wheelEvent(QWheelEvent* event)
{
    changeCursor(Qt::OpenHandCursor);
    QPoint mousePos = event->position().toPoint() + m_viewport.topLeft();
    mousePos = mousePos / zoomFactor;
    QPoint diff =  event->position().toPoint();
    if (event->angleDelta().y() > 0) {
        zoomFactor = qMin(5.0, zoomFactor + 0.05);
    }
    else {
        zoomFactor = qMax(minZoomFactor, zoomFactor - 0.05);
    }
    if (zoomFactor == minZoomFactor)
        changeCursor(Qt::ArrowCursor);
    mousePos = mousePos * zoomFactor;
    QPoint topLeft = mousePos - diff;
    QImage tempImage = image.scaled(image.size() * zoomFactor);
    topLeft.setX(qMax(0, qMin(tempImage.size().width() - size().width(), topLeft.x())));
    topLeft.setY(qMax(0, qMin(tempImage.size().height() - size().height(), topLeft.y())));
    m_viewport.setTopLeft(topLeft);
    update();
    event->accept();
    QWidget::wheelEvent(event);
}

void ImageViewer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.translate(-m_viewport.topLeft().x(), -m_viewport.topLeft().y());
    QImage tempImage = image.scaled(image.size() * zoomFactor);
    painter.drawImage(tempImage.rect(), tempImage);
    event->accept();
    QWidget::paintEvent(event);
}

void ImageViewer::resizeEvent(QResizeEvent *event)
{
    event->accept();
    QSize newSize = event->size();
    QImage tempImage = image.scaled(newSize, Qt::KeepAspectRatio);
    zoomFactor = tempImage.size().width() / qreal(image.size().width());
    minZoomFactor = zoomFactor;
    QSize viewportSize = tempImage.size();
    m_viewport = QRect(QPoint(0, 0), viewportSize);
    resize(newSize);
    QWidget::resizeEvent(event);
}
