/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2022 Volker Kohaupt
 * 
 * Author:
 *      Volker Kohaupt <vkohaupt@volkoh.de>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * --End_License--
 */

#include "QvkPlayer.h"
#include "QvkPlayerVideoSurface.h"
#include "QvkSpezialSlider.h"
#include "global.h"
#include "QvkFileDialog.h"

#include <QTime>
#include <QStandardPaths>
#include <QFileDialog>
#include <QTimer>
#include <QMediaPlayer>
#include <QScreen>
#include <QMessageBox>

QvkPlayer::QvkPlayer( QMainWindow *parent, Ui_formMainWindow *ui_mainwindow ) : ui(new Ui::player)
{
    ui->setupUi(this);

    sliderVideo = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_2->insertWidget( 0, sliderVideo );
    ui->horizontalLayout_2->setStretch( 0, 1 );
    sliderVideo->setObjectName( "sliderVideo" );
    sliderVideo->setTracking( true );
    sliderVideo->setMaximum(1);
    sliderVideo->show();
    sliderVideo->setEnabled( false );
    sliderVideo->setShowValue( false );
    sliderVideo->setDecimalPoint( false );
    sliderVideo->setBigHandel( false );

    sliderVolume = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_3->insertWidget( 12, sliderVolume );
    ui->horizontalLayout_3->setStretch( 12, 1 );
    sliderVolume->setObjectName( "sliderVolume" );
    sliderVolume->setTracking( true );
    sliderVolume->setMinimum( 0 );
    sliderVolume->setMaximum( 100 );
    sliderVolume->show();
    sliderVolume->setEnabled( true );
    sliderVolume->setShowValue( true );
    sliderVolume->setDecimalPoint( false );
    sliderVolume->setBigHandel( true );

    parentMainWindow = parent;
    oldWindowTitel = parentMainWindow->windowTitle();
    uiMainWindow = ui_mainwindow;
    uiMainWindow->verticalLayoutTabSidebarPlayer->addWidget( this );

    QList<QWidget *> listToolButton = ui->frameMenueBar->findChildren<QWidget *>();
    for ( int i = 0; i < listToolButton.count(); i++ )
    {
        listToolButton.at(i)->setMouseTracking( true );
    }

    this->setMouseTracking( true );
    ui->framePlayer->setMouseTracking( true );
    ui->labelPlayer->setMouseTracking( true );
    ui->labelPlayer->setFocus(); // Need Focus for F11 in windowmode(GUI)

    QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
    ui->labelPlayer->setPixmap( icon.pixmap( 128, 128 ) );

    ui->framePlayer->setStyleSheet( "QFrame { background-color: black; }" );

    ui->pushButtonPlay->setIcon( QIcon (":/pictures/player/start.png" ) );
    ui->pushButtonPause->setIcon(QIcon( ":/pictures/player/pause.png" ) );
    ui->pushButtonStop->setIcon( QIcon( ":/pictures/player/stop.png" ) );
    ui->toolButtonFrameBackward->setIcon( QIcon( ":/pictures/player/go-previous.png" ) );
    ui->toolButtonFrameForward->setIcon( QIcon( ":/pictures/player/go-next.png" ) );
    ui->toolButtonOpenFile->setIcon( QIcon( ":/pictures/player/document-open.png" ) );
    ui->toolButtonMute->setIcon( QIcon( ":/pictures/player/audio-volume-high.png" ) );
    ui->toolButtonFullscreen->setIcon( QIcon( ":/pictures/player/fullscreen.png" ) );

    mediaPlayer = new QMediaPlayer;
    sliderVolume->setValue( 70 );
    mediaPlayer->setVolume( 70 );
    mediaPlayer->setNotifyInterval( 40 );

    QvkPlayerVideoSurface *videoSurface = new QvkPlayerVideoSurface( this );
    connect( videoSurface, SIGNAL( signal_newPicture( QImage ) ), this, SLOT( slot_setNewImage( QImage ) ) );
    mediaPlayer->setVideoOutput( videoSurface );

    connect( mediaPlayer, SIGNAL( mutedChanged( bool ) ),                this, SLOT( slot_mutedChanged( bool ) ) );
    connect( mediaPlayer, SIGNAL( durationChanged( qint64 ) ),           this, SLOT( slot_durationChanged( qint64 ) ) );
    connect( mediaPlayer, SIGNAL( positionChanged( qint64 ) ),           this, SLOT( slot_positionChanged( qint64 ) ) );
    connect( mediaPlayer, SIGNAL( stateChanged( QMediaPlayer::State ) ), this, SLOT( slot_stateChanged( QMediaPlayer::State ) ) );
    connect( mediaPlayer, SIGNAL( volumeChanged( int ) ),                this, SLOT( slot_volumeChanged( int ) ) ); // Funktioniert nicht mit Pulse
    connect( mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, [=](QMediaPlayer::Error error)
    {
        switch ( error )
        {
            case QMediaPlayer::NoError:
            {
               qDebug().noquote() << global::nameOutput + " " + "[Player]" + " " + "No error has occurred.";
               break;
            }
            case QMediaPlayer::ResourceError:
            {
               qDebug().noquote() << global::nameOutput + " " + "[Player]" + " " + "To play this video a codec is needed. "
                                                                         "Please install a codec pack. "
                                                                         "Note: For a recording, encoders are required, you can find them in a tab for "
                                                                         "the recording. These cannot be used for the player. The player requires decoders.";

               QMessageBox msgBox( this );
               msgBox.setText( "To play this video a codec is needed. "
                               "Please install a codec pack.\n"
                               "\n"
                               "Note: For a recording, encoders are required, you can find them in a tab for "
                               "the recording. These cannot be used for the player. The player requires decoders.");
               msgBox.setWindowTitle( global::name + " " + global::version );
               msgBox.setIcon( QMessageBox::Information );
               msgBox.exec();

               break;
            }
            case QMediaPlayer::FormatError:
            {
               qDebug().noquote() << global::nameOutput + " " + "[Player]" + " " + "The format of a media resource isn't (fully) supported. Playback may still be possible, but without an audio or video component.";
               QMessageBox msgBox( this );
               msgBox.setText( "The format of a media resource isn't (fully) supported.\n"
                               "Playback may still be possible, but without an audio or video component." );
               msgBox.setWindowTitle( global::name + " " + global::version );
               msgBox.setIcon( QMessageBox::Information );
               msgBox.exec();
               break;
            }
            case QMediaPlayer::NetworkError:
            {
               qDebug().noquote() << global::nameOutput + " " + "[Player]" + " " + "A network error occurred.";
               QMessageBox msgBox( this );
               msgBox.setText( "A network error occurred." );
               msgBox.setWindowTitle( global::name + " " + global::version );
               msgBox.setIcon( QMessageBox::Information );
               msgBox.exec();
               break;
            }
            case QMediaPlayer::AccessDeniedError:
            {
               qDebug().noquote() << global::nameOutput + " " + "[Player]" + " " + "There are not the appropriate permissions to play a media resource.";
               QMessageBox msgBox( this );
               msgBox.setText( "There are not the appropriate permissions to play a media resource." );
               msgBox.setWindowTitle( global::name + " " + global::version );
               msgBox.setIcon( QMessageBox::Information );
               msgBox.exec();
               break;
            }
            case QMediaPlayer::ServiceMissingError:
            {
               qDebug().noquote() << global::nameOutput + " " + "[Player]" + " " + "A valid playback service was not found, playback cannot proceed.";
               QMessageBox msgBox( this );
               msgBox.setText( "A valid playback service was not found, playback cannot proceed." );
               msgBox.setWindowTitle( global::name + " " + global::version );
               msgBox.setIcon( QMessageBox::Information );
               msgBox.exec();
               break;
            }
            case QMediaPlayer::MediaIsPlaylist:
            {
               qDebug().noquote() << global::nameOutput + " " + "[Player]" + " " + "QMediaPlayer::MediaIsPlaylist";
               QMessageBox msgBox( this );
               msgBox.setText( "QMediaPlayer::MediaIsPlaylist" );
               msgBox.setWindowTitle( global::name + " " + global::version );
               msgBox.setIcon( QMessageBox::Information );
               msgBox.exec();
               break;
            }
        }
    } );

    connect( sliderVideo, SIGNAL( sliderPressed() ),   this, SLOT( slot_sliderVideoPressed() ) );
    connect( sliderVideo, SIGNAL( sliderReleased() ),  this, SLOT( slot_sliderVideoReleased() ) );
    connect( sliderVideo, SIGNAL( sliderMoved( int )), this, SLOT( slot_sliderVideoMoved( int ) ) );
    connect( sliderVideo, SIGNAL( actionTriggered( int ) ), this, SLOT( slot_sliderVideo_KeyRight_KeyLeft( int ) ) );

    ui->pushButtonPause->hide();
    connect( ui->pushButtonPlay,  SIGNAL( clicked( bool ) ), this,                SLOT( slot_play() ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), mediaPlayer,         SLOT( pause() ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), mediaPlayer,         SLOT( stop() ) );

    connect( ui->toolButtonFrameBackward, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonFrameBackward() ) );
    connect( ui->toolButtonFrameForward, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonFrameForward() ) );

    connect( ui->toolButtonOpenFile, SIGNAL( clicked( bool ) ), this, SLOT( slot_openFile() ) );

    connect( ui->toolButtonFullscreen, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonFullscreen() ) );

    connect( sliderVolume, SIGNAL( sliderMoved( int ) ), mediaPlayer, SLOT( setVolume( int ) ) );

    connect( ui->toolButtonMute,  SIGNAL( clicked( bool ) ), this, SLOT( slot_mute() ) );

    timerHideMouse = new QTimer( this );
    timerHideMouse->setTimerType( Qt::PreciseTimer );
    connect( timerHideMouse, SIGNAL( timeout() ), this, SLOT( slot_hideMouse() ) );
    timerHideMouse->setInterval( 3000 );
    timerHideMouse->start();

    QPixmap pixmap( ":/pictures/player/move.png" );
    QPixmap map = pixmap.scaled( ui->labelMovePicture->size().height()-7, ui->labelMovePicture->size().height()-7 );
    ui->labelMovePicture->setPixmap( map );
    ui->labelMovePicture->hide();

    metaFrame = new QFrame( ui->labelPlayer );
    metaLabel = new QLabel( metaFrame );
    metaLabel->setStyleSheet( "QLabel { background-color : white; color : blue; }" );
    metaFrame->setStyleSheet( "QFrame { background-color : white; color : blue; }" );
    metaFrame->hide();
    metaLabel->hide();
}


