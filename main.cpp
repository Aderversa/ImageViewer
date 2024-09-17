#include "ImageViewer.h"

#include <QApplication>
#include <QPainter>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageViewer w(QString("E:\\1.jpg"));
    w.show();
    return a.exec();
}
