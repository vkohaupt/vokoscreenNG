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

QvkPlayer::QvkPlayer( QMainWindow *parent, Ui_formMainWindow *m_ui_mainwindow ) : ui(new Ui::player)
{
    Q_UNUSED(parent)

    ui->setupUi(this);

    ui_mainwindow = m_ui_mainwindow;

    setWindowTitle( global::name + " " + global::version + " - " + "Player");
    setWindowIcon( QIcon( QString::fromUtf8( ":/pictures/logo/logo.png" ) ) );

    widget_Video = new QWidget;
    ui->verticalLayout->insertWidget( 0, widget_Video );
    ui->verticalLayout->setStretch( 0, 1 );
    widget_Video->setStyleSheet( "QWidget { background-color: black; }" );
    widget_Video->setVisible( false );

    // Only update by Video not by Qt.
    // If embedded and setUpdatesEnabled=true, Video flickering all one second, if the "label_playbackTime" is updated.
    widget_Video->setUpdatesEnabled( false );

    sliderVideo = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_2->insertWidget( 0, sliderVideo );
    ui->horizontalLayout_2->setStretch( 0, 1 );
    sliderVideo->setObjectName( "sliderVideo" );
    sliderVideo->setTracking( true );
    sliderVideo->setShowValue( false );
    sliderVideo->setEnabled( false );
    sliderVideo->setMinimum( 0 );
    sliderVideo->setMaximum( 1000000 );
    sliderVideo->show();

    sliderVolume = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout->addWidget( sliderVolume );
    sliderVolume->setObjectName( "sliderVolume" );
    sliderVolume->setTracking( true );
    sliderVolume->setShowValue( true );
    sliderVolume->setEnabled( true );
    sliderVolume->setMinimum( 0 );
    sliderVolume->setMaximum( 100 );
    sliderVolume->show();

    ui->verticalLayout->setContentsMargins( 0, 0, 0, 0 );
    ui->label_logo->setStyleSheet( "QWidget { background-color: black; }" );
    ui->widget_menuebar->setStyleSheet( "QWidget { background-color: lightGray; }" );

    ui->pushButtonPause->setVisible( false );
    ui->pushButtonPlay->setEnabled( false );
    ui->pushButtonStop->setEnabled( false );
}


void QvkPlayer::init()
{
    vkPlayerGst = new QvkPlayerGst();
    vkPlayerGst->set_winId( widget_Video->winId() );
    vkPlayerGst->init();

    connect( vkPlayerGst, SIGNAL( signal_EOS( QString ) ),        this, SLOT( slot_EOS( QString ) ) );
    connect( vkPlayerGst, SIGNAL( signal_duration( qint64 ) ),    this, SLOT( slot_duration( qint64 ) ) );
    connect( vkPlayerGst, SIGNAL( signal_currentTime( qint64 ) ), this, SLOT( slot_currentTime( qint64 ) ) );
    connect( vkPlayerGst, SIGNAL( signal_mute( bool ) ),          this, SLOT( slot_mute_from_Pulse( bool ) ) );
    connect( vkPlayerGst, SIGNAL( signal_volume( qreal ) ),       this, SLOT( slot_volume_from_pulse( qreal ) ) );

    connect( ui->pushButtonPlay, SIGNAL( clicked() ),  this, SLOT( slot_play() ) );
    connect( ui->pushButtonStop, SIGNAL( clicked() ),  this, SLOT( slot_stop() ) );
    connect( ui->pushButtonPause, SIGNAL( clicked() ), this, SLOT( slot_pause() ) );

    connect( ui->pushButtonFrameForward,  SIGNAL( clicked() ), this, SLOT( slot_frameForward() ) );
    connect( ui->pushButtonFrameBackward, SIGNAL( clicked() ), this, SLOT( slot_frameBackward() ) );

    connect( ui->pushButtonEmbedded, SIGNAL( clicked( bool ) ), this, SLOT( slot_embedded( bool ) ) );

    connect( ui->pushButtonOpenFile, SIGNAL( clicked() ), this, SLOT( slot_openFile() )  );

    connect( sliderVideo, SIGNAL( sliderMoved( int ) ), this, SLOT( slot_sliderVideoMoved( int ) ) );

    connect( ui->pushButtonMute, SIGNAL( clicked( bool ) ), this, SLOT( slot_mute( bool ) ) );

    connect( sliderVolume, SIGNAL( sliderMoved( int ) ), this, SLOT( slot_volume( int ) ) );

    timerHideMouse = new QTimer( this );
    timerHideMouse->setTimerType( Qt::PreciseTimer );
    connect( timerHideMouse, SIGNAL( timeout() ), this, SLOT( slot_hideMouse() ) );
    timerHideMouse->setInterval( 3000 );
    timerHideMouse->start();

    ui->pushButtonEmbedded->click();
}


