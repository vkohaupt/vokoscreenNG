#include "QvkHelp.h"

#include <QMessageBox>
#include <QLabel>
#include <QDir>
#include <QTimer>
#include <QTextDocument>

QvkHelp::QvkHelp( Ui_MainWindow *ui_mainwindow ) : uiForm(new(Ui::Form))
{
    ui = ui_mainwindow;
    uiForm->setupUi( this );

    resize( 800, 600 );
    setWindowTitle( "vokoscreen help" );
    QIcon icon;
    icon.addFile( QString::fromUtf8( ":/pictures/vokoscreen.png" ), QSize(), QIcon::Normal, QIcon::Off );
    setWindowIcon( icon );

    QStringList helpStringList;
    helpStringList << "http:/"
                   << "linuxecke.volkoh.de"
                   << "vokoscreen"
                   << "help"
                   << "3.0";

    vk_helpPath = helpStringList.join( "/" ).append( "/");
    QString language = "en";
    vk_helpPath = vk_helpPath + language + "/";

    vkDownloadHTML = new QvkDownloader();
    vkDownloadFiles = new QvkDownloader();

    connect( ui->toolButtonHelpFullscreen, SIGNAL( clicked( bool ) ), this, SLOT( slot_screenFullscreen() ) );
    connect( ui->toolButtonHelpWindow, SIGNAL( clicked( bool ) ), this, SLOT( slot_screenWindow() ) );
    connect( ui->toolButtonHelpArea, SIGNAL( clicked( bool ) ), this, SLOT( slot_screenArea() ) );
    connect( ui->toolButtonHelpMagnifier, SIGNAL( clicked( bool ) ), this, SLOT( slot_screenMagnifier() ) );
    connect( ui->toolButtonHelpShowclick, SIGNAL( clicked( bool ) ), this, SLOT( slot_screenShowclick() ) );
    connect( ui->toolButtonHelpCountdown, SIGNAL( clicked( bool ) ), this, SLOT( slot_screenCountdown() ) );

    connect( ui->toolButtonAudioHelp, SIGNAL( clicked( bool ) ), this, SLOT( slot_audioHelp() ) );

    connect( ui->toolButtonHelpVideoPath, SIGNAL( clicked( bool ) ), this, SLOT( slot_miscHelpVideoPath() ) );
    connect( ui->toolButtonHelpStartTime, SIGNAL( clicked( bool ) ), this, SLOT( slot_miscHelpStartTime() ) );
    connect( ui->toolButtonHelpStopRecordingAfter, SIGNAL( clicked( bool ) ), this, SLOT( slot_miscHelpStopRecordingAfter() ) );
    connect( ui->toolButtonHelpScale, SIGNAL( clicked( bool ) ), this, SLOT( slot_miscHelpScal() ) );
    connect( ui->toolButtonHelpLimitOfFreeDiskSpace, SIGNAL( clicked( bool ) ), this, SLOT( slot_miscHelpLimitOfFreeDiskSpace() ) );

    connect( ui->toolButtonAvalaibleHelp, SIGNAL( clicked( bool ) ), this, SLOT( slot_availableHelp() ) );

}


QvkHelp::~QvkHelp()
{
}

void QvkHelp::slot_screenFullscreen()
{
    loadHTML( vk_helpPath + "screencast/tab1-fullscreen.html" );
}


void QvkHelp::slot_screenWindow()
{
    loadHTML( vk_helpPath + "screencast/tab1-window.html" );
}


void QvkHelp::slot_screenArea()
{
    loadHTML( vk_helpPath + "screencast/tab1-area.html" );
}


void QvkHelp::slot_screenMagnifier()
{
    loadHTML( vk_helpPath + "screencast/tab1-magnifier.html" );
}


void QvkHelp::slot_screenShowclick()
{
    loadHTML( vk_helpPath + "screencast/tab1-showclick.html" );
}


void QvkHelp::slot_screenCountdown()
{
    loadHTML( vk_helpPath + "screencast/screenCountdown.html" );
}


