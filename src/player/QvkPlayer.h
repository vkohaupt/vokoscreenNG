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

#ifndef QVKPLAYER_H
#define QVKPLAYER_H

#include "ui_player.h"
#include "QvkPlayerGst.h"
#include "QvkSpezialSlider.h"
#include "ui_formMainWindow.h"

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QCloseEvent>

class QvkPlayer : public QWidget
{
    Q_OBJECT

public:
    //explicit QvkPlayer();
    QvkPlayer( QMainWindow *parent, Ui_formMainWindow *m_ui_mainwindow );
    ~QvkPlayer();
    void init();
    void setMediaFile( QString string );
    Ui_player *ui;


public slots:
    void slot_play();


private slots:
    void slot_stop();
    void slot_pause();
    void slot_openFile();
    void slot_EOS( QString m_file );
    void slot_duration( qint64 durationMSecs );
    void slot_currentTime( qint64 currentTimeMSecs );
    void slot_frameForward();
    void slot_frameBackward();
    void slot_sliderVideoMoved( int );
    void slot_mute( bool bol );
    void slot_mute_from_Pulse( bool muted );
    void slot_volume( int volume );
    void slot_volume_from_pulse( qreal );
    void slot_hideMouse();
    void slot_embedded( bool bo );


private:
    Ui_formMainWindow *ui_mainwindow;
    QvkPlayerGst *vkPlayerGst;
    QString mediaFile;
    QString getMediaFile();
    QWidget *widget_Video = nullptr;
    QvkSpezialSlider *sliderVideo;
    QvkSpezialSlider *sliderVolume;
    int mouseInWidgetX;
    int mouseInWidgetY;
    bool pressed = false;
    QTimer *timerHideMouse;


protected:
    void mouseDoubleClickEvent( QMouseEvent *event );
    void keyPressEvent( QKeyEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void resizeEvent( QResizeEvent *event );
    void closeEvent( QCloseEvent *event );


};

#endif // QVKPLAYER_H
