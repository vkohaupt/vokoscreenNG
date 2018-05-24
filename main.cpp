#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

#include <gst/gst.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Initialize GStreamer
    gst_init (&argc, &argv);

    QTranslator * qtTranslator = new QTranslator();
    qtTranslator->load( "qt_" + QLocale::system().name(), QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
    app.installTranslator( qtTranslator );

    MainWindow w;
    w.show();

    return app.exec();
}
