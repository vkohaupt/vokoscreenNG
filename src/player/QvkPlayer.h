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

#ifndef QVKPLAYER_H
#define QVKPLAYER_H

#include "ui_formMainWindow.h"
#include "ui_player.h"
#include "QvkSpezialSlider.h"

#include <QWidget>
#include <QMediaPlayer>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QStyle>
#include <QMainWindow>

class QvkPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit QvkPlayer(QMainWindow *parent, Ui_formMainWindow *ui_mainwindow);
    ~QvkPlayer();
    void setMediaFile( QString string );
    Ui_player *ui;



public slots:
    void slot_play();


private slots:
    void slot_setNewImage( QImage image );
    void slot_mute();
    void slot_durationChanged( qint64 value );
    void slot_positionChanged( qint64 value );
    void slot_stateChanged( QMediaPlayer::State );
    void slot_mutedChanged( bool muted );
    void slot_toolButtonFullscreen();
    void slot_openFile();
    void slot_sliderVideoPressed();
    void slot_sliderVideoReleased();
    void slot_sliderVideoMoved( int value );
    void slot_volumeChanged(int value); // Funktioniert nicht mit Pulse
    void slot_hideMouse();
    void slot_sliderVideo_KeyRight_KeyLeft( int value );
    void slot_toolButtonFrameForward();
    void slot_toolButtonFrameBackward();


private:
    QMainWindow *parentMainWindow;
    Ui_formMainWindow *uiMainWindow;
    QMediaPlayer *mediaPlayer;
    QString get_time( qint64 value );
    int mouseInWidgetX;
    int mouseInWidgetY;
    bool pressed = false;
    void vk_showFullscreen();
    void vk_showNormal();
    QTimer *timerHideMouse;
    QString pathOpenFile;
    QString oldWindowTitel;
    bool playingFlag = false;
    QLabel *metaLabel;
    QFrame *metaFrame;
    QvkSpezialSlider *sliderVolume;
    QvkSpezialSlider *sliderVideo;

protected:
    void mouseDoubleClickEvent( QMouseEvent *event );
    void closeEvent(QCloseEvent *event);
    void keyPressEvent( QKeyEvent *event );
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void leaveEvent(QEvent *event);


};

#endif // QVKPLAYER_H
