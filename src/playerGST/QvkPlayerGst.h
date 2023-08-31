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

#ifndef QVKPLAYERGST_H
#define QVKPLAYERGST_H

#include "glib.h"
#include "gst/gst.h"
#include "gst/video/videooverlay.h"

#include <QObject>
#include <QWidget>
#include <QTimer>

#include "QvkPlayerGstDiscover.h"

class QvkPlayerGst : public QObject
{
    Q_OBJECT
public:
    explicit QvkPlayerGst();
    void init();
    void set_winId( WId value );
    void set_mediaFile( QString string );
    QString get_mediaFile();
    bool is_running();
    QTimer *timer;
    bool frameSkip = false;
    void frameForward( qint64 slidervalue );
    void frameBackward( qint64 slidervalue );
    void playAfterFrameSkip( qint64 slidervalue );
    bool is_pause();
    QvkPlayerGstDiscover *vkPlayerGstDiscover;
    bool have_stream_video = false;
    bool have_stream_audio = false;
    void mute( bool bol );
    void volume( qreal volume );

    static void on_pad_added( GstElement *element, GstPad *pad, gpointer data );
    static void call_bus_message( GstBus *bus, GstMessage *message, gpointer user_data );


private:
    GstElement *pipeline = nullptr;
    GstElement *filesrc = nullptr;
    GstElement *decodebin = nullptr;
    GstElement *videoconvert = nullptr;
    GstElement *audioconvert = nullptr;
#ifdef Q_OS_LINUX
    GstElement *videosink = nullptr;
    GstElement *audiosink = nullptr;
#endif
#ifdef Q_OS_WIN
    GstElement *videosink = nullptr;
    GstElement *audiosink = nullptr;
#endif
    WId get_winId();
    WId m_winID;
    QString mediaFile;
    int timerID;
    bool have_duration = false;
    void play_pre();


private slots:
    void slot_timer();
    void slot_discover_quit( bool video, bool audio );


public slots:
    void slot_play();
    void slot_stop();
    void slot_pause();
    void slot_EOS(QString value);


signals:
    void signal_EOS( QString value );
    void signal_duration( qint64 duration_second );
    void signal_currentTime( qint64 currentTime );
    void signal_mute( bool bol );
    void signal_volume( qreal volume );


protected:


};

#endif // QVKGSTPLAYER_H