void QvkHelp::slot_audioHelp()
{
    loadHTML( vk_helpPath + "screencast/tab2-audio.html" );
}


/*
 * To first we load the html file ...
 */
void QvkHelp::loadHTML( QString value )
{
    QFileInfo fileInfo( value );
    remotePath = fileInfo.path();
    remoteBasename = fileInfo.baseName();
    disconnect( vkDownloadHTML, 0, 0, 0 );
    connect( vkDownloadHTML, SIGNAL( signal_fileDownloaded( QString ) ), this, SLOT( slot_parseHTML( QString ) ) );
    vkDownloadHTML->doDownload( value );
}


int QvkHelp::getCountPNG( QString tempPathFileName )
{
    QFile file( tempPathFileName );
    if( !file.open( QIODevice::ReadOnly ) )
    {
        QMessageBox::information( 0, "Help error", file.errorString() );
    }

    int count = 0;
    QTextStream textStream( &file );
    while( !textStream.atEnd() )
    {
        QString line = textStream.readLine();
        if ( line.contains( "png", Qt::CaseInsensitive ) == true )
        {
            count++;
        }
    }
    file.close();
    return count;
}


/*
 * ... then we parse *.html of png´s and download all files from remote Url and save local in tmp ...
 */
void QvkHelp::slot_parseHTML( QString tempPathFileName )
{
    localFiles.clear();
    localFiles << tempPathFileName;

    QFileInfo fileInfo( tempPathFileName );
    QString tmpPath = fileInfo.absolutePath();


    QFile file( tempPathFileName );
    if( !file.open( QIODevice::ReadOnly ) )
    {
        QMessageBox::information( 0, "error", file.errorString() );
    }

    int countPNG = getCountPNG( tempPathFileName );
    qDebug() << "[vokoscreen] HTML file parsed, downloading" << countPNG << "picture";
    int counter = 0;
    QTextStream textStream( &file );
    while( !textStream.atEnd() )
    {
        QString line = textStream.readLine();
        if ( line.contains( ".png", Qt::CaseInsensitive ) or
             line.contains( ".jpg", Qt::CaseInsensitive ) or
             line.contains( ".svg", Qt::CaseInsensitive ) or
             line.contains( ".css", Qt::CaseInsensitive ) )
        {
            counter++;
            QString fileForHTML = line.section( "\"", 1, 1 );
            if ( counter == countPNG )
            {
                disconnect( vkDownloadFiles, 0, 0, 0 );
                connect( vkDownloadFiles, SIGNAL( signal_fileDownloaded( QString ) ), this, SLOT( slot_showHelp( QString ) ) );
            }
            vkDownloadFiles->doDownload( remotePath + "/" + fileForHTML );
            localFiles << tmpPath + + "/" + fileForHTML;
        }
    }

    if ( counter == 0 )
    {
        // "dummy.png" is a fake, we need this if no picture is existing
        slot_showHelp( tmpPath + "/" + "dummy.png");
    }

    file.close();
}

/*
 * ... then we show the html file
 */
void QvkHelp::slot_showHelp( QString tempPathFileName )
{
    QDir dir;
    QString currentdir = dir.currentPath();

    QFileInfo fileInfo( tempPathFileName );
    QString tmpPath = fileInfo.absolutePath();
    dir.setCurrent( tmpPath );

    QString htmlFile = tmpPath + "/" + remoteBasename + ".html";
    QFile file( htmlFile );
    if( !file.open( QIODevice::ReadOnly ) )
    {
        QMessageBox::information( 0, "error", file.errorString() );
    }

    QTextStream textStream( &file );
    QString value = textStream.readAll();
    uiForm->textBrowser->setText( value );
    file.close();

    show();
    qDebug() << "[vokoscreen] Show help";

    dir.setCurrent( currentdir );

    // remove all tmp files
    for ( int i = 0; i < localFiles.count(); i++  )
    {
        QFile file( localFiles.at( i ) );
        file.remove();
    }
}

