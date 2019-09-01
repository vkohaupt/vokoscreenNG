/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
 * 
 * Author:
 *      Volker Kohaupt <vkohaupt@freenet.de>
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

    // Augenscheinlich hat die Angabe des Mimetypes keinen Einfluß.
    qDebug() << "h264" << QMediaPlayer::hasSupport( "video/h264", (QStringList)"h264" );
    qDebug() << "vp8"  << QMediaPlayer::hasSupport( "video/webm", (QStringList)"vp8" );

    sliderVideo = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_2->insertWidget( 0, sliderVideo );
    ui->horizontalLayout_2->setStretch( 0, 1 );
    sliderVideo->setObjectName( "sliderVideo" );
    sliderVideo->setTracking( true );
    sliderVideo->setShowValue( false );
    sliderVideo->setEnabled( false );
    sliderVideo->show();

    sliderVolume = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_3->insertWidget( 12, sliderVolume );
    ui->horizontalLayout_3->setStretch( 12, 1 );
    sliderVolume->setObjectName( "sliderVolume" );
    sliderVolume->setTracking( true );
    sliderVolume->setMinimum( 0 );
    sliderVolume->setMaximum( 100 );
    sliderVolume->show();

    parentMainWindow = parent;
    oldWindowTitel = parentMainWindow->windowTitle();
    uiMainWindow = ui_mainwindow;
    uiMainWindow->verticalLayoutTabSidebarPlayer->addWidget( this );

    this->setMouseTracking( true );
    ui->framePlayer->setMouseTracking( true );
    ui->labelPlayer->setMouseTracking( true );
    ui->labelPlayer->setFocus(); // Need Focus for F11 in windowmode(GUI)

    QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
    ui->labelPlayer->setPixmap( icon.pixmap( 128, 128 ) );

    ui->framePlayer->setStyleSheet( "background-color: black;" );

    ui->pushButtonPlay->setIcon( QIcon::fromTheme( "media-playback-start" , style()->standardIcon( QStyle::SP_MediaPlay ) ) );
    ui->pushButtonPause->setIcon( QIcon::fromTheme( "media-playback-pause", style()->standardIcon( QStyle::SP_MediaPause ) ) );
    ui->pushButtonStop->setIcon( QIcon::fromTheme( "media-playback-stop"  , style()->standardIcon( QStyle::SP_MediaStop ) ) );
    ui->toolButtonFrameBackward->setIcon( QIcon::fromTheme( "go-previous",  style()->standardIcon( QStyle::SP_MediaSkipBackward ) ) );
    ui->toolButtonFrameForward->setIcon( QIcon::fromTheme( "go-next",       style()->standardIcon( QStyle::SP_MediaSkipForward ) ) );
    ui->toolButtonOpenFile->setIcon( QIcon::fromTheme( "document-open",     style()->standardIcon( QStyle::SP_FileIcon ) ) );
    ui->toolButtonMute->setIcon( QIcon::fromTheme( "audio-volume-high"    , style()->standardIcon( QStyle::SP_MediaVolume ) ) );
    ui->toolButtonFullscreen->setIcon( QIcon::fromTheme( "view-fullscreen", QIcon( ":/pictures/player/fullscreen.png" ) ) );

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
    connect( mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
        [=](QMediaPlayer::Error error){
                                        Q_UNUSED(error);
                                        QMessageBox msgBox( this );
                                        msgBox.setText( "To play this video a codec is needed. \n Please install a codec pack." );
                                        msgBox.setWindowTitle( global::name + " " + global::version );
                                        msgBox.setIcon( QMessageBox::Information );
                                        msgBox.exec();
                                      });

    connect( sliderVideo, SIGNAL( sliderPressed() ),   this, SLOT( slot_sliderVideoPressed() ) );
    connect( sliderVideo, SIGNAL( sliderReleased() ),  this, SLOT( slot_sliderVideoReleased() ) );
    connect( sliderVideo, SIGNAL( sliderMoved( int )), this, SLOT( slot_sliderVideoMoved( int ) ) );

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

    QList<QWidget *> listWidget = ui->frameMenueBar->findChildren<QWidget *>();
    for ( int i = 0; i < listWidget.count(); i++ )
    {
        listWidget.at(i)->setStyleSheet( "QToolTip { background-color: black; color: white }" );
    }
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
        ui->labelPlayer->unsetCursor();
        ui->widgetMenueBar->show();
        timerHideMouse->start();
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
    ui->labelVideoLenght->setEnabled( true );
    sliderVideo->setEnabled( true );
}


