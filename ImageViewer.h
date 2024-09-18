#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QScrollArea>
#include <QScrollBar>

class ImageViewer : public QWidget
{
    Q_OBJECT
public:
    ImageViewer(const QString& filename, QWidget* parent = nullptr);
    ~ImageViewer();
    void changeCursor(Qt::CursorShape shape);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent* event);
    void resizeEvent(QResizeEvent *event);

private:
    QImage image;
    QRect m_viewport;
    QPoint lastPos;
    qreal zoomFactor;
    qreal minZoomFactor;
};

#endif // IMAGEVIEWER_H
