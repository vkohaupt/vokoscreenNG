#include "QvkPlayer.h"
#include "ui_player.h"
#include "QvkPlayerVideoSurface.h"

QvkPlayer::QvkPlayer(QWidget *parent) : QWidget(parent),
                                        ui(new Ui::QvkPlayer)
{
    ui->setupUi(this);


    setWindowTitle( "vokoplayer" );
    QIcon icon;
    icon.addFile( QString::fromUtf8( ":/pictures/vokoscreen.png" ), QSize(), QIcon::Normal, QIcon::Off );
    setWindowIcon( icon );
    ui->frame->setStyleSheet( "background-color: black;" );
    ui->pushButtonPlay->setIcon( QIcon::fromTheme( "media-playback-start" ) );
    ui->pushButtonPause->setIcon( QIcon::fromTheme( "media-playback-pause" ) );
    ui->pushButtonStop->setIcon( QIcon::fromTheme( "media-playback-stop" ) );
    ui->pushButtonMute->setIcon( QIcon::fromTheme( "audio-volume-high" ) );

    mediaPlayer = new QMediaPlayer;
    ui->sliderVolume->setValue( 70 );
    mediaPlayer->setVolume( 70 );

    QvkPlayerVideoSurface *videoSurface = new QvkPlayerVideoSurface( this );
    connect( videoSurface, SIGNAL( signal_newPicture( QImage ) ), this, SLOT( slot_setNewImage( QImage ) ) );
    mediaPlayer->setVideoOutput( videoSurface );

    connect( ui->pushButtonPlay,  SIGNAL( clicked( bool ) ),    this,        SLOT( slot_play() ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ),    this,        SLOT( slot_pause() ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ),    this,        SLOT( slot_stop() ) );
    connect( ui->pushButtonMute,  SIGNAL( clicked( bool ) ),    this,        SLOT( slot_mute() ) );
    connect( ui->sliderVolume,    SIGNAL( sliderMoved( int ) ), mediaPlayer, SLOT( setVolume( int ) ) );
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
    mediaFile = string;
}


QString QvkPlayer::getMediaFile()
{
    return mediaFile;
}


void QvkPlayer::slot_play()
{
    if ( mediaPlayer->state() == QMediaPlayer::StoppedState )
    {
        show();
        mediaPlayer->setMedia( QUrl::fromLocalFile( "/home/vk/Downloads/vokoscreen.mp4" ) );
        //mediaPlayer->setMedia( QUrl::fromLocalFile( getMediaFile() ) );
        mediaPlayer->play();
        ui->pushButtonPlay->setEnabled( false );
        ui->pushButtonPause->setEnabled( true);
        ui->pushButtonStop->setEnabled( true );
        ui->pushButtonPause->setFocus();
    }

    if ( mediaPlayer->state() == QMediaPlayer::PausedState )
    {
        mediaPlayer->play();
        ui->pushButtonPlay->setEnabled( false );
        ui->pushButtonPause->setEnabled( true);
        ui->pushButtonStop->setEnabled( true );
        ui->pushButtonPause->setFocus();
    }

    qDebug() << "[vokoscreen}" << mediaPlayer->state();
}


void QvkPlayer::slot_pause()
{
    mediaPlayer->pause();
    ui->pushButtonPlay->setEnabled( true );
    ui->pushButtonPause->setEnabled( false );
    ui->pushButtonStop->setEnabled( true );
    ui->pushButtonPlay->setFocus();
    qDebug() << "[vokoscreen}" << mediaPlayer->state();
}


void QvkPlayer::slot_stop()
{
    mediaPlayer->stop();
    ui->pushButtonPlay->setEnabled( true );
    ui->pushButtonPause->setEnabled( false );
    ui->pushButtonStop->setEnabled( false );
    ui->pushButtonPlay->setFocus();
}


void QvkPlayer::slot_mute()
{
    if ( mediaPlayer->isMuted() == false )
    {
        mediaPlayer->setMuted( true );
        ui->pushButtonMute->setIcon( QIcon::fromTheme( "audio-volume-muted" ) );
        ui->sliderVolume->setEnabled( false );
        return;
    }

    if ( mediaPlayer->isMuted() == true )
    {
        mediaPlayer->setMuted( false );
        ui->pushButtonMute->setIcon( QIcon::fromTheme( "audio-volume-high" ) );
        ui->sliderVolume->setEnabled( true );
        return;
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
