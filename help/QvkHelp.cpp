#include "QvkHelp.h"

#include <QDesktopServices>
#include <QUrl>


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

    QIcon icon;
    icon.addFile( QString::fromUtf8( ":/pictures/vokoscreen.png" ), QSize(), QIcon::Normal, QIcon::Off );
    webEngineView->setWindowIcon( icon );
#endif
}


void QvkHelp::slot_screenCountdown()
{
#ifdef Q_OS_LINUX
    webEnginePage->load( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/linux/3.0/screenCountdown.html") );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/windows/3.0/screenCountdown.html", QUrl::TolerantMode ) );
#endif
}


void QvkHelp::slot_audioHelp()
{
#ifdef Q_OS_LINUX
    webEnginePage->load( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/linux/3.0/audio.html") );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/windows/3.0/audio.html", QUrl::TolerantMode ) );
#endif
}

void QvkHelp::slot_miscHelpVideoPath()
{
#ifdef Q_OS_LINUX
    webEnginePage->load( QUrl("http://linuxecke.volkoh.de/vokoscreen/help/linux/3.0/misc.html#miscHelpVideoPath") );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/windows/3.0/misc.html#miscHelpVideoPath", QUrl::TolerantMode ) );
#endif
}

void QvkHelp::slot_miscHelpStartTime()
{
#ifdef Q_OS_LINUX
    webEnginePage->load( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/linux/3.0/misc.html#miscHelpStartTime" ) );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/windows/3.0/misc.html#miscHelpStartTime", QUrl::TolerantMode ) );
#endif
}

void QvkHelp::slot_miscHelpStopRecordingAfter()
{
#ifdef Q_OS_LINUX
    webEnginePage->load( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/linux/3.0/misc.html#miscHelpStopRecordingAfter" ) );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/windows/3.0/misc.html#miscHelpStopRecordingAfter", QUrl::TolerantMode ) );
#endif
}

void QvkHelp::slot_miscHelpScal()
{
#ifdef Q_OS_LINUX
    webEnginePage->load( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/linux/3.0/misc.html#miscHelpScal" ) );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/windows/3.0/misc.html#miscHelpScal", QUrl::TolerantMode ) );
#endif
}

void QvkHelp::slot_miscHelpLimitOfFreeDiskSpace()
{
#ifdef Q_OS_LINUX
    webEnginePage->load( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/linux/3.0/misc.html#miscHelpLimitOfFreeDiskSpace" ) );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/windows/3.0/misc.html#miscHelpLimitOfFreeDiskSpace", QUrl::TolerantMode ) );
#endif
}

void QvkHelp::slot_availableHelp()
{
#ifdef Q_OS_LINUX
    webEnginePage->load( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/linux/3.0/available.html" ) );
    webEngineView->setPage( webEnginePage );
    webEngineView->show();
#endif
#ifdef Q_OS_WIN
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/windows/3.0/available.html", QUrl::TolerantMode ) );
#endif
}