QvkPlayer::~QvkPlayer()
{
    delete ui;
}


// Funktioniert nicht mit Pulse
void QvkPlayer::slot_volumeChanged( int )
{
}


void QvkPlayer::slot_hideMouse()
{
   if ( parentMainWindow->isFullScreen() == false )
   {
       ui->labelPlayer->setCursor( Qt::BlankCursor );
       return;
   }

   if ( parentMainWindow->isFullScreen() == true )
   {
       ui->labelPlayer->setCursor( Qt::BlankCursor );
       if ( ui->widgetMenueBar->isHidden() == false )
       {
           ui->widgetMenueBar->hide();
       }
   }
}


void QvkPlayer::mouseMoveEvent( QMouseEvent *event )
{
    Q_UNUSED(event);
    timerHideMouse->stop();
    if ( parentMainWindow->isFullScreen() == false )
    {
        ui->labelPlayer->unsetCursor();
        timerHideMouse->start();
        return;
    }

    if ( ( ui->widgetMenueBar->underMouse() == true ) and ( pressed == true ) )
    {
        ui->widgetMenueBar->move( event->pos().x() - mouseInWidgetX , event->pos().y() - mouseInWidgetY );
        return;
    }

    if ( parentMainWindow->isFullScreen() == true )
    {
        if ( ui->widgetMenueBar->underMouse() == false )
        {
            ui->labelPlayer->unsetCursor();
            ui->widgetMenueBar->show();
            timerHideMouse->start();
        }
    }
}


