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

#include "global.h"
#include "QvkPlayerGst.h"

#include <QDebug>
#include <QAudio>

QvkPlayerGst::QvkPlayerGst()
{
    vkPlayerGstDiscover = new QvkPlayerGstDiscover;
    connect( vkPlayerGstDiscover, SIGNAL( signal_discover_quit( bool, bool ) ), this, SLOT( slot_discover_quit( bool, bool ) ) );
}


void QvkPlayerGst::on_pad_added( GstElement *element, GstPad *pad, gpointer data )
{
    Q_UNUSED(element)

    GstPad *sinkpad;
    GstElement *decoder = (GstElement *) data;

    /* We can now link this pad with the vorbis-decoder sink pad */
    //g_print ("Dynamic pad created, linking demuxer/decoder\n");

    sinkpad = gst_element_get_static_pad( decoder, "sink" );
    gst_pad_link( pad, sinkpad );
    gst_object_unref( sinkpad );
}

// https://stackoverflow.com/questions/57556590/how-to-emit-signal-from-static-function-in-qt
// https://gstreamer.freedesktop.org/documentation/application-development/advanced/pipeline-manipulation.html?gi-language=c
/* called when a new message is posted on the bus */
int counter = 0;
void QvkPlayerGst::call_bus_message( GstBus *bus, GstMessage *message, gpointer user_data )
{
    Q_UNUSED(bus);
    Q_UNUSED(user_data)
    switch (GST_MESSAGE_TYPE (message)) {
        case GST_MESSAGE_ERROR:
            qDebug().noquote() << global::nameOutput << "GST_MESSAGE_ERROR";
            break;
        case GST_MESSAGE_EOS:
            global::lineEdit_EOS->setText( QString::number( counter++ ) );
            break;
        case GST_MESSAGE_DURATION_CHANGED:
            break;
        case GST_MESSAGE_STEP_DONE:
            qDebug().noquote() << global::nameOutput << "MESSAGE_STEP_DONE";
            break;
        case GST_MESSAGE_TAG:
            break;
        case GST_MESSAGE_STATE_CHANGED:
            {
/*                GstState old_state, new_state;
                gst_message_parse_state_changed ( message, &old_state, &new_state, NULL);
                QString message1 = GST_OBJECT_NAME ( message->src );
                QString oldState = gst_element_state_get_name( old_state );
                QString newState = gst_element_state_get_name( new_state );
*/
/*
                if( ( message1 == "pipeline" ) and ( oldState == "PAUSED" ) and ( newState == "READY" ) ) {
                    qDebug().noquote().nospace() << global::nameOutput << "[Player] " << "GST_MESSAGE_STATE_CHANGED from PAUSED to READY";
                }

                if( ( message1 == "pipeline" ) and ( oldState == "PLAYING" ) and ( newState == "PAUSED" ) ) {
                    qDebug().noquote().nospace() << global::nameOutput << "[Player] " << "GST_MESSAGE_STATE_CHANGED from PLAYING to PAUSED";
                }

                g_print ("Element %s changed state from %s to %s.\n",
                     GST_OBJECT_NAME (message->src),
                     gst_element_state_get_name (old_state),
                     gst_element_state_get_name (new_state));
*/
            }
            break;
        case GST_MESSAGE_STREAM_START:
            break;
        case GST_MESSAGE_APPLICATION:
            {
                break;
            }
        default:
            break;
    }
}


void QvkPlayerGst::init()
{
    global::lineEdit_EOS = new QLineEdit;
    connect( global::lineEdit_EOS, SIGNAL( textChanged( const QString ) ), this, SLOT( slot_EOS( QString ) ) );

    timer = new QTimer;
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( 20 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_timer() ) );
}


void QvkPlayerGst::set_winId( WId value )
{
    m_winID = value;
}


WId QvkPlayerGst::get_winId()
{
    return m_winID;
}


void QvkPlayerGst::set_mediaFile( QString string )
{
    mediaFile = string;
    vkPlayerGstDiscover->discover_file( mediaFile );
}


QString QvkPlayerGst::get_mediaFile()
{
    return mediaFile;
}


void QvkPlayerGst::slot_discover_quit( bool video, bool audio )
{
    have_stream_video = video;
    have_stream_audio = audio;
}


void QvkPlayerGst::mute( bool bol )
{
    qDebug() << "mute" << bol;
    g_object_set( G_OBJECT( audiosink ), "mute", bol, NULL );
}


void QvkPlayerGst::volume( qreal m_volume )
{
    qreal volume = QAudio::convertVolume( m_volume / qreal(100.0), QAudio::CubicVolumeScale, QAudio::LinearVolumeScale );
    g_object_set( G_OBJECT( audiosink ), "volume", volume, NULL );
}