void QvkPlayer::slot_openFile()
{
    if ( pathOpenFile == "" )
    {
        pathOpenFile = QStandardPaths::writableLocation( QStandardPaths::MoviesLocation );
    }


    QString file = QFileDialog::getOpenFileName( this,
                                                 tr( "Open File" ),
                                                 pathOpenFile,
                                                 "" );

    if ( !file.isEmpty() )
    {
        setMediaFile( file );
        slot_play();
        pathOpenFile = QDir( file ).absolutePath();
    }
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
}


void QvkPlayer::slot_play()
{
    show();
    mediaPlayer->play();
    parentMainWindow->setWindowTitle( mediaPlayer->currentMedia().canonicalUrl().fileName() + " - " + oldWindowTitel );
}


void QvkPlayer::slot_mute()
{
    ui->toolButtonMute->setEnabled( false );

    if ( mediaPlayer->isMuted()== true )
    {
        mediaPlayer->setMuted( false );
        ui->toolButtonMute->setFocus();
        return;
    }

    if ( mediaPlayer->isMuted()== false )
    {
        mediaPlayer->setMuted( true );
        ui->toolButtonMute->setFocus();
        return;
    }
}


void QvkPlayer::slot_mutedChanged( bool muted )
{
    if ( muted == true )
    {
        ui->toolButtonMute->setIcon( QIcon::fromTheme( "audio-volume-muted", style()->standardIcon( QStyle::SP_MediaVolumeMuted ) ) );
        sliderVolume->setEnabled( false );
        ui->toolButtonMute->setEnabled( true );
        return;
    }

    if ( muted == false )
    {
        ui->toolButtonMute->setIcon( QIcon::fromTheme( "audio-volume-high", style()->standardIcon( QStyle::SP_MediaVolume ) ) );
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
    // Set lenght from video on slider
    sliderVideo->setMaximum( static_cast<int>( value / mediaPlayer->notifyInterval() ) );

    // Show lenght from video in label
    ui->labelVideoLenght->setText( get_time( value ) );
}


void QvkPlayer::slot_stateChanged( QMediaPlayer::State state )
{
    qDebug() << "[vokoplayer]" << state;

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
        ui->pushButtonPlay->setFocus();
        metaFrame->hide();
        metaLabel->hide();
    }

    if ( state == QMediaPlayer::PlayingState )
    {
        ui->pushButtonPlay->setVisible( false );
        ui->pushButtonPause->setVisible( true );
        ui->pushButtonPause->setEnabled( true );
        ui->pushButtonPause->setFocus();
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
        ui->pushButtonPlay->setFocus();
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
    QScreen *screen = QGuiApplication::primaryScreen();
    image.setDevicePixelRatio( screen->devicePixelRatio() );
    image = image.scaled( static_cast<int>( ui->framePlayer->width()*screen->devicePixelRatio() ),
                          static_cast<int>( ui->framePlayer->height()*screen->devicePixelRatio() ),
                          Qt::KeepAspectRatio,
                          Qt::SmoothTransformation );
    ui->labelPlayer->setPixmap( QPixmap::fromImage( image ) );
}


void QvkPlayer::vk_showFullscreen()
{
    uiMainWindow->tabWidgetSideBar->hide();
    uiMainWindow->verticalLayoutCentralWidget->addWidget( this );
    parentMainWindow->showFullScreen();

    ui->widgetMenueBar->setParent( ui->labelPlayer);
    ui->framePlayer->setStyleSheet( "background-color: black;"  );
    ui->widgetMenueBar->setStyleSheet( "background-color: lightgray;"  );
    ui->toolButtonFullscreen->setIcon( QIcon::fromTheme( "view-restore", QIcon( ":/pictures/player/restore.png" ) ) );
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
    uiMainWindow->verticalLayoutTabSidebarPlayer->addWidget( this );
    parentMainWindow->showNormal();

    ui->verticalLayout_4->addWidget( ui->widgetMenueBar );
    ui->framePlayer->setStyleSheet( "background-color: black;"  );
    // An empty string resets the color
    ui->widgetMenueBar->setStyleSheet( "" );
    ui->toolButtonFullscreen->setIcon( QIcon::fromTheme( "view-fullscreen", QIcon( ":/pictures/player/fullscreen.png" ) ) );
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

    if ( event->key() == Qt::Key_F11 )
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
                    QString heigth = QString::number( mediaPlayer->metaData( stringList.at(i) ).toSize().height() );
                    metaString += stringList.at(i) + " :   " + width + "x" + heigth + "\n";
                    continue;
                }

                if ( stringList.at(i) == "PixelAspectRatio" )
                {
                    QString width = QString::number( mediaPlayer->metaData( stringList.at(i) ).toSize().width() );
                    QString heigth = QString::number( mediaPlayer->metaData( stringList.at(i) ).toSize().height() );
                    metaString += stringList.at(i) + " :   " + width + "x" + heigth + "\n";
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