void QvkPlayer::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    mediaPlayer->stop();
}


void QvkPlayer::slot_toolButtonFrameForward()
{
    if ( mediaPlayer->media().isNull() == false   )
    {
        ui->labelDuration->setText( get_time( ( sliderVideo->value() + 1 ) * mediaPlayer->notifyInterval() ) );
        sliderVideo->setValue( sliderVideo->value() + 1 );
        mediaPlayer->pause();
        mediaPlayer->setPosition( sliderVideo->value() * mediaPlayer->notifyInterval() );
    }
}


void QvkPlayer::slot_toolButtonFrameBackward()
{
    if ( mediaPlayer->media().isNull() == false   )
    {
        ui->labelDuration->setText( get_time( ( sliderVideo->value() - 1 ) * mediaPlayer->notifyInterval() ) );
        sliderVideo->setValue( sliderVideo->value() - 1 );
        mediaPlayer->pause();
        mediaPlayer->setPosition( sliderVideo->value() * mediaPlayer->notifyInterval() );
    }
}


void QvkPlayer::setMediaFile( QString string )
{
    if ( string.contains( "http://" ) or string.contains( "https://" ) )
    {
       QMediaContent mediaContent = QMediaContent( QUrl( string ) );
       mediaPlayer->setMedia( mediaContent );
    }
    else
    {
       mediaPlayer->setMedia( QUrl::fromLocalFile( string ) );
    }

    ui->labelDuration->setEnabled( true );
    ui->labelSeparator->setEnabled( true );
    ui->labelVideoLength->setEnabled( true );
    sliderVideo->setEnabled( true );
}


