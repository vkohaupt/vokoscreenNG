#include "QvkScreenshot.h"
#include "QvkWinInfo.h"
#include "QvkCountdown.h"
#include "QvkRegionChoise.h"
#include "global.h"

#include <QImageWriter>
#include <QGuiApplication>
#include <QScreen>
#include <QGuiApplication>
#include <QDesktopServices>
#include <QDialog>
#include <QFileDialog>
#include <QStandardPaths>
#include <QIcon>
#include <QMessageBox>

QvkScreenshot::QvkScreenshot( QvkMainWindow *value, Ui_formMainWindow *ui_mainwindow ):vkWinInfo(new QvkWinInfo),
                                                                                       vkCountdown(new QvkCountdown),
                                                                                       regionChoise(new QvkRegionChoise)
{
    parent = value;
    ui = ui_mainwindow;

    regionChoise->setFrameColor( Qt::black );

    connect( qApp, SIGNAL( screenAdded( QScreen* ) ),   this, SLOT( slot_screen_count_changed() ) );
    connect( qApp, SIGNAL( screenRemoved( QScreen* ) ), this, SLOT( slot_screen_count_changed() ) );
    emit qApp->screenAdded(0);

    ui->radioButtonScreenshotFullscreen->setText( tr("Fullscreen") ); // QT Creator sets an ampersand, translation now here
    ui->radioButtonScreenshotWindow->setText( tr( "Window") ); // QT Creator sets an ampersand, translation now here

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

    makeScreenshotByStart();

}

QvkScreenshot::~QvkScreenshot()
{
}

void QvkScreenshot::makeScreenshotByStart()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    pixmap = screen->grabWindow( 0 );
    ui->labelScreenShotPicture->setAlignment( Qt::AlignCenter );
    ui->labelScreenShotPicture->setPixmap( pixmap.scaled( ui->labelScreenShotPicture->width(),
                                                          ui->labelScreenShotPicture->height(),
                                                          Qt::KeepAspectRatio,
                                                          Qt::SmoothTransformation ) );

}


/*
 * Hint:QLabel sizePolicy in GUI must be set to ignored
 */
void QvkScreenshot::slot_resizeEvent( QResizeEvent *event )
{
    Q_UNUSED(event);
    if ( !pixmap.isNull() )
    {
        ui->labelScreenShotPicture->setPixmap( pixmap.scaled( ui->labelScreenShotPicture->width(),
                                                              ui->labelScreenShotPicture->height(),
                                                              Qt::KeepAspectRatio,
                                                              Qt::SmoothTransformation ) );
    }
}


void QvkScreenshot::slot_Folder()
{
    QString path = ui->lineEditPicturePath->text();
    if ( QDesktopServices::openUrl( QUrl( path, QUrl::TolerantMode ) ) == false )
    {
        QMessageBox msgBox( parent );
        msgBox.setText( tr( "No filemanager found." ) + "\n" + tr( "Please install a filemanager." ) );
        msgBox.setWindowTitle( global::name);
        msgBox.setIcon( QMessageBox::Information );
        msgBox.exec();
    }


}


void QvkScreenshot::slot_formats_Screenshot()
{
    ui->comboBoxScreenshotFormat->clear();
    QImageWriter imageWriter;
    QList<QByteArray> list = imageWriter.supportedImageFormats();

    if ( !list.empty() )
    {
        for ( int i = 0; i < list.count(); i++ )
        {
            ui->comboBoxScreenshotFormat->addItem( list.at( i ) );
        }
    }

    if ( ui->comboBoxScreenshotFormat->findText( "png" ) > -1 )
    {
       ui->comboBoxScreenshotFormat->setCurrentIndex( ui->comboBoxScreenshotFormat->findText( "png" ) );
    }

}


