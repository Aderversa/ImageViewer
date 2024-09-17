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
    QSize viewportSize = image.size() / 3;
    m_viewport = QRect(QPoint(0, 0), viewportSize);
    resize(m_viewport.size());
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
        changeCursor(Qt::ClosedHandCursor);
        lastPos = event->pos();
        event->accept();
    }
    QWidget::mousePressEvent(event);
}

void ImageViewer::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        changeCursor(Qt::ClosedHandCursor);
        event->accept();
        QPoint diff = event->pos() - lastPos;
        diff = -diff;
        lastPos = event->pos();
        QPoint topLeft;
        topLeft.setX(qMax(0, qMin(image.size().width() * 2 / 3, m_viewport.left() + diff.x())));
        topLeft.setY(qMax(0, qMin(image.size().height() * 2 / 3, m_viewport.top() + diff.y())));
        m_viewport.setTopLeft(topLeft);
        update();
    }
    QWidget::mouseMoveEvent(event);
}

void ImageViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        changeCursor(Qt::ArrowCursor);
        event->accept();
    }
    QWidget::mouseReleaseEvent(event);
}

void ImageViewer::wheelEvent(QWheelEvent* event)
{
}

void ImageViewer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.translate(-m_viewport.topLeft().x(), -m_viewport.topLeft().y());
    painter.drawImage(image.rect(), image);
    event->accept();
    QWidget::paintEvent(event);
}
