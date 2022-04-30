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
#include "QvkPlayerGst.h"
#include "QvkFileDialog.h"
#include "global.h"

#include <QTime>
#include <QStandardPaths>
#include <QDebug>
#include <QMainWindow>
#include <QScreen>

QvkPlayer::QvkPlayer( Ui_formMainWindow *ui_formMainWindow ) : ui_player(new Ui_player)
{
    ui_player->setupUi( this );
    ui_gui = ui_formMainWindow;

    // Create a widget_video and set it to player
    widget_Video = new QWidget;
    ui_player->verticalLayout_player->insertWidget( 0, widget_Video );
    ui_player->verticalLayout_player->setStretch( 0, 1 );
    widget_Video->setStyleSheet( "QWidget { background-color: black; }" );
    widget_Video->setUpdatesEnabled( true );
    widget_Video->hide();

    widgetsToGui();

    ui_player->label_logo->setStyleSheet( "QWidget { background-color: black; }" );
    ui_player->widget_menuebar->setStyleSheet( "QWidget { background-color: lightGray; }" );

    ui_player->pushButtonPause->hide();

    sliderVideo = new QvkSpezialSlider( Qt::Horizontal );
    ui_player->horizontalLayout_2->insertWidget( 0, sliderVideo );
    ui_player->horizontalLayout_2->setStretch( 0, 1 );
    sliderVideo->setObjectName( "sliderVideo" );
    sliderVideo->setTracking( true );
    sliderVideo->setShowValue( false );
    sliderVideo->setEnabled( false );
    sliderVideo->setMinimum( 0 );
    sliderVideo->setMaximum( 1000000 );
    sliderVideo->show();

    sliderVolume = new QvkSpezialSlider( Qt::Horizontal );
    ui_player->horizontalLayout->addWidget( sliderVolume );
    sliderVolume->setObjectName( "sliderVolume" );
    sliderVolume->setTracking( true );
    sliderVolume->setShowValue( true );
    sliderVolume->setEnabled( true );
    sliderVolume->setMinimum( 0 );
    sliderVolume->setMaximum( 100 );
    sliderVolume->show();

    vkPlayerGst = new QvkPlayerGst();
    vkPlayerGst->set_winId( widget_Video->winId() );
    vkPlayerGst->init();

    connect( vkPlayerGst, SIGNAL( signal_EOS( QString ) ), this, SLOT( slot_EOS( QString ) ) );
    connect( vkPlayerGst, SIGNAL( signal_duration( qint64 ) ),    this, SLOT( slot_duration( qint64 ) ) );
    connect( vkPlayerGst, SIGNAL( signal_currentTime( qint64 ) ), this, SLOT( slot_currentTime( qint64 ) ) );

    connect( ui_player->pushButtonPlay,     SIGNAL( clicked() ), this, SLOT( slot_play() ) );
    connect( ui_player->pushButtonStop,     SIGNAL( clicked() ), this, SLOT( slot_stop() ) );
    connect( ui_player->pushButtonPause,    SIGNAL( clicked() ), this, SLOT( slot_pause() ) );
    connect( ui_player->pushButtonOpenFile, SIGNAL( clicked() ), this, SLOT( slot_openFile() ) );

    connect( ui_player->pushButtonFrameForward,  SIGNAL( clicked() ), this, SLOT( slot_frameForward() ) );
    connect( ui_player->pushButtonFrameBackward, SIGNAL( clicked() ), this, SLOT( slot_frameBackward() ) );

    connect( vkPlayerGst, SIGNAL( signal_mute( bool ) ),          this, SLOT( slot_mute_from_Pulse( bool ) ) );
    connect( vkPlayerGst, SIGNAL( signal_volume( qreal ) ),       this, SLOT( slot_volume_from_pulse( qreal ) ) );

    connect( sliderVideo, SIGNAL( sliderMoved( int ) ), this, SLOT( slot_sliderVideoMoved( int ) ) );
    connect( ui_player->pushButtonMute, SIGNAL( clicked( bool ) ), this, SLOT( slot_mute( bool ) ) );
    connect( sliderVolume, SIGNAL( sliderMoved( int ) ), this, SLOT( slot_volume( int ) ) );

    timeHideMouse = 3000;
    timerHideMouse = new QTimer( this );
    timerHideMouse->setTimerType( Qt::PreciseTimer );
    connect( timerHideMouse, SIGNAL( timeout() ), this, SLOT( slot_hideMouse() ) );
    timerHideMouse->setInterval( timeHideMouse );
    timerHideMouse->start();
}


