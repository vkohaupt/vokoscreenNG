#include "ui_QvkNoPlayerDialog.h"

#include "QvkScreenshot.h"
#include "QvkWinInfo.h"
#include "QvkCountdown.h"
#include "QvkRegionChoise.h"

#include <QImageWriter>
#include <QApplication>
#include <QScreen>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QDialog>
#include <QFileDialog>
#include <QStandardPaths>
#include <QIcon>

QvkScreenshot::QvkScreenshot(MainWindow *value, Ui_MainWindow *ui_mainwindow ):vkWinInfo(new QvkWinInfo),
                                                                               vkCountdown(new QvkCountdown),
                                                                               regionChoise(new QvkRegionChoise)
{
    parent = value;
    ui = ui_mainwindow;

    QDesktopWidget *desk = QApplication::desktop();
    connect( desk, SIGNAL( screenCountChanged(int) ), this, SLOT( slot_Screenshot_count_changed( int ) ) );
    connect( desk, SIGNAL( resized( int ) ),          this, SLOT( slot_Screenshot_count_changed( int ) ) );
    emit desk->screenCountChanged(0);

    ui->radioButtonScreenshotFullscreen->setText( tr("Fullscreen") ); // QT Creator sets an ampersand, translation now here
    ui->radioButtonScreenshotWindow->setText( tr( "Window") ); // QT Creator sets an ampersand, translation now here

    ui->toolButtonScreenshotHelp->setIcon( ui->toolButtonScreenshotHelp->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
    ui->toolButtonScreenShotMiscHelp->setIcon( ui->toolButtonScreenShotMiscHelp->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );

    pictureFileSystemWatcher = new QFileSystemWatcher();
    connect( ui->toolButtonPicturePath, SIGNAL( clicked( bool ) ),        this, SLOT( slot_newPicturePath() ) );
    connect( ui->lineEditPicturePath,   SIGNAL( textChanged( QString ) ), this, SLOT( slot_pictureFileSystemWatcherSetNewPath() ) );
    connect( ui->lineEditPicturePath,   SIGNAL( textChanged( QString ) ), this, SLOT( slot_pictureFileSystemWatcherSetButtons() ) );
    connect( pictureFileSystemWatcher,  SIGNAL( directoryChanged( const QString& ) ), this, SLOT( slot_pictureFileSystemWatcherSetButtons() ) );
    ui->lineEditPicturePath->setText( QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ) );

    connect( ui->radioButtonScreenshotWindow, SIGNAL( toggled( bool ) ), ui->comboBoxScreenshotScreen, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonScreenshotArea,   SIGNAL( toggled( bool ) ), ui->comboBoxScreenshotScreen, SLOT( setDisabled( bool ) ) );

    connect( ui->radioButtonScreenshotArea, SIGNAL( toggled( bool ) ), regionChoise, SLOT( setVisible( bool ) ) );

    connect( ui->pushButtonScreenshotShot, SIGNAL( clicked( bool ) ), this, SLOT( slot_preshot_Screenshot() ) );
    connect( ui->pushButtonScreenshotShow, SIGNAL( clicked( bool ) ), this, SLOT( slot_show_Screenshoot() ) );
    connect( ui->pushButtonScreenShotOpenFolder, SIGNAL( clicked( bool ) ), this, SLOT( slot_Folder() ) );

    connect( parent, SIGNAL( signal_resizeEvent( QResizeEvent*) ), this, SLOT( slot_resizeEvent( QResizeEvent* ) ) );

    connect( parent, SIGNAL( signal_close() ), regionChoise, SLOT( close() ) );

    slot_formats_Screenshot();

    // Checkable Widget sind in vokoscreen standardmäßig nicht gesetzt.
    // Diese werden hier beziehungsweise wenn die Settings vorhanden sind dort gesetzt.
    ui->radioButtonScreenshotFullscreen->click();
}

QvkScreenshot::~QvkScreenshot()
{
}


void QvkScreenshot::slot_resizeEvent( QResizeEvent *event )
{
    Q_UNUSED(event);
    if ( !pixmap.isNull() )
    {
        ui->labelScreenShotPicture->setPixmap( pixmap.scaled( ui->labelScreenShotPicture->width()-20,
                                                              ui->labelScreenShotPicture->height()-20,
                                                              Qt::KeepAspectRatio,
                                                              Qt::SmoothTransformation ) );
    }
}


