#include "QvkHelp.h"

#include <QMessageBox>
#include <QLabel>
#include <QDir>
#include <QTimer>
#include <QTextDocument>

QvkHelp::QvkHelp(QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow ) : uiFormHelp(new(Ui::formHelp))
{
    connect( mainWindow, SIGNAL( destroyed( QObject*) ), this, SLOT( slot_close() ) );

    ui = ui_mainwindow;
    uiFormHelp->setupUi( this );

    resize( 800, 600 );
    setWindowTitle( "vokoscreen help" );
    QIcon icon;
    icon.addFile( QString::fromUtf8( ":/pictures/vokoscreen.png" ), QSize(), QIcon::Normal, QIcon::Off );
    setWindowIcon( icon );

    QStringList helpStringList;
    helpStringList << "http:/"
                   << "vokoscreen.volkoh.de"
                   << "help"
                   << "3.0";

    vk_helpPath = helpStringList.join( "/" ).append( "/");
    QString language = "en";
    vk_helpPath = vk_helpPath + language + "/";

    vkDownloadHTML = new QvkDownloader( temporaryDirLocal.path() );
    vkDownloadFiles = new QvkDownloader( temporaryDirLocal.path() );

    connect( ui->toolButtonHelpFullscreen, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonHelpFullscreen() ) );
    connect( ui->toolButtonHelpWindow, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonHelpWindow() ) );
    connect( ui->toolButtonHelpArea, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonHelpArea() ) );
    connect( ui->toolButtonHelpMagnifier, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonHelpMagnifier() ) );
    connect( ui->toolButtonHelpShowclick, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonHelpShowclick() ) );
    connect( ui->toolButtonHelpCountdown, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonHelpCountdown() ) );

    connect( ui->toolButtonAudioHelp, SIGNAL( clicked( bool ) ), this, SLOT( slot_audioHelp() ) );

    //connect( ui->toolButtonHelpVideoPath, SIGNAL( clicked( bool ) ), this, SLOT( slot_miscHelpVideoPath() ) );
    //connect( ui->toolButtonHelpStartTime, SIGNAL( clicked( bool ) ), this, SLOT( slot_miscHelpStartTime() ) );
    //connect( ui->toolButtonHelpStopRecordingAfter, SIGNAL( clicked( bool ) ), this, SLOT( slot_miscHelpStopRecordingAfter() ) );
    //connect( ui->toolButtonHelpScale, SIGNAL( clicked( bool ) ), this, SLOT( slot_miscHelpScal() ) );
    //connect( ui->toolButtonHelpLimitOfFreeDiskSpace, SIGNAL( clicked( bool ) ), this, SLOT( slot_miscHelpLimitOfFreeDiskSpace() ) );

    //connect( ui->toolButtonAvalaibleHelp, SIGNAL( clicked( bool ) ), this, SLOT( slot_availableHelp() ) );
}


QvkHelp::~QvkHelp()
{
}


void QvkHelp::slot_close()
{
    temporaryDirLocal.remove();
}


void QvkHelp::slot_toolButtonHelpFullscreen()
{
    loadHTML( vk_helpPath + "screencast/toolButtonHelpFullscreen.html" );
}


void QvkHelp::slot_toolButtonHelpWindow()
{
    loadHTML( vk_helpPath + "screencast/toolButtonHelpWindow.html" );
}


void QvkHelp::slot_toolButtonHelpArea()
{
    loadHTML( vk_helpPath + "screencast/toolButtonHelpArea.html" );
}


void QvkHelp::slot_toolButtonHelpMagnifier()
{
    loadHTML( vk_helpPath + "screencast/toolButtonHelpMagnifier.html" );
}


void QvkHelp::slot_toolButtonHelpShowclick()
{
    loadHTML( vk_helpPath + "screencast/toolButtonHelpShowclick.html" );
}


void QvkHelp::slot_toolButtonHelpCountdown()
{
    loadHTML( vk_helpPath + "screencast/toolButtonHelpCountdown.html" );
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


bool QvkHelp::isFileInLine( QString line )
{
    bool value = false;
    for ( int i = 0; i < toDownloadFiles.count(); i++ )
    {
        if ( line.contains( toDownloadFiles.at(i), Qt::CaseInsensitive ) )
        {
            value = true;
            break;
        }
        else
        {
            value = false;
        }
    }
    return value;
}


int QvkHelp::getCountFileToDownload( QString tempPathFileName )
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
        if ( isFileInLine( line ) )
        {
            count++;
        }
    }
    file.close();
    return count;
}


/*
 * ... then we parse *.html of files and download all files from remote Url and save local in tmp ...
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

    int countFiles = getCountFileToDownload( tempPathFileName );
    qDebug() << "[vokoscreen] HTML file parsed, downloading" << countFiles << "files";
    int counter = 0;
    QTextStream textStream( &file );
    while( !textStream.atEnd() )
    {
        QString line = textStream.readLine();
        if ( isFileInLine( line ) )
        {
            QString fileForHTML = line.section( "\"", 1, 1 );
            counter++;
            if ( counter == countFiles )
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
        // "dummy.png" is a fake, we need this if no file is downlowded
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
    uiFormHelp->textBrowser->setText( value );
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