QvkPlayer::~QvkPlayer()
{
    delete ui;
}


void QvkPlayer::slot_embedded( bool embedded )
{
    if ( embedded == true )
    {
        ui_mainwindow->verticalLayoutTabSidebarPlayer->insertWidget( 0, widget_Video, 1 );
        ui_mainwindow->verticalLayoutTabSidebarPlayer->insertWidget( 1, ui->label_logo, 1 );
        ui_mainwindow->verticalLayoutTabSidebarPlayer->insertWidget( 2, ui->widget_menuebar, 0 );
        widget_Video->setUpdatesEnabled( false );
        hide();
    } else {
        ui->verticalLayout->insertWidget( 0, widget_Video, 1 );
        ui->verticalLayout->insertWidget( 1, ui->label_logo, 1 );
        ui->verticalLayout->insertWidget( 2, ui->widget_menuebar, 0 );
        show();
    }
}


void QvkPlayer::slot_hideMouse()
{
    if ( ( widget_Video->underMouse() == true ) and ( ui->widget_menuebar->underMouse() == false ) ){
       setCursor( Qt::BlankCursor );
       if ( isFullScreen() == true ){
         ui->widget_menuebar->hide();
       }
    }
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
    if ( ( muted == true ) and ( ui->pushButtonMute->isChecked() == false ) )
    {
        ui->pushButtonMute->setChecked( true );
        ui->pushButtonMute->setIcon( QIcon( ":/pictures/player/audio-volume-muted.png" ) );
        return;
    }

    if ( ( muted == false ) and ( ui->pushButtonMute->isChecked() == true ) )
    {
        ui->pushButtonMute->setChecked( false );
        ui->pushButtonMute->setIcon( QIcon( ":/pictures/player/audio-volume-high.png" ) );
        return;
    }
}


void QvkPlayer::slot_mute( bool muted )
{
    vkPlayerGst->mute( muted );

    if ( muted == true )
    {
        ui->pushButtonMute->setIcon( QIcon( ":/pictures/player/audio-volume-muted.png" ) );
        return;
    }

    if ( muted == false )
    {
        ui->pushButtonMute->setIcon( QIcon( ":/pictures/player/audio-volume-high.png" ) );
        return;
    }
}


void QvkPlayer::slot_sliderVideoMoved( int value )
{
    if ( ui->pushButtonPlay->isHidden() == true ) {
        vkPlayerGst->playAfterFrameSkip( value );
    } else {
        vkPlayerGst->frameForward( value );
    }
}


void QvkPlayer::slot_play()
{
    if ( getMediaFile() > "" )
    {
        ui->label_logo->setVisible( false );
        widget_Video->setVisible( true );
        this->setMouseTracking( true );
        widget_Video->setMouseTracking( true );

        ui->pushButtonPlay->setVisible( false );
        ui->pushButtonPause->setVisible( true );

        ui->pushButtonPause->setEnabled( true );
        ui->pushButtonStop->setEnabled( true );

        sliderVideo->setEnabled( true );
        ui->pushButtonFrameBackward->setEnabled( true );
        ui->pushButtonFrameForward->setEnabled( true );

        if ( vkPlayerGst->frameSkip == true ){
            vkPlayerGst->playAfterFrameSkip( sliderVideo->value() );
        } else {
            vkPlayerGst->slot_play();
        }
    }
}