void QvkPlayer::slot_openFile()
{
    QApplication::setDesktopSettingsAware( false );

    QString file;
    QvkFileDialog vkFileDialog( this );
    if ( !pathOpenFile.isEmpty() ) {
        vkFileDialog.setVideoPath( pathOpenFile );
    } else {
        vkFileDialog.setVideoPath( uiMainWindow->lineEditVideoPath->text() );
    }

    if ( vkFileDialog.exec() == QDialog::Accepted )
    {
        if ( !vkFileDialog.selectedFiles().empty() ) {
            file = vkFileDialog.selectedFiles().at(0);
        }
    }

    if ( !file.isEmpty() )
    {
        setMediaFile( file );
        slot_play();
        pathOpenFile = QDir( file ).absolutePath();
    }

    QApplication::setDesktopSettingsAware( true );
}


void QvkPlayer::slot_sliderVideoPressed()
{
    if ( mediaPlayer->state() == QMediaPlayer::PlayingState )
    {
        playingFlag = true;
    }

    mediaPlayer->pause();
}


void QvkPlayer::slot_sliderVideoReleased()
{
    // if pause und video not playing
    if ( ( mediaPlayer->state() == QMediaPlayer::PausedState ) and ( playingFlag == false ) )
    {
        mediaPlayer->setPosition( sliderVideo->value() * mediaPlayer->notifyInterval() );
        return;
    }

    // If pause and video playing
    if ( ( mediaPlayer->state() == QMediaPlayer::PausedState ) and ( playingFlag == true ) )
    {
        mediaPlayer->setPosition( sliderVideo->value() * mediaPlayer->notifyInterval() );
        playingFlag = false;
    }


    mediaPlayer->setPosition( sliderVideo->value() * mediaPlayer->notifyInterval() );
    mediaPlayer->play();
}


void QvkPlayer::slot_sliderVideoMoved( int value )
{
    mediaPlayer->setPosition( sliderVideo->value() * mediaPlayer->notifyInterval() );
    ui->labelDuration->setText( get_time( value * mediaPlayer->notifyInterval() ) );
}


void QvkPlayer::slot_sliderVideo_KeyRight_KeyLeft( int value )
{
    Q_UNUSED(value);
    mediaPlayer->setPosition( sliderVideo->value() * mediaPlayer->notifyInterval() );
    ui->labelDuration->setText( get_time( sliderVideo->value() * mediaPlayer->notifyInterval() ) );
}