static GstBus *bus;
void QvkPlayerGst::play_pre()
{
    if( ( have_stream_video == true ) and ( have_stream_audio == true ) )
    {
        pipeline  = gst_pipeline_new( "pipeline" );
        filesrc   = gst_element_factory_make( "filesrc", nullptr );
        decodebin = gst_element_factory_make( "decodebin", nullptr );
        videoconvert = gst_element_factory_make( "videoconvert", nullptr );
        audioconvert = gst_element_factory_make( "audioconvert", nullptr );
        videosink = gst_element_factory_make( "xvimagesink", nullptr );
        audiosink = gst_element_factory_make( "pulsesink", nullptr );

        gst_bin_add_many( GST_BIN( pipeline ), filesrc, decodebin, audioconvert, audiosink, videoconvert, videosink, nullptr );
        gst_element_link( filesrc, decodebin  );

        gst_element_link( decodebin, videoconvert );
        gst_element_link( videoconvert, videosink );
        g_signal_connect( decodebin, "pad-added", G_CALLBACK( on_pad_added ), videoconvert );

        gst_element_link( decodebin, audioconvert );
        gst_element_link( audioconvert, audiosink );
        g_signal_connect( decodebin, "pad-added", G_CALLBACK( on_pad_added ), audioconvert );

        gst_video_overlay_set_window_handle( GST_VIDEO_OVERLAY( videosink ), get_winId() );

        g_object_set( G_OBJECT( filesrc ), "location", get_mediaFile().toUtf8().constData(), NULL );

        bus = gst_pipeline_get_bus( GST_PIPELINE ( pipeline ) );
        gst_bus_add_signal_watch( bus );
        g_signal_connect( bus, "message", ( GCallback ) call_bus_message, pipeline );
        gst_object_unref( bus );
        return;
    }


    if( ( have_stream_video == true ) and ( have_stream_audio == false ) )
    {
        pipeline  = gst_pipeline_new( "pipeline" );
        filesrc   = gst_element_factory_make( "filesrc", nullptr );
        decodebin = gst_element_factory_make( "decodebin", nullptr );
        videoconvert = gst_element_factory_make( "videoconvert", nullptr );
        videosink = gst_element_factory_make( "xvimagesink", nullptr );

        gst_bin_add_many( GST_BIN( pipeline ), filesrc, decodebin, videoconvert, videosink, nullptr );
        gst_element_link( filesrc, decodebin  );

        gst_element_link( decodebin, videoconvert );
        gst_element_link( videoconvert, videosink );
        g_signal_connect( decodebin, "pad-added", G_CALLBACK( on_pad_added ), videoconvert );

        gst_video_overlay_set_window_handle( GST_VIDEO_OVERLAY( videosink ), get_winId() );

        g_object_set( G_OBJECT( filesrc ), "location", get_mediaFile().toUtf8().constData(), NULL );

        bus = gst_pipeline_get_bus( GST_PIPELINE ( pipeline ) );
        gst_bus_add_signal_watch( bus );
        g_signal_connect( bus, "message", ( GCallback ) call_bus_message, pipeline );
        gst_object_unref( bus );
        return;
    }

    if( ( have_stream_video == false ) and ( have_stream_audio == true ) )
    {
        pipeline  = gst_pipeline_new( "pipeline" );
        filesrc   = gst_element_factory_make( "filesrc", nullptr );
        decodebin = gst_element_factory_make( "decodebin", nullptr );
        audioconvert = gst_element_factory_make( "audioconvert", nullptr );
        audiosink = gst_element_factory_make( "pulsesink", nullptr );

        gst_bin_add_many( GST_BIN( pipeline ), filesrc, decodebin, audioconvert, audiosink, nullptr );
        gst_element_link( filesrc, decodebin  );

        gst_element_link( decodebin, audioconvert );
        gst_element_link( audioconvert, audiosink );
        g_signal_connect( decodebin, "pad-added", G_CALLBACK( on_pad_added ), audioconvert );

        g_object_set( G_OBJECT( filesrc ), "location", get_mediaFile().toUtf8().constData(), NULL );

        bus = gst_pipeline_get_bus( GST_PIPELINE ( pipeline ) );
        gst_bus_add_signal_watch( bus );
        g_signal_connect( bus, "message", ( GCallback ) call_bus_message, pipeline );
        gst_object_unref( bus );
        return;
    }
}


void QvkPlayerGst::slot_play()
{
    if ( !is_pause() ) {
        play_pre();
    }

    if ( ( have_stream_video == true ) or ( have_stream_audio == true ) )
    {
        GstStateChangeReturn sret = gst_element_set_state( pipeline, GST_STATE_PLAYING );

        if ( sret == GST_STATE_CHANGE_FAILURE ) {
            gst_element_set_state ( pipeline, GST_STATE_NULL );
            gst_object_unref( pipeline );
        }

        have_duration = false;
        timer->start();
    }
}


void QvkPlayerGst::slot_stop()
{
    frameSkip = false;
    timer->stop();
    gst_element_set_state( pipeline, GST_STATE_READY );
}