void QvkScreenshot::slot_Folder()
{
    QString path = ui->lineEditPicturePath->text();
    bool b = QDesktopServices::openUrl( QUrl( path, QUrl::TolerantMode ) );
    if ( b == false )
    {
        QDialog *newDialog = new QDialog;
        Ui_NoPlayerDialog myUiDialog;
        myUiDialog.setupUi( newDialog );
        myUiDialog.labelPleaseInstall->setText( tr( "Please install an filemanager" ) );
        newDialog->setModal( true );
        newDialog->setWindowTitle( "vokoscreen" );
        newDialog->show();
    }
}


void QvkScreenshot::slot_formats_Screenshot()
{
    ui->comboBoxScreenShotFormat->clear();
    QImageWriter imageWriter;
    QList<QByteArray> list = imageWriter.supportedImageFormats();

    if ( !list.empty() )
    {
        for ( int i = 0; i < list.count(); i++ )
        {
            ui->comboBoxScreenShotFormat->addItem( list.at( i ) );
        }
    }

    if ( ui->comboBoxScreenShotFormat->findText( "png" ) > -1 )
    {
       ui->comboBoxScreenShotFormat->setCurrentIndex( ui->comboBoxScreenShotFormat->findText( "png" ) );
    }

}


void QvkScreenshot::slot_preshot_Screenshot()
{
    if ( ( ui->radioButtonScreenshotFullscreen->isChecked() == true ) and ( ui->spinBoxScreenshotCountDown->value() > 0 ) )
    {
        parent->hide();
        disconnect( vkCountdown, 0, 0, 0 );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), this, SLOT( slot_singleShot_Screenshot() ) );
        vkCountdown->startCountdown( ui->spinBoxScreenshotCountDown->value() );
        return;
    }

    if ( ui->radioButtonScreenshotFullscreen->isChecked() == true )
    {
        parent->hide();
        slot_singleShot_Screenshot();
        return;
    }

    if( ( ui->radioButtonScreenshotWindow->isChecked() == true )  and ( ui->spinBoxScreenshotCountDown->value() > 0 ) )
    {

        parent->hide();
        disconnect( vkCountdown, 0, 0, 0 );
        disconnect( vkWinInfo, 0, 0, 0 );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), vkWinInfo, SLOT( slot_start() ) );
        connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ), this, SLOT( slot_shot_Screenshot() ) );
        vkCountdown->startCountdown( ui->spinBoxScreenshotCountDown->value() );
        return;
    }

    if( ui->radioButtonScreenshotWindow->isChecked() == true )
    {
        parent->hide();
        disconnect( vkWinInfo, 0, 0, 0 );
        connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ), this, SLOT( slot_shot_Screenshot() ) );
        vkWinInfo->slot_start();
        return;
    }


    if ( ( ui->radioButtonScreenshotArea->isChecked() == true ) and ( ui->spinBoxScreenshotCountDown->value() > 0 ) )
    {
        parent->hide();
        regionChoise->recordMode( true );
        disconnect( vkCountdown, 0, 0, 0 );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), this, SLOT( slot_singleShot_Screenshot() ) );
        vkCountdown->startCountdown( ui->spinBoxScreenshotCountDown->value() );
        return;
    }

    if ( ui->radioButtonScreenshotArea->isChecked() == true )
    {
        regionChoise->recordMode( true );
        slot_singleShot_Screenshot();
    }
}


void QvkScreenshot::slot_singleShot_Screenshot()
{
    QTimer::singleShot( 1000, this, SLOT( slot_shot_Screenshot() ) );
}


void QvkScreenshot::slot_shot_Screenshot()
{
    QApplication::beep();

    if ( ui->radioButtonScreenshotFullscreen->isChecked() == true )
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        pixmap = screen->grabWindow( QApplication::desktop()->winId() );
    }

    if( ui->radioButtonScreenshotWindow->isChecked() == true )
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        pixmap = screen->grabWindow( vkWinInfo->getWinID() );
    }

    if ( ui->radioButtonScreenshotArea->isChecked() == true )
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        pixmap = screen->grabWindow( QApplication::desktop()->winId(),
                                     regionChoise->getXRecordArea(),
                                     regionChoise->getYRecordArea(),
                                     regionChoise->getWidth(),
                                     regionChoise->getHeight() );
        regionChoise->recordMode( false );
    }

    bool ok = pixmap.save( ui->lineEditPicturePath->text() + \
                           "/" + \
                           "vokoscreen-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + "." + ui->comboBoxScreenShotFormat->currentText() );
    Q_UNUSED(ok);

    ui->labelScreenShotPicture->setAlignment( Qt::AlignCenter );
    ui->labelScreenShotPicture->setPixmap( pixmap.scaled( ui->labelScreenShotPicture->width()-6,
                                                          ui->labelScreenShotPicture->height()-6,
                                                          Qt::KeepAspectRatio,
                                                          Qt::SmoothTransformation ) );

    parent->show();
    emit signal_finish_screenshot( true );
}