void QvkPlayer::slot_stop()
{
    vkPlayerGst->slot_stop();

    ui->pushButtonPlay->setVisible( true );
    ui->pushButtonPause->setVisible( false );
    ui->pushButtonPause->setEnabled( false );
    ui->pushButtonStop->setEnabled( false );
    ui->label_logo->setVisible( true );
    sliderVideo->setEnabled( false );
    ui->pushButtonFrameBackward->setEnabled( false );
    ui->pushButtonFrameForward->setEnabled( false );

    // Remove and hide the old widget_video
    ui->verticalLayout->removeWidget( widget_Video );
    widget_Video->hide();

    // Create a new widget_video
    widget_Video = new QWidget;
    vkPlayerGst->set_winId( widget_Video->winId() );

    if ( ui->pushButtonEmbedded->isChecked() == false )
    {
        ui->verticalLayout->insertWidget( 0, widget_Video );
        ui->verticalLayout->setStretch( 0, 1 );
    } else {
        ui_mainwindow->verticalLayoutTabSidebarPlayer->insertWidget( 0, widget_Video );
        ui_mainwindow->verticalLayoutTabSidebarPlayer->setStretch( 0, 1 );;
    }

    widget_Video->setStyleSheet( "QWidget { background-color: black; }" );
    widget_Video->setVisible( false );

    // Only update by Video not by Qt.
    // If embedded and setUpdatesEnabled=true, Video flickering all one second, if the "label_playbackTime" is updated.
    widget_Video->setUpdatesEnabled( false );

    if ( isFullScreen() == true ) {
        ui->verticalLayout->addWidget( ui->widget_menuebar );
        ui->widget_menuebar->show();
        unsetCursor();
        showNormal();
    }

    ui->label_duration->setText( "00:00:00" );
    ui->label_playbackTime->setText( "00:00:00" );
    sliderVideo->setSliderPosition( 0 );
}


void QvkPlayer::slot_pause()
{
    vkPlayerGst->slot_pause();

    ui->pushButtonPlay->setVisible( true );
    ui->pushButtonPause->setVisible( false );

    ui->pushButtonStop->setEnabled( true );
}


void QvkPlayer::slot_EOS( QString m_file )
{
    QFile file( m_file );
    qDebug().noquote().nospace() << global::nameOutput << "[Player] End of stream" << file.fileName();

    ui->pushButtonStop->click();
}


void QvkPlayer::slot_frameForward()
{
    vkPlayerGst->frameForward( sliderVideo->value() );

    ui->pushButtonPlay->setVisible( true );
    ui->pushButtonPause->setVisible( false );

}


void QvkPlayer::slot_frameBackward()
{
    vkPlayerGst->frameBackward( sliderVideo->value() );

    ui->pushButtonPlay->setVisible( true );
    ui->pushButtonPause->setVisible( false );
}