void QvkPlayer::slot_play()
{
    show();
    mediaPlayer->play();
    QUrl url( mediaPlayer->currentMedia().request().url() );
    QString filename = url.fileName();
    parentMainWindow->setWindowTitle( filename + " - " + oldWindowTitel );
}


void QvkPlayer::slot_mute()
{
    ui->toolButtonMute->setEnabled( false );

    if ( mediaPlayer->isMuted()== true )
    {
        mediaPlayer->setMuted( false );
        return;
    }

    if ( mediaPlayer->isMuted()== false )
    {
        mediaPlayer->setMuted( true );
        return;
    }
}


void QvkPlayer::slot_mutedChanged( bool muted )
{
    if ( muted == true )
    {
        ui->toolButtonMute->setIcon( QIcon( ":/pictures/player/audio-volume-muted.png" ) );
        sliderVolume->setEnabled( false );
        ui->toolButtonMute->setEnabled( true );
        return;
    }

    if ( muted == false )
    {
        ui->toolButtonMute->setIcon( QIcon( ":/pictures/player/audio-volume-high.png" ) );
        sliderVolume->setEnabled( true );
        ui->toolButtonMute->setEnabled( true );
        return;
    }
}


QString QvkPlayer::get_time( qint64 value )
{
    int sec = (value/1000) % 60;
    int min = (value/1000/60) % 60;
    int hour= static_cast<int>(value/1000/60/60);

    QTime time( 0, 0, 0 );
    time.setHMS( hour, min, sec );
    return time.toString( "HH:mm:ss" );
}


void QvkPlayer::slot_durationChanged( qint64 value )
{
    // Set length from video on slider
    sliderVideo->setMaximum( static_cast<int>( value / mediaPlayer->notifyInterval() ) );

    // Show length from video in label
    ui->labelVideoLength->setText( get_time( value ) );
}


void QvkPlayer::slot_stateChanged( QMediaPlayer::State state )
{
    qDebug().noquote() << global::nameOutput + " " + "[Player]" << state;

    if ( state == QMediaPlayer::StoppedState )
    {
        ui->pushButtonStop->setEnabled( false );
        ui->pushButtonPause->setVisible( false );
        ui->pushButtonPlay->setVisible( true );
        ui->pushButtonPlay->setEnabled( true );
        sliderVideo->setValue( 0 );
        ui->labelDuration->setText( "00:00:00" );

        QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
        ui->labelPlayer->setPixmap( icon.pixmap( 128, 128 ) );
        parentMainWindow->setWindowTitle( oldWindowTitel );
        vk_showNormal();
        ui->toolButtonFrameBackward->setEnabled( true );
        ui->toolButtonFrameForward->setEnabled( true );
        metaFrame->hide();
        metaLabel->hide();
    }

    if ( state == QMediaPlayer::PlayingState )
    {
        ui->pushButtonPlay->setVisible( false );
        ui->pushButtonPause->setVisible( true );
        ui->pushButtonPause->setEnabled( true );
        ui->pushButtonStop->setEnabled( true );
        ui->toolButtonFrameBackward->setEnabled( true );
        ui->toolButtonFrameForward->setEnabled( true );
    }

    if ( state == QMediaPlayer::PausedState )
    {
        ui->pushButtonPause->setVisible( false );
        ui->pushButtonStop->setEnabled( true );
        ui->pushButtonPlay->setVisible( true );
        ui->pushButtonPlay->setEnabled( true );
        ui->toolButtonFrameBackward->setEnabled( true );
        ui->toolButtonFrameForward->setEnabled( true );
    }
}


/*
 * Wird von mediplayer-notyfier periodisch aufgerufen.
 */
void QvkPlayer::slot_positionChanged( qint64 value )
{
    if ( mediaPlayer->state() == QMediaPlayer::PlayingState )
    {
       sliderVideo->setValue( static_cast<int>( value / mediaPlayer->notifyInterval() ) );

       // Show playing time in label
       ui->labelDuration->setText( get_time( value ) );
    }
}


