#include "mainwindow.h"

#include <QApplication>

#include <gst/gst.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    gst_init (&argc, &argv);

    QvkMainWindow w;
    w.show();
    return a.exec();
}