void QvkPlayer::setMediaFile( QString string )
{
    mediaFile = string;
    QFileInfo file( getMediaFile() );
    setWindowTitle( file.fileName() + " - " + global::name + " " + global::version + " - " + "Player" );
    vkPlayerGst->set_mediaFile( mediaFile );

    ui->label_logo->hide();
    ui->pushButtonPlay->setEnabled( true );
    ui->pushButtonStop->click();
    ui->pushButtonPlay->click();

    if ( ui->pushButtonEmbedded->isChecked() == true )
    {
        slot_embedded( true );
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
    ui->label_duration->setText( t.toString( "hh:mm:ss" ) );
}


void QvkPlayer::slot_currentTime( qint64 currentTimeMSecs ) //mycrosekunden
{
    sliderVideo->setSliderPosition( currentTimeMSecs );

    QTime time( 0, 0, 0 );
    QTime t = time.addMSecs( currentTimeMSecs );
    ui->label_playbackTime->setText( t.toString( "hh:mm:ss" ) );
}


void QvkPlayer::closeEvent( QCloseEvent *event )
{
    Q_UNUSED(event)
    ui->pushButtonEmbedded->click();
}


void QvkPlayer::mouseDoubleClickEvent( QMouseEvent *event )
{
    Q_UNUSED(event)

    if ( isFullScreen() == false )
    {
        if ( vkPlayerGst->is_running() == true )
        {
            widget_Video->setUpdatesEnabled( true );
            ui->widget_menuebar->setParent( widget_Video );
            ui->widget_menuebar->raise();
            ui->widget_menuebar->show();
            showFullScreen();
        }

        if ( ( vkPlayerGst->is_running() == false ) and ( vkPlayerGst->is_pause() == false ) )
        {
            widget_Video->setUpdatesEnabled( true );
            ui->widget_menuebar->setParent( ui->label_logo );
            ui->widget_menuebar->raise();
            ui->widget_menuebar->show();
            showFullScreen();
        }

        if ( ( vkPlayerGst->is_running() == false ) and ( vkPlayerGst->is_pause() == true ) )
        {
            widget_Video->setUpdatesEnabled( true );
            ui->widget_menuebar->setParent( widget_Video );
            ui->widget_menuebar->raise();
            ui->widget_menuebar->show();
            showFullScreen();
        }
    }
    else {
        showNormal();
        ui->verticalLayout->addWidget( ui->widget_menuebar );
        ui->widget_menuebar->raise();
        ui->widget_menuebar->show();
        widget_Video->setUpdatesEnabled( false );
    }
}


void QvkPlayer::keyPressEvent( QKeyEvent *event )
{
    if ( event->key() == Qt::Key_Escape ) {
        showNormal();
        ui->verticalLayout->addWidget( ui->widget_menuebar );
        ui->widget_menuebar->raise();
        ui->widget_menuebar->show();
        widget_Video->setUpdatesEnabled( false );
    }

    if ( ( event->key() == Qt::Key_F11 ) or ( event->key() == Qt::Key_F ) )
    {
        if ( isFullScreen() == false )
        {
            if ( vkPlayerGst->is_running() == true )
            {
                widget_Video->setUpdatesEnabled( true );
                ui->widget_menuebar->setParent( widget_Video );
                ui->widget_menuebar->raise();
                ui->widget_menuebar->show();
                showFullScreen();
            }

            if ( ( vkPlayerGst->is_running() == false ) and ( vkPlayerGst->is_pause() == false ) )
            {
                widget_Video->setUpdatesEnabled( true );
                ui->widget_menuebar->setParent( ui->label_logo );
                ui->widget_menuebar->raise();
                ui->widget_menuebar->show();
                showFullScreen();
            }

            if ( ( vkPlayerGst->is_running() == false ) and ( vkPlayerGst->is_pause() == true ) )
            {
                widget_Video->setUpdatesEnabled( true );
                ui->widget_menuebar->setParent( widget_Video );
                ui->widget_menuebar->raise();
                ui->widget_menuebar->show();
                showFullScreen();
            }
        }
        else {
            showNormal();
            ui->verticalLayout->addWidget( ui->widget_menuebar );
            ui->widget_menuebar->raise();
            ui->widget_menuebar->show();
            widget_Video->setUpdatesEnabled( false );
        }
    }
}

void QvkPlayer::mouseMoveEvent( QMouseEvent *event )
{
    unsetCursor();
    if ( isFullScreen() == true ){
      ui->widget_menuebar->show();
    }

    if ( ui->widget_menuebar->underMouse() and ( pressed == true ) and isFullScreen() )
    {
        ui->widget_menuebar->raise();
        ui->widget_menuebar->show();

        int setX;
        if ( ( event->pos().x() - mouseInWidgetX ) < 0 ) {
            setX = 0;
            mouseInWidgetX = event->pos().x();
        } else {
            if ( ( event->pos().x() + ( ui->widget_menuebar->width() - mouseInWidgetX ) ) > width() ) {
                setX = width() - ui->widget_menuebar->width();
                mouseInWidgetX = event->pos().x() - ui->widget_menuebar->pos().x();
            } else {
                setX = event->pos().x() - mouseInWidgetX;
            }
        }

        int setY;
        if ( ( event->pos().y() - mouseInWidgetY ) < 0 ) {
            setY = 0;
            mouseInWidgetY = event->pos().y();
        } else {
            if ( ( event->pos().y() + ( ui->widget_menuebar->height() - mouseInWidgetY ) ) > height() ) {
                setY = height() - ui->widget_menuebar->height();
                mouseInWidgetY = event->pos().y() - ui->widget_menuebar->pos().y();
            } else {
                setY = event->pos().y() - mouseInWidgetY;
            }
        }

        ui->widget_menuebar->move( setX, setY );
    }
}


void QvkPlayer::mousePressEvent( QMouseEvent *event )
{
    if ( ui->widget_menuebar->underMouse() )
    {
        mouseInWidgetX = event->pos().x() - ui->widget_menuebar->pos().x();
        mouseInWidgetY = event->pos().y() - ui->widget_menuebar->pos().y();
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
    if ( isFullScreen() == true )
    {
        int screenHeight = event->size().height();
        int screenWidth = event->size().width();
        int menueBarHeight = ui->widget_menuebar->height();
        int menueBarWidth = ui->widget_menuebar->width();
        ui->widget_menuebar->move( ( screenWidth - menueBarWidth ) / 2 , screenHeight - menueBarHeight );
    }
}
