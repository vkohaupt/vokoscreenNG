#include "QvkPlayer.h"
#include "QvkPlayerVideoSurface.h"

#include <QTime>
#include <QStandardPaths>
#include <QFileDialog>
#include <QTimer>

QvkPlayer::QvkPlayer( QMainWindow *parent, Ui_formMainWindow *ui_mainwindow ) : ui(new Ui::player)
{
    ui->setupUi(this);

    parentMainWindow = parent;
    oldWindowTitel = parentMainWindow->windowTitle();
    uiMainWindow = ui_mainwindow;
    uiMainWindow->verticalLayoutTabSidebarPlayer->addWidget( this );

    this->setMouseTracking( true );
    ui->framePlayer->setMouseTracking( true );
    ui->labelPlayer->setMouseTracking( true );
    ui->labelPlayer->setFocus(); // Need Focus for F11 in windowmode(GUI)

    QIcon icon( QString::fromUtf8( ":/pictures/player/vokoscreen.png" ) );
    ui->labelPlayer->setPixmap( icon.pixmap( 200, 185 ) );

    ui->framePlayer->setStyleSheet( "background-color: black;" );

    ui->toolButtonOpenFile->setIcon( QIcon::fromTheme( "document-open", style()->standardIcon( QStyle::SP_FileIcon ) ) );
    //ui->toolButtonHelpPlayer->setIcon( style()->standardIcon( QStyle::SP_MessageBoxInformation ) );

    ui->pushButtonPlay->setIcon( QIcon::fromTheme( "media-playback-start" , style()->standardIcon( QStyle::SP_MediaPlay ) ) );
    ui->pushButtonPause->setIcon( QIcon::fromTheme( "media-playback-pause", style()->standardIcon( QStyle::SP_MediaPause ) ) );
    ui->pushButtonStop->setIcon( QIcon::fromTheme( "media-playback-stop"  , style()->standardIcon( QStyle::SP_MediaStop ) ) );
    ui->toolButtonMute->setIcon( QIcon::fromTheme( "audio-volume-high"    , style()->standardIcon( QStyle::SP_MediaVolume ) ) );
    ui->toolButtonFullscreen->setIcon( QIcon::fromTheme( "view-fullscreen", QIcon( ":/pictures/player/fullscreen.png" ) ) );

    mediaPlayer = new QMediaPlayer;
    ui->sliderVolume->setValue( 70 );
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

    connect( ui->toolButtonOpenFile, SIGNAL( clicked( bool ) ), this, SLOT( slot_openFile() ) );

    connect( ui->sliderVideo, SIGNAL( sliderPressed() ),   this, SLOT( slot_sliderVideoPressed() ) );
    connect( ui->sliderVideo, SIGNAL( sliderReleased() ),  this, SLOT( slot_sliderVideoReleased() ) );
    connect( ui->sliderVideo, SIGNAL( sliderMoved( int) ), this, SLOT( slot_sliderVideoMoved( int ) ) );

    connect( ui->pushButtonPlay,  SIGNAL( clicked( bool ) ), this,                SLOT( slot_play() ) );

    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), ui->pushButtonPause, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), mediaPlayer,         SLOT( pause() ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), ui->pushButtonStop,  SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), ui->pushButtonPlay,  SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), ui->pushButtonPlay,  SLOT( setFocus() ) );

    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->pushButtonStop,  SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), mediaPlayer,         SLOT( stop() ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->pushButtonPause, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->pushButtonPlay , SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->pushButtonPlay,  SLOT( setFocus() ) );

    connect( ui->toolButtonFullscreen, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonFullscreen() ) );

    connect( ui->sliderVolume,    SIGNAL( sliderMoved( int ) ), mediaPlayer, SLOT( setVolume( int ) ) );

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


void QvkPlayer::setMediaFile( QString string )
{
    mediaPlayer->setMedia( QUrl::fromLocalFile( string ) );
    ui->labelDuration->setEnabled( true );
    ui->labelSeparator->setEnabled( true );
    ui->labelVideoLenght->setEnabled( true );
    ui->sliderVideo->setEnabled( true );
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
    mediaPlayer->pause();
}