QvkPlayer::~QvkPlayer()
{
}


void QvkPlayer::widgetsToGui()
{
    ui_player->widget_menuebar->show();
    ui_gui->verticalLayoutTabSidebarPlayer->addWidget( ui_player->widget_player );
    ui_player->verticalLayout_player->addWidget( ui_player->widget_menuebar );

    // Only update by Video not by Qt.
    // If embedded and setUpdatesEnabled=true, Video flickering all one second, if the "label_playbackTime" is updated.
    widget_Video->setUpdatesEnabled( false );

    hide();
}


void QvkPlayer::widgetsToPlayer()
{
    timerHideMouse->stop();
    timerHideMouse->start( timeHideMouse );
    unsetCursor();
    QList<QWidget *> listWidget = ui_gui->tabSidebarPlayer->findChildren<QWidget *>();
    for( int i = 0; i < listWidget.count(); i++ )
    {
        if ( listWidget.at(i)->objectName() == "widget_player" )
        {
            ui_player->verticalLayout_2->addWidget( listWidget.at(i) );
        }
        if ( listWidget.at(i)->objectName() == "widget_menuebar" )
        {
            listWidget.at(i)->setParent( widget_Video );
        }
    }

    QScreen *Screen = screen();
    int screenWidth = Screen->size().width();
    int screenHeight = Screen->size().height();
    int menueBarHeight = ui_player->widget_menuebar->height();
    int menueBarWidth = ui_player->widget_menuebar->width();
    ui_player->widget_menuebar->move( ( screenWidth - menueBarWidth ) / 2 , screenHeight - menueBarHeight );

    // Menu must updated if fullscreen
    widget_Video->setUpdatesEnabled( true );

    if ( vkPlayerGst->is_pause() == true )
    {
        QCursor cursor;
        cursor.setPos( cursor.pos() + QPoint( 1, 1 ) );

        QTimer::singleShot( 200, this, SLOT( slot_hideMouse() ) );
        QTimer::singleShot( 250, ui_player->widget_menuebar, SLOT( show() ) );
    }
}


void QvkPlayer::slot_hideMouse()
{
    if ( ( widget_Video->underMouse() == true ) and ( ui_player->widget_menuebar->underMouse() == false ) ){
       setCursor( Qt::BlankCursor );
       if ( isFullScreen() == true ){
         ui_player->widget_menuebar->hide();
       }
    }
}


void QvkPlayer::mouseMoveEvent( QMouseEvent *event )
{
    timerHideMouse->stop();
    timerHideMouse->start( timeHideMouse );

    unsetCursor();

    if ( isFullScreen() == true ){
      ui_player->widget_menuebar->show();
    }

    if ( ui_player->widget_menuebar->underMouse() and ( pressed == true ) and isFullScreen() )
    {
        ui_player->widget_menuebar->raise();
        ui_player->widget_menuebar->show();

        int setX;
        if ( ( event->pos().x() - mouseInWidgetX ) < 0 ) {
            setX = 0;
            mouseInWidgetX = event->pos().x();
        } else {
            if ( ( event->pos().x() + ( ui_player->widget_menuebar->width() - mouseInWidgetX ) ) > width() ) {
                setX = width() - ui_player->widget_menuebar->width();
                mouseInWidgetX = event->pos().x() - ui_player->widget_menuebar->pos().x();
            } else {
                setX = event->pos().x() - mouseInWidgetX;
            }
        }

        int setY;
        if ( ( event->pos().y() - mouseInWidgetY ) < 0 ) {
            setY = 0;
            mouseInWidgetY = event->pos().y();
        } else {
            if ( ( event->pos().y() + ( ui_player->widget_menuebar->height() - mouseInWidgetY ) ) > height() ) {
                setY = height() - ui_player->widget_menuebar->height();
                mouseInWidgetY = event->pos().y() - ui_player->widget_menuebar->pos().y();
            } else {
                setY = event->pos().y() - mouseInWidgetY;
            }
        }

        ui_player->widget_menuebar->move( setX, setY );
    }
}


