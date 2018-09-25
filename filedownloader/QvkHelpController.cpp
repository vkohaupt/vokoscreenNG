#include "QvkHelpController.h"


QvkHelpController::QvkHelpController( Ui_MainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;
}


QvkHelpController::~QvkHelpController()
{
}


void QvkHelpController::init()
{
    QStringList helpStringList;
#ifdef Q_OS_LINUX
    helpStringList << "http:/"
                   << "linuxecke.volkoh.de"
                   << "vokoscreen"
                   << "help"
                   << "linux"
                   << "3.0";
#endif
#ifdef Q_OS_WIN
    helpStringList << "http:/"
                   << "linuxecke.volkoh.de"
                   << "vokoscreen"
                   << "help"
                   << "windows"
                   << "3.0";
#endif
    vk_helpPath = helpStringList.join( "/" ).append( "/");

    connect( ui->toolButtonHelpFullscreen, SIGNAL( clicked( bool ) ), this, SLOT( slot_screenFullscreen() ) );
}


void QvkHelpController::slot_screenFullscreen()
{
    QUrl url( vk_helpPath + "screencast/screenFullscreen.png" );
    vkFileDownloader = new QvkFileDownloader( url, this );
    connect( vkFileDownloader, SIGNAL( signal_downloaded() ), this, SLOT( slot_loadImage() ) );
}


void QvkHelpController::slot_loadImage()
{
    QPixmap pixmap;
    pixmap.loadFromData( vkFileDownloader->downloadedData() );
    pixmap.save( "/home/vk/Bilder/test.png" );

    QLabel *label = new QLabel();
    label->setPixmap( pixmap );
    label->show();
}