void QvkPlayer::slot_setNewImage( QImage image )
{
    QSize ratioSize = getPixelaspectRatio();
    qreal ratio = (qreal)ratioSize.width() / (qreal)ratioSize.height();
    QScreen *screen = QGuiApplication::primaryScreen();
    if ( ratio == 1.0 )
    {
        image.setDevicePixelRatio( screen->devicePixelRatio() );
        image = image.scaled( static_cast<int>( ui->framePlayer->width()*screen->devicePixelRatio() ),
                              static_cast<int>( ui->framePlayer->height()*screen->devicePixelRatio() ),
                              Qt::KeepAspectRatio,
                              Qt::SmoothTransformation );
        ui->labelPlayer->setPixmap( QPixmap::fromImage( image ) );
        return;
    }

    // Testvideos find in source code under /vokoscreenNG/Videos-to-test-the-player/
    // Example:
    // Area:450x800 scale:1280*720
    if ( ( ratio != 1.0 ) and ( ratioSize.width() < ratioSize.height() ) )
    {
        // Adapt to the frame
        if ( ui->framePlayer->width() < (int)( (qreal)ui->framePlayer->height() * ( (qreal)image.width() / (qreal)image.height() * ratio ) * screen->devicePixelRatio() ) )
        {
            image = image.scaled( static_cast<int>( ui->framePlayer->width() * screen->devicePixelRatio() ),
                                  (int)( (qreal)ui->framePlayer->width() * ( (qreal)image.height() / (qreal)image.width() / ratio ) * screen->devicePixelRatio() ),
                                  Qt::IgnoreAspectRatio,
                                  Qt::SmoothTransformation );
            ui->labelPlayer->setPixmap( QPixmap::fromImage( image ) );
            return;
        }

        // Adapt to the frame
        if ( ui->framePlayer->height() > (int)( (qreal)ui->framePlayer->height() * ( (qreal)image.width() / (qreal)image.height() * ratio ) * screen->devicePixelRatio() ) )
        {
            image = image.scaled( (int)( (qreal)ui->framePlayer->height() * ( (qreal)image.width() / (qreal)image.height() * ratio ) * screen->devicePixelRatio() ),
                                  static_cast<int>( ui->framePlayer->height() * screen->devicePixelRatio() ),
                                  Qt::IgnoreAspectRatio,
                                  Qt::SmoothTransformation );
            ui->labelPlayer->setPixmap( QPixmap::fromImage( image ) );
            return;
        }
    }

    // Example:
    // Area:800x320 scale:1280*720
    if ( ( ratio != 1.0 ) and ( ratioSize.width() > ratioSize.height() ) )
    {
        // Adapt to the frame
        if ( ui->framePlayer->height() < (int)( (qreal)ui->framePlayer->width() * ( (qreal)image.height() / (qreal)image.width() / ratio ) * screen->devicePixelRatio() ) )
        {
            image = image.scaled( (int)( (qreal)ui->framePlayer->height() * ( (qreal)image.width() / (qreal)image.height() * ratio ) * screen->devicePixelRatio() ),
                                  static_cast<int>( ui->framePlayer->height() * screen->devicePixelRatio() ),
                                  Qt::IgnoreAspectRatio,
                                  Qt::SmoothTransformation );
            ui->labelPlayer->setPixmap( QPixmap::fromImage( image ) );
            return;
        }

        // Adapt to the frame
        if ( ui->framePlayer->height() > (int)( (qreal)ui->framePlayer->width() * ( (qreal)image.height() / (qreal)image.width() / ratio ) * screen->devicePixelRatio() ) )
        {
            image = image.scaled( static_cast<int>( ui->framePlayer->width() * screen->devicePixelRatio() ),
                                  (int)( (qreal)ui->framePlayer->width() * (qreal)image.height() / (qreal)image.width() / ratio * screen->devicePixelRatio() ),
                                  Qt::IgnoreAspectRatio,
                                  Qt::SmoothTransformation );
            ui->labelPlayer->setPixmap( QPixmap::fromImage( image ) );
            return;
        }
    }
}


