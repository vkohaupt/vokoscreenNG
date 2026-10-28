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
#include "QvkPlayerGst_wl.h"
#include "gst/video/videooverlay.h"
#include <gst/wayland/wayland.h>

#include <QDebug>
#include <QAudio>
#include <QGuiApplication>
#include <QWindow>
#include <QPlatformSurfaceEvent>
#include <QtGui/qpa/qplatformnativeinterface.h>

QvkPlayerGst_wl::QvkPlayerGst_wl()
{
}


void QvkPlayerGst_wl::on_pad_added( GstElement *element, GstPad *pad, gpointer data )
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


void QvkPlayerGst_wl::call_bus_message( GstBus *bus, GstMessage *message, gpointer data )
{
    Q_UNUSED(bus);
    switch (GST_MESSAGE_TYPE (message)) {
        case GST_MESSAGE_ERROR:
            qDebug().noquote() << global::nameOutput << "GST_MESSAGE_ERROR";
            break;
        case GST_MESSAGE_EOS:{
                QvkPlayerGst_wl *self = static_cast<QvkPlayerGst_wl*>(data);
                QMetaObject::invokeMethod(self, [self](){
                    emit self->signal_eos();
                }, Qt::QueuedConnection);
                break;
            }
        case GST_MESSAGE_DURATION_CHANGED:
            break;
        case GST_MESSAGE_STEP_DONE:
            qDebug().noquote() << global::nameOutput << "MESSAGE_STEP_DONE";
            break;
        case GST_MESSAGE_TAG:
            break;
        case GST_MESSAGE_STATE_CHANGED:
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


static GstBusSyncReply bus_sync_handler(GstBus *bus, GstMessage *message, gpointer data)
{
    Q_UNUSED(bus)
    if (gst_is_video_overlay_prepare_window_handle_message(message)) {
        QvkPlayerGst_wl *player = static_cast<QvkPlayerGst_wl*>(data);

        QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
        if (!native || !player->m_parent) return GST_BUS_PASS;

        struct wl_surface *surface = static_cast<struct wl_surface*>(
                    native->nativeResourceForWindow("surface", player->m_parent->windowHandle())
                    );

        if (surface){
            GstVideoOverlay *overlay = GST_VIDEO_OVERLAY(GST_MESSAGE_SRC(message));

            gst_video_overlay_set_window_handle(overlay, (guintptr)surface);

            // Größe an das Qt-Widget anpassen
            int width = player->m_parent->width();
            int height = player->m_parent->height();
            gst_video_overlay_set_render_rectangle(overlay, 0, 0, width, height);

            return GST_BUS_DROP;
        }
    }
    return GST_BUS_PASS;
}


void QvkPlayerGst_wl::play_pre()
{
    QString uri = "file://" + mediaFile;
    pipeline = gst_pipeline_new("pipeline");
    if (!pipeline){
        qDebug().noquote().nospace() << "[Player] Failed to create pipeline.";
        return;
    }

    playbin = gst_element_factory_make("playbin3", nullptr);
    if (!playbin) {
        qDebug().noquote().nospace() << "[Player] Failed to create playbin3 element.";
        return;
    }
    g_object_set(G_OBJECT(playbin), "uri", uri.toUtf8().constData(), nullptr);

    gst_bin_add_many(GST_BIN(pipeline), playbin, nullptr);

    // Bus-Sync-Handler registrieren, damit das Handle zum perfekten Zeitpunkt gesetzt wird
    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    if (bus) {
        gst_bus_set_sync_handler(bus, bus_sync_handler, this, nullptr);
        gst_object_unref(bus);
    }

    // Wayland Video-Sink erzwingen, um X11-Fallbacks (BadWindow Fehler) zu verhindern
    GstElement *vsink = gst_element_factory_make("waylandsink", "wayland-video-sink");
    if (!vsink) {
        qDebug().noquote().nospace()
                << global::nameOutput
                << "[Player] Failed to create 'waylandsink' element. Check your plugins (gst-plugins-bad).";
        return;
    }
    g_object_set(G_OBJECT(playbin), "video-sink", vsink, nullptr);

    // Erzwingen, dass das QWidget ein natives Fenster inkl. Wayland-Surface besitzt
    m_parent->setAttribute(Qt::WA_NativeWindow, true);
    m_parent->setAttribute(Qt::WA_DontCreateNativeAncestors, true); // Verhindert Artefakte im Widget-Baum
    //    m_parent->setAttribute(Qt::WA_OpaquePaintEvent, false);
    //    m_parent->setAttribute(Qt::WA_NoSystemBackground, true);
    //    m_parent->setStyleSheet("background: transparent;");

    // Sicherstellen, dass das native Fenster erstellt wurde
    m_parent->winId();

    // Extraktion des wl_display und wl_surface über das Qt Platform Native Interface
    QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
    if (!native) {
        qDebug().noquote().nospace()
                << global::nameOutput
                << "[Player] Failed to get QPlatformNativeInterface.";
        return;
    }

    // Den globalen Wayland-Display-Kontext von Qt extrahieren
    struct wl_display *display = static_cast<struct wl_display*>(
                native->nativeResourceForIntegration("display")
                );

    // Das Display an das GStreamer Wayland-Sink koppeln, damit beide den gleichen Kontext nutzen
    if (display) {
        GstContext *context = gst_wl_display_handle_context_new(display);
        gst_element_set_context(GST_ELEMENT(vsink), context);
        gst_context_unref(context);
    } else {
        qDebug().noquote().nospace()
                << global::nameOutput
                << "[Player] Could not retrieve native Wayland display context from Qt.";
    }

    // Testweiser Abruf der Surface zur Validierung im Log
    struct wl_surface *surface = static_cast<struct wl_surface*>(
                native->nativeResourceForWindow("surface", m_parent->windowHandle())
                );
    if (!surface) {
        qDebug().noquote().nospace()
                << global::nameOutput
                << "[Player] Warning: Wayland surface not ready yet (will be retried in bus handler).";
    }

    m_parent->update();
    // Alternativ: Dem Window-System signalisieren, dass das Widget expose-Events braucht
    if (m_parent->windowHandle()) {
        m_parent->windowHandle()->requestUpdate();
    }

    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_signal_watch(bus);
    g_signal_connect(bus, "message", (GCallback)call_bus_message, this);
    gst_object_unref(bus);
}


void QvkPlayerGst_wl::slot_play()
{
    if ( !is_pause() ) {
        play_pre();
    }

    GstStateChangeReturn sret = gst_element_set_state( pipeline, GST_STATE_PLAYING );

    if ( sret == GST_STATE_CHANGE_FAILURE ) {
        gst_element_set_state ( pipeline, GST_STATE_NULL );
        gst_object_unref( pipeline );
    }

    have_duration = false;
    timer->start();
}


void QvkPlayerGst_wl::slot_stop()
{
    frameSkip = false;
    timer->stop();
    gst_element_set_state( pipeline, GST_STATE_READY );
}


void QvkPlayerGst_wl::slot_pause()
{
    timer->stop();
    gst_element_set_state( pipeline, GST_STATE_PAUSED );
}


bool QvkPlayerGst_wl::is_running()
{
    GstState cur_state = GST_STATE_NULL;
    GstStateChangeReturn state_change;

    if ( pipeline != nullptr ) {
        /* 10 ms timeout */
        state_change = gst_element_get_state( pipeline, &cur_state, nullptr, 10000000 );
        Q_UNUSED(state_change)
    }
    if ( cur_state == GST_STATE_PLAYING ) {
        return true;
    } else {
        return false;
    }
}


bool QvkPlayerGst_wl::is_pause()
{
    GstState curState = GST_STATE_NULL;
    GstStateChangeReturn state_change;

    if ( pipeline != nullptr ) {
        /* 10 ms timeout */
        state_change = gst_element_get_state( pipeline, &curState, nullptr, 10000000 );
        Q_UNUSED(state_change)
    }

    if ( curState == GST_STATE_PAUSED ) {
        return true;
    } else {
        return false;
    }
}


void QvkPlayerGst_wl::set_video_widget(QWidget *widget)
{
    m_parent = widget;
}


void QvkPlayerGst_wl::init()
{
    timer = new QTimer;
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( 200 );
    connect(timer, &QTimer::timeout, this, &QvkPlayerGst_wl::slot_timer);
}


void QvkPlayerGst_wl::set_mediaFile( QString string )
{
    mediaFile = string;
}


QString QvkPlayerGst_wl::get_mediaFile()
{
    return mediaFile;
}


void QvkPlayerGst_wl::mute(bool bol)
{
    g_object_set(G_OBJECT(playbin), "mute", static_cast<gboolean>(bol), nullptr);
    qDebug().noquote().nospace() << global::nameOutput << "[Player][Mute]" << bol;
}


void QvkPlayerGst_wl::volume(qreal m_volume)
{
    qreal volume = QAudio::convertVolume(m_volume / qreal(100.0),
                                         QAudio::CubicVolumeScale,
                                         QAudio::LinearVolumeScale);
    g_object_set(G_OBJECT(playbin), "volume", volume, nullptr);
    qDebug().noquote().nospace() << global::nameOutput << "[Player][Volume]" << volume;
}


void QvkPlayerGst_wl::slot_timer()
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
    /*
    // Get mute from external e.g. System-Volumecontrol or Systray-mutecontrol
    if ( have_stream_audio == true ) {
        bool bol;
        g_object_get( G_OBJECT( audiosink ), "mute", &bol, nullptr );
        emit signal_mute( bol );
    }

    // Get volume from external e.g. System-Volumecontrol or Systray-volumecontrol
    if ( have_stream_audio == true ) {
        qreal volume;
        g_object_get( G_OBJECT( audiosink ), "volume", &volume, nullptr );
        qreal m_volume = QAudio::convertVolume( volume, QAudio::LinearVolumeScale, QAudio::CubicVolumeScale );
        emit signal_volume( m_volume );
    }
*/
}


// https://gstreamer.freedesktop.org/documentation/gstreamer/gstelement.html?gi-language=c
void QvkPlayerGst_wl::frameForward( qint64 slidervalue )
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


void QvkPlayerGst_wl::frameBackward( qint64 slidervalue )
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


void QvkPlayerGst_wl::playAfterFrameSkip( qint64 slidervalue )
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
void QvkPlayerGst_wl::goToTime( qint64 value )
{
    timer->start();
    gint64 seekPos = value * GST_SECOND / 1000;
    gst_element_seek_simple( pipeline,
                             GST_FORMAT_TIME,
                             GST_SEEK_FLAG_FLUSH, seekPos );
}
