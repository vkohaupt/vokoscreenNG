#include "QvkPlayer.h"
#include "ui_player.h"
#include "QvkPlayerVideoSurface.h"

#include <QTime>

QvkPlayer::QvkPlayer(QWidget *parent) : QWidget(parent),
                                        ui(new Ui::QvkPlayer)
{
    ui->setupUi(this);

    setWindowTitle( "vokoplayer" );
    QIcon icon( QString::fromUtf8( ":/pictures/vokoscreen.png" ) );
    setWindowIcon( icon );
    ui->frame->setStyleSheet( "background-color: black;" );
    ui->pushButtonPlay->setIcon( QIcon::fromTheme( "media-playback-start" , style()->standardIcon( QStyle::SP_MediaPlay ) ) );
    ui->pushButtonPause->setIcon( QIcon::fromTheme( "media-playback-pause", style()->standardIcon( QStyle::SP_MediaPause ) ) );
    ui->pushButtonStop->setIcon( QIcon::fromTheme( "media-playback-stop"  , style()->standardIcon( QStyle::SP_MediaStop ) ) );
    ui->pushButtonMute->setIcon( QIcon::fromTheme( "audio-volume-high"    , style()->standardIcon( QStyle::SP_MediaVolume ) ) );
    ui->toolButtonFullscreen->setIcon( QIcon::fromTheme( "view-fullscreen" ) );

    mediaPlayer = new QMediaPlayer;
    ui->sliderVolume->setValue( 70 );
    mediaPlayer->setVolume( 70 );

    QvkPlayerVideoSurface *videoSurface = new QvkPlayerVideoSurface( this );
    connect( videoSurface, SIGNAL( signal_newPicture( QImage ) ), this, SLOT( slot_setNewImage( QImage ) ) );
    mediaPlayer->setVideoOutput( videoSurface );

    connect( mediaPlayer,         SIGNAL( mutedChanged( bool ) ),      this,        SLOT( slot_mutedChanged( bool ) ) );
    connect( mediaPlayer,         SIGNAL( durationChanged( qint64 ) ), this,        SLOT( slot_durationChanged( qint64 ) ) );
    connect( mediaPlayer,         SIGNAL( positionChanged( qint64 ) ), this,        SLOT( slot_positionChanged( qint64 ) ) );
    connect( mediaPlayer,         SIGNAL( stateChanged( QMediaPlayer::State ) ), this, SLOT( slot_stateChanged( QMediaPlayer::State ) ) );

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

    connect( ui->sliderVolume,    SIGNAL( sliderMoved( int ) ), mediaPlayer, SLOT( setVolume( int ) ) );
    //connect( ui->sliderPosition,  SIGNAL( sliderMoved( int ) ),        this,        SLOT( slot_sliderMoved( int ) ) );

    connect( ui->pushButtonMute,  SIGNAL( clicked( bool ) ), this, SLOT( slot_mute() ) );

    connect( ui->toolButtonFullscreen, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonFullscreen() ) );



}

QvkPlayer::~QvkPlayer()
{
    delete ui;
}


void QvkPlayer::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    mediaPlayer->stop();
}


void QvkPlayer::setMediaFile( QString string )
{
//    mediaPlayer->setMedia( QUrl::fromLocalFile( "/home/vk/Downloads/vokoscreen.mp4" ) );
    mediaPlayer->setMedia( QUrl::fromLocalFile( string ) );
}


void QvkPlayer::slot_play()
{
    show();
    mediaPlayer->play();
    ui->pushButtonPlay->setEnabled( false );
    ui->pushButtonPause->setEnabled( true);
    ui->pushButtonStop->setEnabled( true );
    ui->pushButtonPause->setFocus();
}


void QvkPlayer::slot_mute()
{
    ui->pushButtonMute->setEnabled( false );

    if ( mediaPlayer->isMuted()== true )
    {
        mediaPlayer->setMuted( false );
        ui->pushButtonMute->setFocus();
        return;
    }

    if ( mediaPlayer->isMuted()== false )
    {
        mediaPlayer->setMuted( true );
        ui->pushButtonMute->setFocus();
        return;
    }
}


void QvkPlayer::slot_mutedChanged( bool muted )
{
    if ( muted == true )
    {
        ui->pushButtonMute->setIcon( QIcon::fromTheme( "audio-volume-muted", style()->standardIcon( QStyle::SP_MediaVolumeMuted ) ) );
        ui->sliderVolume->setEnabled( false );
        ui->pushButtonMute->setEnabled( true );
        return;
    }

    if ( muted == false )
    {
        ui->pushButtonMute->setIcon( QIcon::fromTheme( "audio-volume-high", style()->standardIcon( QStyle::SP_MediaVolume ) ) );
        ui->sliderVolume->setEnabled( true );
        ui->pushButtonMute->setEnabled( true );
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
    ui->sliderVideo->setMaximum( value / 1000 );

    // Show lenght from video in label
    ui->labelVideoLenght->setText( get_time( value ) );
}

/*
 * deaktiviert
 * Wenn User den Slider bewegt wird das Videobild nach vorne oder nach hinten gesetzt
 */
void QvkPlayer::slot_sliderMoved( int value )
{
    mediaPlayer->setPosition( value );
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
    }
}


/*
 * Wird von notyfier periodisch aufgerufen.
 */
void QvkPlayer::slot_positionChanged( qint64 value )
{
    if ( mediaPlayer->state() == QMediaPlayer::PlayingState )
    {
       ui->sliderVideo->setValue( value/1000 );

       // Show playing time in label
       ui->labelDuration->setText( get_time( value ) );
    }
}


void QvkPlayer::slot_setNewImage( QImage image )
{
    QTransform transform;
    QImage transformedImage = image.transformed( transform, Qt::SmoothTransformation );

    // Passt Bild am Fensters an
    transformedImage = transformedImage.scaled( ui->frame->width(), ui->frame->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation );

    ui->labelPlayer->setPixmap( QPixmap::fromImage( transformedImage, Qt::AutoColor) );

//    ui->labelPlayer->setPixmap( QPixmap::fromImage( image.scaled( ui->frame->width(), ui->frame->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation ), Qt::AutoColor) );

}


void QvkPlayer::slot_toolButtonFullscreen()
{
    if ( isFullScreen() == true )
    {
        ui->widgetMenue->show();
        showNormal();
    }
    else
    {
        ui->widgetMenue->hide();
        showFullScreen();
    }

}


void QvkPlayer::mouseDoubleClickEvent( QMouseEvent *event )
{
    (void) event;
    if ( isFullScreen() == true )
    {
        ui->widgetMenue->show();
        showNormal();
    }
    else
    {
        ui->widgetMenue->hide();
        showFullScreen();
    }
}


void QvkPlayer::keyPressEvent( QKeyEvent *event )
{
    if ( event->key() == Qt::Key_Escape )
    {
        ui->widgetMenue->show();
        showNormal();
        return;
    }

    if ( event->key() == Qt::Key_F11 )
    {
        if ( isFullScreen() == true )
        {
            ui->widgetMenue->show();
            showNormal();
        }
        else
        {
            ui->widgetMenue->hide();
            showFullScreen();
        }
        return;
    }
}
