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

    vkDownloaderTXT = new QvkFileDownloader( this );
    vkDownloaderFile = new QvkFileDownloader( this );

    connect( ui->toolButtonHelpFullscreen, SIGNAL( clicked( bool ) ), this, SLOT( slot_screenFullscreen() ) );
}


void QvkHelpController::slot_screenFullscreen()
{
    ui->toolButtonHelpFullscreen->setDisabled( true );
    QUrl url( vk_helpPath + "screencast/screenFullscreen.txt" );
    vkDownloaderTXT->setMyUrl( url );
    disconnect( vkDownloaderTXT, 0, 0, 0 );
    connect( vkDownloaderTXT, SIGNAL( signal_downloaded() ), this, SLOT( slot_loadTXT() ) );
}


void QvkHelpController::slot_loadTXT()
{
    QByteArray byteArray = vkDownloaderTXT->downloadedData();
    QTextStream textStream( byteArray );
    int x = 0;
    while( textStream.atEnd() == false )
    {
        QString fileName = textStream.readLine();
        QUrl url( vk_helpPath + "screencast/" + fileName );
        //vkDownloaderFile->setMyUrl( url );
        //disconnect( vkDownloaderFile, 0, 0, 0 );
        //connect( vkDownloaderFile, SIGNAL( signal_downloaded() ), this, SLOT( slot_loadFile() ) );
        qDebug() << url;
    }
    ui->toolButtonHelpFullscreen->setEnabled( true );
}


void QvkHelpController::slot_loadFile()
{
    QByteArray byteArray = vkDownloaderFile->downloadedData();
    qDebug() << "******************************************************";
    qDebug() << byteArray;
    /*
        QPixmap pixmap;
        pixmap.loadFromData( vkFileDownloader->downloadedData() );
        pixmap.save( "/home/vk/Bilder/test.png" );

        QLabel *label = new QLabel();
        label->setText("<html>\n  <head>\n    <title></title>\n    <meta content=\"\">\n    <style>\n      .tux\n      {\n        margin: 0 auto;\n        width: 30;\n        position: fixed;\n        top: 0;        \n      }\n    </style>\n  </head>\n  </body>\n    <p> <img src=\"tux.png\" class=\"tux\"> </p>\n    <br>\n    <br>\n    <b>Description</b>\n    <ul>\n    The fullscreen function record a display that was selected in the dropdown box.\n    </ul>\n       <p> <img src=\"/home/vk/Bilder/test.png\"> </p>\n    <br>\n  </body>\n</html>\n\n\n");
        label->show();
    */

}