QSize QvkPlayer::getPixelaspectRatio()
{
    int width = 1;
    int height = 1;
    if ( mediaPlayer->isMetaDataAvailable() == true )
    {
        QStringList stringList = mediaPlayer->availableMetaData();
        for ( int i = 0; i < stringList.count(); i++ )
        {
            if ( stringList.at(i) == "PixelAspectRatio" )
            {
                width = mediaPlayer->metaData( stringList.at(i) ).toSize().width();
                height = mediaPlayer->metaData( stringList.at(i) ).toSize().height();
                break;
           }
        }
    }
    return QSize( width, height );
}


void QvkPlayer::vk_showFullscreen()
{
    uiMainWindow->tabWidgetSideBar->hide();
    uiMainWindow->widgetSidbar->hide();
    uiMainWindow->horizontalLayout_49->addWidget( this );

    parentMainWindow->showFullScreen();

    ui->widgetMenueBar->setParent( ui->labelPlayer);
    ui->framePlayer->setStyleSheet( "QFrame { background-color: black; }"  );
    ui->widgetMenueBar->setStyleSheet( "QWidget { background-color: lightgray; }" );
    ui->toolButtonFullscreen->setIcon( QIcon( ":/pictures/player/restore.png" ) );
    ui->labelMovePicture->show();
    ui->widgetMenueBar->show();
    ui->labelPlayer->setFocus();

    if ( mediaPlayer->state() == QMediaPlayer::PausedState )
    {
        ui->toolButtonFrameBackward->click();
        qApp->processEvents();
        ui->toolButtonFrameForward->click();
        qApp->processEvents();
    }
}


void QvkPlayer::vk_showNormal()
{
    uiMainWindow->tabWidgetSideBar->show();
    uiMainWindow->widgetSidbar->show();
    uiMainWindow->verticalLayoutTabSidebarPlayer->addWidget( this );
    parentMainWindow->showNormal();

    ui->verticalLayout_4->addWidget( ui->widgetMenueBar );
    ui->framePlayer->setStyleSheet( "QFrame { background-color: black; }"  );
    // An empty string resets the color
    ui->widgetMenueBar->setStyleSheet( "" );
    ui->toolButtonFullscreen->setIcon( QIcon( ":/pictures/player/fullscreen.png" ) );
    ui->labelMovePicture->hide();
    ui->widgetMenueBar->show();
    ui->labelPlayer->setFocus();

    if ( mediaPlayer->state() == QMediaPlayer::PausedState )
    {
        ui->toolButtonFrameBackward->click();
        qApp->processEvents();
        ui->toolButtonFrameForward->click();
        qApp->processEvents();
    }
}


void QvkPlayer::slot_toolButtonFullscreen()
{
    if ( parentMainWindow->isFullScreen() == true )
    {
        vk_showNormal();
    }
    else
    {
        vk_showFullscreen();
    }
}


void QvkPlayer::mouseDoubleClickEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton )
    {
        if ( parentMainWindow->isFullScreen() == true )
        {
            vk_showNormal();
        }
        else
        {
            vk_showFullscreen();
        }
    }
}


void QvkPlayer::keyPressEvent( QKeyEvent *event )
{
    int stepSize = 25;
    if ( ( mediaPlayer->state() == QMediaPlayer::PlayingState ) or ( mediaPlayer->state() == QMediaPlayer::PausedState ) )
    {
        if ( event->key() == Qt::Key_Right )
        {
            ui->labelDuration->setText( get_time( ( sliderVideo->value() + stepSize ) * mediaPlayer->notifyInterval() ) );
            sliderVideo->setValue( sliderVideo->value() + stepSize );
            mediaPlayer->setPosition( sliderVideo->value() * mediaPlayer->notifyInterval() );
            return;
        }

        if ( event->key() == Qt::Key_Left )
        {
            ui->labelDuration->setText( get_time( ( sliderVideo->value() - stepSize ) * mediaPlayer->notifyInterval() ) );
            sliderVideo->setValue( sliderVideo->value() - stepSize );
            mediaPlayer->setPosition( sliderVideo->value() * mediaPlayer->notifyInterval() );
        }
    }

    if ( event->key() == Qt::Key_Space )
    {
        switch ( mediaPlayer->state() )
        {
            case QMediaPlayer::PlayingState:
            {
                ui->pushButtonPause->click();
                break;
            }
            case QMediaPlayer::PausedState:
            {
                ui->pushButtonPlay->click();
                break;
            }
            case QMediaPlayer::StoppedState:
            {
                ui->pushButtonPlay->click();
                break;
            }
        }
    }

    if ( event->key() == Qt::Key_Escape )
    {
        vk_showNormal();
    }

    if ( ( event->key() == Qt::Key_F11 ) or ( event->key() == Qt::Key_F ) )
    {
        if ( parentMainWindow->isFullScreen() == true )
        {
            vk_showNormal();
        }
        else
        {
            vk_showFullscreen();
        }
    }
}


