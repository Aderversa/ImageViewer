#include "ImageViewer.h"

#include <QApplication>
#include <QPainter>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageViewer w(QString("E:\\2.png"));
    w.show();
    return a.exec();
}