void QvkScreenshot::slot_preshot_Screenshot()
{
    if ( ( ui->radioButtonScreenshotFullscreen->isChecked() == true ) and ( ui->spinBoxScreenshotCountDown->value() > 0 ) )
    {
        if ( ui->checkBoxScreenshotHideThisWindow->checkState() == Qt::Checked )
        {
           parent->hide();
        }
        disconnect( vkCountdown, nullptr, nullptr, nullptr );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), this, SLOT( slot_singleShot_Screenshot() ) );
        vkCountdown->startCountdown( ui->spinBoxScreenshotCountDown->value() );
        return;
    }

    if ( ui->radioButtonScreenshotFullscreen->isChecked() == true )
    {
        if ( ui->checkBoxScreenshotHideThisWindow->checkState() == Qt::Checked )
        {
           parent->hide();
        }
        slot_singleShot_Screenshot();
        return;
    }

    if( ( ui->radioButtonScreenshotWindow->isChecked() == true )  and ( ui->spinBoxScreenshotCountDown->value() > 0 ) )
    {
        if ( ui->checkBoxScreenshotHideThisWindow->checkState() == Qt::Checked )
        {
           parent->hide();
        }
        disconnect( vkCountdown, nullptr, nullptr, nullptr );
        disconnect( vkWinInfo, nullptr, nullptr, nullptr );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), vkWinInfo, SLOT( slot_start() ) );
        connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ), this, SLOT( slot_shot_Screenshot() ) );
        vkCountdown->startCountdown( ui->spinBoxScreenshotCountDown->value() );
        return;
    }

    if( ui->radioButtonScreenshotWindow->isChecked() == true )
    {
        if ( ui->checkBoxScreenshotHideThisWindow->checkState() == Qt::Checked )
        {
           parent->hide();
        }
        disconnect( vkWinInfo, nullptr, nullptr, nullptr );
        connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ), this, SLOT( slot_shot_Screenshot() ) );
        vkWinInfo->slot_start();
        return;
    }


    if ( ( ui->radioButtonScreenshotArea->isChecked() == true ) and ( ui->spinBoxScreenshotCountDown->value() > 0 ) )
    {
        if ( ui->checkBoxScreenshotHideThisWindow->checkState() == Qt::Checked )
        {
           parent->hide();
        }
        regionChoise->recordMode( true );
        disconnect( vkCountdown, nullptr, nullptr, nullptr );
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
        pixmap = screen->grabWindow( 0 );
    }

    if( ui->radioButtonScreenshotWindow->isChecked() == true )
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        pixmap = screen->grabWindow( vkWinInfo->getWinID() );
    }

    if ( ui->radioButtonScreenshotArea->isChecked() == true )
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        pixmap = screen->grabWindow( 0,
                                     regionChoise->getXRecordArea(),
                                     regionChoise->getYRecordArea(),
                                     regionChoise->getWidth(),
                                     regionChoise->getHeight() );
        regionChoise->recordMode( false );
    }

    bool ok = pixmap.save( ui->lineEditPicturePath->text() + \
                           "/" + \
                           global::name + "-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + "." + ui->comboBoxScreenshotFormat->currentText() );
    Q_UNUSED(ok);

    ui->labelScreenShotPicture->setAlignment( Qt::AlignCenter );
    ui->labelScreenShotPicture->setPixmap( pixmap.scaled( ui->labelScreenShotPicture->width(),
                                                          ui->labelScreenShotPicture->height(),
                                                          Qt::KeepAspectRatio,
                                                          Qt::SmoothTransformation ) );

    parent->show();
    emit signal_finish_screenshot( true );
}


void QvkScreenshot::slot_show_Screenshoot()
{
    qDebug().noquote() << global::nameOutput << "show picture with standard system displayer";
    QDir dir( ui->lineEditPicturePath->text() );
    QStringList filters;
    filters << global::name + "*";
    QStringList pictureFileList = dir.entryList( filters, QDir::Files, QDir::Time );

    QString string;
    string.append( "file:///" );
    string.append( ui->lineEditPicturePath->text() );
    string.append( QDir::separator() );
    string.append( pictureFileList.at( 0 ) );
    if ( QDesktopServices::openUrl( QUrl( string, QUrl::TolerantMode ) ) == false )
    {
        QMessageBox msgBox( parent );
        msgBox.setText( tr( "No imageviewer found." ) + "\n" + tr( "Please install a imageviewer." ) );
        msgBox.setWindowTitle( global::name );
        msgBox.setIcon( QMessageBox::Information );
        msgBox.exec();
    }
}


void QvkScreenshot::slot_screen_count_changed()
{
    ui->comboBoxScreenshotScreen->clear();

    QList<QScreen *> screen = QGuiApplication::screens();
    for ( int i = 0; i < screen.size(); i++ )
    {
        QString X = QString::number( screen.at(i)->geometry().left() * screen.at(i)->devicePixelRatio() );
        QString Y = QString::number( screen.at(i)->geometry().top() * screen.at(i)->devicePixelRatio() );
        QString Width = QString::number( screen.at(i)->geometry().width() * screen.at(i)->devicePixelRatio() );
        QString Height = QString::number( screen.at(i)->geometry().height() * screen.at(i)->devicePixelRatio() );
        QString stringText = screen.at(i)->name() + " " + ":  " + Width + " x " + Height;
        QString stringData = "x=" + X + " " +
                             "y=" + Y + " " +
                             "with=" + Width + " " +
                             "height=" + Height;
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
  filters << global::name + "*";
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