void QvkPlayer::mousePressEvent( QMouseEvent *event )
{
    if ( parentMainWindow->isFullScreen() == true )
    {
        if ( ui->widgetMenueBar->underMouse() and ( pressed == false  ) )
        {
            mouseInWidgetX = event->pos().x() - ui->widgetMenueBar->pos().x();
            mouseInWidgetY = event->pos().y() - ui->widgetMenueBar->pos().y();
            ui->labelMovePicture->setCursor( Qt::SizeAllCursor );
            pressed = true;
        }
    }

    if ( ( event->button() == Qt::RightButton ) and ( metaFrame->isVisible() == true ) )
    {
        metaFrame->hide();
        metaLabel->hide();
        return;
    }

    if ( event->button() == Qt::RightButton )
    {
        if ( mediaPlayer->isMetaDataAvailable() == true )
        {
            QString metaString;
            QStringList stringList = mediaPlayer->availableMetaData();
            stringList.removeAt( stringList.indexOf( "AudioBitRate" ) );
            stringList.removeAt( stringList.indexOf( "maximum-bitrate" ) );
            stringList.removeAt( stringList.indexOf( "minimum-bitrate" ) );
            stringList.sort();
            for ( int i = 0; i < stringList.count(); i++ )
            {
                if ( stringList.at(i) == "Resolution" )
                {
                    QString width = QString::number( mediaPlayer->metaData( stringList.at(i) ).toSize().width() );
                    QString height = QString::number( mediaPlayer->metaData( stringList.at(i) ).toSize().height() );
                    metaString += stringList.at(i) + " :   " + width + "x" + height + "\n";
                    continue;
                }

                if ( stringList.at(i) == "PixelAspectRatio" )
                {
                    QString width = QString::number( mediaPlayer->metaData( stringList.at(i) ).toSize().width() );
                    QString height = QString::number( mediaPlayer->metaData( stringList.at(i) ).toSize().height() );
                    metaString += stringList.at(i) + " :   " + width + "x" + height + "\n";
                    continue;
                }

                metaString += stringList.at(i) + " :   "
                           + mediaPlayer->metaData( stringList.at(i) ).toString()
                           + "\n";
            }
            metaLabel->setText( metaString );
            metaLabel->move( 10, 10 );
            metaFrame->hide();
            metaLabel->hide();
            metaLabel->show();
            metaFrame->show();
            metaFrame->resize( QSize( metaLabel->size().width() + 20, metaLabel->size().height() ) );
        }
    }
}


void QvkPlayer::mouseReleaseEvent( QMouseEvent *event )
{
    Q_UNUSED(event);
    ui->labelMovePicture->setCursor( Qt::ArrowCursor );
    pressed = false;
}


void QvkPlayer::leaveEvent( QEvent *event )
{
    Q_UNUSED(event);
    ui->labelPlayer->setCursor( Qt::ArrowCursor );
}


void QvkPlayer::resizeEvent( QResizeEvent *event )
{
    Q_UNUSED(event);
    if ( parentMainWindow->isFullScreen() == true )
    {
        ui->widgetMenueBar->move( ui->framePlayer->width()/2 - ui->widgetMenueBar->width()/2,
                                  ui->framePlayer->height() - ui->widgetMenueBar->height() );
        return;
    }


    if ( mediaPlayer->state() == QMediaPlayer::PausedState )
    {
        ui->toolButtonFrameForward->click();
        ui->toolButtonFrameBackward->click();
    }
}