void QvkPlayerGst::slot_pause()
{
    timer->stop();
    gst_element_set_state( pipeline, GST_STATE_PAUSED );
}


void QvkPlayerGst::slot_EOS( QString value )
{
    Q_UNUSED(value)

//    if ( frameSkip == false )
    {
        emit signal_EOS( get_mediaFile() );
    }
}


bool QvkPlayerGst::is_running()
{
    GstState cur_state = GST_STATE_NULL;
    GstStateChangeReturn state_change;

    if ( pipeline != NULL  ) {
        /* 10 ms timeout */
        state_change = gst_element_get_state( pipeline, &cur_state,  NULL, 10000000 );
        Q_UNUSED(state_change)
    }
    if ( cur_state == GST_STATE_PLAYING ) {
        return true;
    } else {
        return false;
    }
}


bool QvkPlayerGst::is_pause()
{
    GstState curState = GST_STATE_NULL;
    GstStateChangeReturn state_change;

    if ( pipeline != NULL  ) {
        /* 10 ms timeout */
        state_change = gst_element_get_state( pipeline, &curState,  NULL, 10000000 );
        Q_UNUSED(state_change)
    }

    if ( curState == GST_STATE_PAUSED ) {
        return true;
    } else {
        return false;
    }
}


void QvkPlayerGst::slot_timer()
{
    if ( have_duration == false ) {
        gint64 duration;
        gst_element_query_duration( pipeline, GST_FORMAT_TIME, &duration );
        if ( duration > -1 ) {
            emit signal_duration( duration / 1000 / 1000 );
            have_duration = true;
        }
    }

    gint64 currentTime;
    if ( gst_element_query_position( pipeline, GST_FORMAT_TIME, &currentTime ) ) {
        emit signal_currentTime( currentTime / 1000 / 1000 );
    }

    // Get mute from external e.g. Pulseaudio-Volumecontrol or Systray-mutecontrol
    if ( have_stream_audio == true ) {
        bool bol;
        g_object_get( G_OBJECT( audiosink ), "mute", &bol, NULL );
        emit signal_mute( bol );
    }

    // Get volume from external e.g. Pulseaudio-Volumecontrol or Systray-volumecontrol
    if ( have_stream_audio == true ) {
        qreal volume;
        g_object_get( G_OBJECT( audiosink ), "volume", &volume, NULL );
        qreal m_volume = QAudio::convertVolume( volume, QAudio::LinearVolumeScale, QAudio::CubicVolumeScale );
        emit signal_volume( m_volume );
    }
}


// https://gstreamer.freedesktop.org/documentation/gstreamer/gstelement.html?gi-language=c
void QvkPlayerGst::frameForward( qint64 slidervalue )
{
    timer->start();
    frameSkip = true;
    gint64 sliderValue = slidervalue;
    gint64 frameRate = 25;
    gint64 timeInterval = 1000000000 / frameRate;
    gint64 start = sliderValue * GST_SECOND / 1000;
    gint64 stop  = sliderValue * GST_SECOND / 1000 + timeInterval;

    gst_element_seek( pipeline,
                      1.0,
                      GST_FORMAT_TIME,
                      GST_SEEK_FLAG_FLUSH,
                      GST_SEEK_TYPE_SET, start,
                      GST_SEEK_TYPE_SET, stop );

    gst_element_set_state( pipeline, GST_STATE_PLAYING );
}


void QvkPlayerGst::frameBackward( qint64 slidervalue )
{
    timer->start();
    frameSkip = true;
    gint64 sliderValue = slidervalue;
    gint64 frameRate = 25;
    gint64 timeInterval = 1000000000 / frameRate;
    gint64 start  = sliderValue * GST_SECOND / 1000 - timeInterval - 1000000;
    gint64 stop   = sliderValue * GST_SECOND / 1000 - timeInterval;

    gst_element_seek( pipeline,
                      1.0,
                      GST_FORMAT_TIME,
                      GST_SEEK_FLAG_FLUSH,
                      GST_SEEK_TYPE_SET, start,
                      GST_SEEK_TYPE_SET, stop );

    gst_element_set_state( pipeline, GST_STATE_PLAYING );
}


void QvkPlayerGst::playAfterFrameSkip( qint64 slidervalue )
{
    timer->start();
    frameSkip = false;
    gint64 sliderValue = slidervalue;
    gint64 start  = sliderValue * GST_SECOND / 1000;

    gst_element_seek_simple( pipeline,
                             GST_FORMAT_TIME,
                             GST_SEEK_FLAG_FLUSH, start );

    gst_element_set_state( pipeline, GST_STATE_PLAYING );
}

/*
 * The player move to the seek position in play and pause mode
 */
void QvkPlayerGst::goToTime( qint64 value )
{
    timer->start();
    gint64 seekPos = value * GST_SECOND / 1000;
    gst_element_seek_simple( pipeline,
                             GST_FORMAT_TIME,
                             GST_SEEK_FLAG_FLUSH, seekPos );
}
