#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QvkMainWindow w;
    w.show();
    return a.exec();
}