void QvkScreenshot::slot_show_Screenshoot()
{
    qDebug() << "[vokoscreen] show picture with standard system displayer";
    QDir dir( ui->lineEditPicturePath->text() );
    QStringList filters;
    filters << "vokoscreen*";
    QStringList pictureFileList = dir.entryList( filters, QDir::Files, QDir::Time );

    QString string;
    string.append( "file:///" );
    string.append( ui->lineEditPicturePath->text() );
    string.append( QDir::separator() );
    string.append( pictureFileList.at( 0 ) );
    bool b = QDesktopServices::openUrl( QUrl( string, QUrl::TolerantMode ) );
    if ( b == false )
    {
        QDialog *newDialog = new QDialog;
        Ui_NoPlayerDialog myUiDialog;
        myUiDialog.setupUi( newDialog );
        myUiDialog.labelPleaseInstall->setText( tr( "Please install an image viewer" ) );
        newDialog->setModal( true );
        newDialog->setWindowTitle( "vokoscreen" );
        newDialog->show();
    }
}


void QvkScreenshot::slot_Screenshot_count_changed( int newCount )
{
    Q_UNUSED(newCount);
    ui->comboBoxScreenshotScreen->clear();
    QDesktopWidget *desk = QApplication::desktop();

    QScreen *screen = QGuiApplication::primaryScreen();

    for ( int i = 1; i < desk->screenCount()+1; i++ )
    {
        QString ScreenGeometryX = QString::number( desk->screenGeometry( i-1 ).left() * screen->devicePixelRatio() );
        QString ScreenGeometryY = QString::number( desk->screenGeometry( i-1 ).top() * screen->devicePixelRatio() );
        QString ScreenGeometryWidth = QString::number( desk->screenGeometry( i-1 ).width() * screen->devicePixelRatio() );
        QString ScreenGeometryHeight = QString::number( desk->screenGeometry( i-1 ).height() * screen->devicePixelRatio() );
        QString stringText = tr( "Display" ) + " " + QString::number( i ) + ":  " + ScreenGeometryWidth + " x " + ScreenGeometryHeight;
        QString stringData = "x=" + ScreenGeometryX + " " +
                             "y=" + ScreenGeometryY + " " +
                             "with=" + ScreenGeometryWidth + " " +
                             "height=" + ScreenGeometryHeight;
        ui->comboBoxScreenshotScreen->addItem( stringText, stringData );
    }

    ui->comboBoxScreenshotScreen->addItem( tr( "All Displays" ), -1 );
}


void QvkScreenshot::slot_newPicturePath()
{
    QString dir = QFileDialog::getExistingDirectory( parent,
                                                     "",
                                                     QStandardPaths::writableLocation( QStandardPaths::HomeLocation ),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

    if ( dir > "" )
    {
        ui->lineEditPicturePath->setText( dir );
    }
}


void QvkScreenshot::slot_pictureFileSystemWatcherSetNewPath()
{
    if ( !pictureFileSystemWatcher->directories().isEmpty() )
    {
       pictureFileSystemWatcher->removePaths( pictureFileSystemWatcher->directories() );
    }
    pictureFileSystemWatcher->addPath( ui->lineEditPicturePath->text() );
}


void QvkScreenshot::slot_pictureFileSystemWatcherSetButtons()
{
  QDir dir( ui->lineEditPicturePath->text() );
  QStringList filters;
  filters << "vokoscreen*";
  QStringList List = dir.entryList( filters, QDir::Files, QDir::Time );

  if ( List.isEmpty() || ( ui->pushButtonScreenshotShot->isEnabled() == false ) )
  {
    ui->pushButtonScreenshotShow->setEnabled( false );
  }
  else
  {
    ui->pushButtonScreenshotShow->setEnabled( true );
  }
}