void QvkPlayer::slot_sliderVideoReleased()
{
    mediaPlayer->setPosition( ui->sliderVideo->value() * mediaPlayer->notifyInterval() );
    mediaPlayer->play();
}


void QvkPlayer::slot_sliderVideoMoved( int value )
{
    mediaPlayer->setPosition( ui->sliderVideo->value() * mediaPlayer->notifyInterval() );
    ui->labelDuration->setText( get_time( value * mediaPlayer->notifyInterval() ) );
}


void QvkPlayer::slot_play()
{
    show();
    mediaPlayer->play();
    parentMainWindow->setWindowTitle( mediaPlayer->currentMedia().canonicalUrl().fileName() + " - " + oldWindowTitel );
    ui->pushButtonPlay->setEnabled( false );
    ui->pushButtonPause->setEnabled( true);
    ui->pushButtonStop->setEnabled( true );
    ui->pushButtonPause->setFocus();
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
        ui->sliderVolume->setEnabled( false );
        ui->toolButtonMute->setEnabled( true );
        return;
    }

    if ( muted == false )
    {
        ui->toolButtonMute->setIcon( QIcon::fromTheme( "audio-volume-high", style()->standardIcon( QStyle::SP_MediaVolume ) ) );
        ui->sliderVolume->setEnabled( true );
        ui->toolButtonMute->setEnabled( true );
        return;
    }
}


QString QvkPlayer::get_time( qint64 value )
{
    int sec = (value/1000) % 60;
    int min = (value/1000/60) % 60;
    int hour= (value/1000/60/60);

    QTime time( 0, 0, 0 );
    time.setHMS( hour, min, sec );
    return time.toString( "HH:mm:ss" );
}


void QvkPlayer::slot_durationChanged( qint64 value )
{
    // Set lenght from video on slider
    ui->sliderVideo->setMaximum( value / mediaPlayer->notifyInterval() );

    // Show lenght from video in label
    ui->labelVideoLenght->setText( get_time( value ) );
}


void QvkPlayer::slot_stateChanged( QMediaPlayer::State state )
{
    qDebug() << "[vokoplayer]" << state;

    if ( state == QMediaPlayer::StoppedState )
    {
        ui->pushButtonStop->setEnabled( false );
        ui->pushButtonPause->setEnabled( false );
        ui->pushButtonPlay->setEnabled( true );
        ui->pushButtonPlay->setFocus();
        ui->sliderVideo->setValue( 0 );
        ui->labelDuration->setText( "00:00:00" );

        QIcon icon( QString::fromUtf8( ":/pictures/player/vokoscreen.png" ) );
        ui->labelPlayer->setPixmap( icon.pixmap( 200, 185 ) );
        parentMainWindow->setWindowTitle( oldWindowTitel );
        vk_showNormal();
    }
}


/*
 * Wird von mediplayer-notyfier periodisch aufgerufen.
 */
void QvkPlayer::slot_positionChanged( qint64 value )
{
    if ( mediaPlayer->state() == QMediaPlayer::PlayingState )
    {
       ui->sliderVideo->setValue( value / mediaPlayer->notifyInterval() );

       // Show playing time in label
       ui->labelDuration->setText( get_time( value ) );
    }
}


void QvkPlayer::slot_setNewImage( QImage image )
{
    ui->labelPlayer->setPixmap( QPixmap::fromImage( image.scaled( ui->framePlayer->width(),
                                                    ui->framePlayer->height(),
                                                    Qt::KeepAspectRatio,
                                                    Qt::SmoothTransformation ),
                                Qt::AutoColor) );
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
    (void) event;
    if ( parentMainWindow->isFullScreen() == true )
    {
        vk_showNormal();
    }
    else
    {
        vk_showFullscreen();
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
        if ( ui->widgetMenueBar->underMouse() )
        {
            mouseInWidgetX = event->pos().x() - ui->widgetMenueBar->pos().x();
            mouseInWidgetY = event->pos().y() - ui->widgetMenueBar->pos().y();
            ui->labelMovePicture->setCursor( Qt::SizeAllCursor );
            pressed = true;
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
    }
}
