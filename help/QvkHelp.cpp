#include "QvkHelp.h"

#include <QDesktopServices>
#include <QUrl>
#include <QMenu>

QvkHelp::QvkHelp( Ui_MainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;
}


QvkHelp::~QvkHelp()
{
}


void QvkHelp::slot_close()
{
#ifdef Q_OS_LINUX
    webEngineView->close();
#endif
}


void QvkHelp::initHelp()
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
    connect( ui->toolButtonHelpWindow, SIGNAL( clicked( bool ) ), this, SLOT( slot_screenWindow() ) );
    connect( ui->toolButtonHelpArea, SIGNAL( clicked( bool ) ), this, SLOT( slot_screenArea() ) );
    connect( ui->toolButtonHelpCountdown, SIGNAL( clicked( bool ) ), this, SLOT( slot_screenCountdown() ) );

    connect( ui->toolButtonAudioHelp, SIGNAL( clicked( bool ) ), this, SLOT( slot_audioHelp() ) );

    connect( ui->toolButtonHelpVideoPath, SIGNAL( clicked( bool ) ), this, SLOT( slot_miscHelpVideoPath() ) );
    connect( ui->toolButtonHelpStartTime, SIGNAL( clicked( bool ) ), this, SLOT( slot_miscHelpStartTime() ) );
    connect( ui->toolButtonHelpStopRecordingAfter, SIGNAL( clicked( bool ) ), this, SLOT( slot_miscHelpStopRecordingAfter() ) );
    connect( ui->toolButtonHelpScale, SIGNAL( clicked( bool ) ), this, SLOT( slot_miscHelpScal() ) );
    connect( ui->toolButtonHelpLimitOfFreeDiskSpace, SIGNAL( clicked( bool ) ), this, SLOT( slot_miscHelpLimitOfFreeDiskSpace() ) );

    connect( ui->toolButtonAvalaibleHelp, SIGNAL( clicked( bool ) ), SLOT( slot_availableHelp() ) );

#ifdef Q_OS_LINUX
    webEngineProfile = new QWebEngineProfile();
    webEnginePage = new QWebEnginePage( webEngineProfile );
    webEngineView = new QWebEngineView();

    QAction *action = webEnginePage->action( QWebEnginePage::CopyImageUrlToClipboard );
    action->setVisible( false );
    action = webEnginePage->action( QWebEnginePage::DownloadImageToDisk );
    action->setVisible( false );
    action = webEnginePage->action( QWebEnginePage::CopyImageToClipboard );
    action->setVisible( false );
    action = webEnginePage->action( QWebEnginePage::DownloadLinkToDisk );
    action->setVisible( false );
    action = webEnginePage->action( QWebEnginePage::CopyLinkToClipboard );
    action->setVisible( false );
    action = webEnginePage->action( QWebEnginePage::OpenLinkInThisWindow );
    action->setVisible( false );

    QIcon icon;
    icon.addFile( QString::fromUtf8( ":/pictures/vokoscreen.png" ), QSize(), QIcon::Normal, QIcon::Off );
    webEngineView->setWindowIcon( icon );
#endif
}


void QvkHelp::slot_screenFullscreen()
{
    QUrl url( vk_helpPath + "screencast/screenFullscreen.html", QUrl::TolerantMode );
#ifdef Q_OS_LINUX
    webEnginePage->load( url );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( url );
#endif
}


void QvkHelp::slot_screenWindow()
{
    QUrl url( vk_helpPath + "screencast/screenWindow.html", QUrl::TolerantMode );
#ifdef Q_OS_LINUX
    webEnginePage->load( url );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( url );
#endif
}


void QvkHelp::slot_screenArea()
{
    QUrl url( vk_helpPath + "screencast/screenArea.html", QUrl::TolerantMode );
#ifdef Q_OS_LINUX
    webEnginePage->load( url );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( url );
#endif
}


void QvkHelp::slot_screenCountdown()
{
    QUrl url( vk_helpPath + "screencast/screenCountdown.html", QUrl::TolerantMode );
#ifdef Q_OS_LINUX
    webEnginePage->load( url );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( url );
#endif
}


void QvkHelp::slot_audioHelp()
{
#ifdef Q_OS_LINUX
    webEnginePage->load( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/linux/3.0/screencast/audio.html") );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/windows/3.0/screencast/audio.html", QUrl::TolerantMode ) );
#endif
}

void QvkHelp::slot_miscHelpVideoPath()
{
#ifdef Q_OS_LINUX
    webEnginePage->load( QUrl("http://linuxecke.volkoh.de/vokoscreen/help/linux/3.0/screencast/misc.html#miscHelpVideoPath") );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/windows/3.0/screencast/misc.html#miscHelpVideoPath", QUrl::TolerantMode ) );
#endif
}

void QvkHelp::slot_miscHelpStartTime()
{
#ifdef Q_OS_LINUX
    webEnginePage->load( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/linux/3.0/screencast/misc.html#miscHelpStartTime" ) );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/windows/3.0/screencast/misc.html#miscHelpStartTime", QUrl::TolerantMode ) );
#endif
}

void QvkHelp::slot_miscHelpStopRecordingAfter()
{
#ifdef Q_OS_LINUX
    webEnginePage->load( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/linux/3.0/screencast/misc.html#miscHelpStopRecordingAfter" ) );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/windows/3.0/screencast/misc.html#miscHelpStopRecordingAfter", QUrl::TolerantMode ) );
#endif
}

void QvkHelp::slot_miscHelpScal()
{
#ifdef Q_OS_LINUX
    webEnginePage->load( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/linux/3.0/screencast/misc.html#miscHelpScal" ) );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/windows/3.0/screencast/misc.html#miscHelpScal", QUrl::TolerantMode ) );
#endif
}

void QvkHelp::slot_miscHelpLimitOfFreeDiskSpace()
{
#ifdef Q_OS_LINUX
    webEnginePage->load( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/linux/3.0/screencast/misc.html#miscHelpLimitOfFreeDiskSpace" ) );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/windows/3.0/screencast/misc.html#miscHelpLimitOfFreeDiskSpace", QUrl::TolerantMode ) );
#endif
}

void QvkHelp::slot_availableHelp()
{
#ifdef Q_OS_LINUX
    webEnginePage->load( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/linux/3.0/screencast/available.html" ) );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/windows/3.0/screencast/available.html", QUrl::TolerantMode ) );
#endif
}