void QvkPlayer::setMediaFile( QString string )
{
    bool isfull = false;
    if ( isFullScreen() == true ){
        isfull = true ;
    }

    mediaFile = string;
    QFileInfo file( getMediaFile() );
    setWindowTitle( file.fileName() + " - " + global::name + " " + global::version + " - " + "Player" );
    vkPlayerGst->set_mediaFile( mediaFile );

    ui_player->label_logo->hide();
    ui_player->pushButtonPlay->setEnabled( true );
    ui_gui->verticalLayoutTabSidebarPlayer->setStretch(0, 1);
    ui_player->pushButtonStop->click();
    ui_player->pushButtonPlay->click();

    if ( isfull == true ){
        widgetsToPlayer();
        showFullScreen();
    }
}


QString QvkPlayer::getMediaFile()
{
    return mediaFile;
}


void QvkPlayer::slot_openFile()
{
    QApplication::setDesktopSettingsAware( false );

    QvkFileDialog vkFileDialog( this );
    vkFileDialog.setVideoPath( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) );
    if ( vkFileDialog.exec() == QDialog::Accepted )
    {
        if ( !vkFileDialog.selectedFiles().empty() ) {
            setMediaFile( vkFileDialog.selectedFiles().at(0) );
        }
    }

    QApplication::setDesktopSettingsAware( true );
}


void QvkPlayer::slot_duration( qint64 durationMSecs )
{
    sliderVideo->setMaximum( durationMSecs );

    QTime time( 0, 0, 0 );
    QTime t = time.addMSecs( durationMSecs );
    ui_player->label_duration->setText( t.toString( "hh:mm:ss" ) );
}


void QvkPlayer::slot_currentTime( qint64 currentTimeMSecs ) //mycrosekunden
{
    sliderVideo->setSliderPosition( currentTimeMSecs );

    QTime time( 0, 0, 0 );
    QTime t = time.addMSecs( currentTimeMSecs );
    ui_player->label_playbackTime->setText( t.toString( "hh:mm:ss" ) );
}


void QvkPlayer::slot_frameForward()
{
    vkPlayerGst->frameForward( sliderVideo->value() );

    ui_player->pushButtonPlay->setVisible( true );
    ui_player->pushButtonPause->setVisible( false );

}


void QvkPlayer::slot_frameBackward()
{
    vkPlayerGst->frameBackward( sliderVideo->value() );

    ui_player->pushButtonPlay->setVisible( true );
    ui_player->pushButtonPause->setVisible( false );
}


void QvkPlayer::slot_volume_from_pulse( qreal volume )
{
    sliderVolume->setValue( qRound( volume*100 ) );
}


void QvkPlayer::slot_volume( int volume )
{
    if ( vkPlayerGst->have_stream_audio == true ) {
        if ( ( volume >= 0 ) and ( volume <= 100 ) ) {
            vkPlayerGst->volume( volume );
        }
    }
}


void QvkPlayer::slot_mute_from_Pulse( bool muted )
{
    if ( ( muted == true ) and ( ui_player->pushButtonMute->isChecked() == false ) )
    {
        ui_player->pushButtonMute->setChecked( true );
        ui_player->pushButtonMute->setIcon( QIcon( ":/pictures/player/audio-volume-muted.png" ) );
        return;
    }

    if ( ( muted == false ) and ( ui_player->pushButtonMute->isChecked() == true ) )
    {
        ui_player->pushButtonMute->setChecked( false );
        ui_player->pushButtonMute->setIcon( QIcon( ":/pictures/player/audio-volume-high.png" ) );
        return;
    }
}


void QvkPlayer::slot_mute( bool muted )
{
    vkPlayerGst->mute( muted );

    if ( muted == true )
    {
        ui_player->pushButtonMute->setIcon( QIcon( ":/pictures/player/audio-volume-muted.png" ) );
        return;
    }

    if ( muted == false )
    {
        ui_player->pushButtonMute->setIcon( QIcon( ":/pictures/player/audio-volume-high.png" ) );
        return;
    }
}


void QvkPlayer::slot_sliderVideoMoved( int value )
{
    if ( ui_player->pushButtonPlay->isHidden() == true ) {
        vkPlayerGst->playAfterFrameSkip( value );
    } else {
        vkPlayerGst->frameForward( value );
    }
}


