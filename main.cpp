#include "mainwindow.h"
#include <QApplication>

#include <gst/gst.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialize GStreamer
    gst_init (&argc, &argv);

    MainWindow w;

    w.show();

    return a.exec();
}