void QvkPlayer::slot_play()
{
    if ( getMediaFile() > "" )
    {
        ui_player->label_logo->setVisible( false );
        widget_Video->setVisible( true );

        this->setMouseTracking( true );
        ui_player->widget_player->setMouseTracking(true);
        widget_Video->setMouseTracking( true );

        ui_player->pushButtonPlay->setVisible( false );
        ui_player->pushButtonPause->setVisible( true );

        ui_player->pushButtonPause->setEnabled( true );
        ui_player->pushButtonStop->setEnabled( true );

        sliderVideo->setEnabled( true );
        ui_player->pushButtonFrameBackward->setEnabled( true );
        ui_player->pushButtonFrameForward->setEnabled( true );

        if ( vkPlayerGst->frameSkip == true ){
            vkPlayerGst->playAfterFrameSkip( sliderVideo->value() );
        } else {
            vkPlayerGst->slot_play();
        }
    }
}


void QvkPlayer::slot_EOS( QString m_file )
{
    QFile file( m_file );
    qDebug().noquote().nospace() << global::nameOutput << "[Player] End of stream " << file.fileName();

    ui_player->widget_menuebar->show();
    ui_player->pushButtonStop->click();
}


void QvkPlayer::slot_stop()
{
    vkPlayerGst->slot_stop();

    ui_player->pushButtonPlay->setVisible( true );
    ui_player->pushButtonPause->setVisible( false );
    ui_player->pushButtonPause->setEnabled( false );
    ui_player->pushButtonStop->setEnabled( false );
    ui_player->label_logo->setVisible( true );
    sliderVideo->setEnabled( false );
    ui_player->pushButtonFrameBackward->setEnabled( false );
    ui_player->pushButtonFrameForward->setEnabled( false );

    // Remove and hide the old widget_video
    ui_player->verticalLayout_player->removeWidget( widget_Video );
    widget_Video->hide();

    // Create a new widget_video
    widget_Video = new QWidget;
    vkPlayerGst->set_winId( widget_Video->winId() );

    ui_player->verticalLayout_player->insertWidget( 0, widget_Video );
    ui_player->verticalLayout_player->setStretch( 0, 1 );

    widget_Video->setStyleSheet( "QWidget { background-color: black; }" );
    widget_Video->setVisible( false );

    // Only update by Video not by Qt.
    // If embedded and setUpdatesEnabled=true, Video flickering all one second, if the "label_playbackTime" is updated.
    widget_Video->setUpdatesEnabled( false );

    ui_player->label_duration->setText( "00:00:00" );
    ui_player->label_playbackTime->setText( "00:00:00" );
    sliderVideo->setSliderPosition( 0 );

    widgetsToGui();
}


void QvkPlayer::slot_pause()
{
    vkPlayerGst->slot_pause();
    ui_player->pushButtonPlay->setVisible( true );
    ui_player->pushButtonPause->setVisible( false );
    ui_player->pushButtonStop->setEnabled( true );
}


void QvkPlayer::mouseDoubleClickEvent( QMouseEvent *event )
{
    Q_UNUSED(event)

    if ( isFullScreen() == true )
    {
        widgetsToGui();
    }
}


void QvkPlayer::keyPressEvent( QKeyEvent *event )
{
    // fullscreen -> GUI
    if ( ( ( event->key() == Qt::Key_F11 ) or ( event->key() == Qt::Key_F ) or ( event->key() == Qt::Key_Escape ) ) and ( isFullScreen() == true ) ) {
        widgetsToGui();
        return;
    }
}


void QvkPlayer::mousePressEvent( QMouseEvent *event )
{
    if ( ui_player->widget_menuebar->underMouse() )
    {
        mouseInWidgetX = event->pos().x() - ui_player->widget_menuebar->pos().x();
        mouseInWidgetY = event->pos().y() - ui_player->widget_menuebar->pos().y();
        pressed = true;
    }
}


void QvkPlayer::mouseReleaseEvent( QMouseEvent *event )
{
    Q_UNUSED(event)
    pressed = false;
}


void QvkPlayer::resizeEvent( QResizeEvent *event )
{
    Q_UNUSED(event)
}
