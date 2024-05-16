/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2024 Volker Kohaupt
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
#include "QvkLevelMeterController.h"

#include <QAudioDevice>
#include <QMediaDevices>
#include <QCheckBox>
#include <QProgressBar>
#include <QLineEdit>

QLineEdit *lineEditProgressBar;

QvkLevelMeterController::QvkLevelMeterController()
{
}


QvkLevelMeterController::~QvkLevelMeterController()
{
}

#ifdef Q_OS_UNIX
void QvkLevelMeterController::add_ProgressBar( QCheckBox *checkBox, QVBoxLayout *layout )
{
    QList<QAudioDevice > devices = QMediaDevices::audioInputs();
    for ( int i = 0; i < devices.count(); i++ ) {
        QAudioDevice audioDevice = devices.at(i);
        if ( audioDevice.id() == checkBox->accessibleName() ) {

            qDebug().noquote() << global::nameOutput << "[Audio] [Levelmeter]" << devices.at(i).description() << devices.at(i).id();

            setObjectName( "progressBarAudioDevice-" + checkBox->objectName().right(2) );
            setFixedHeight(4);
            setTextVisible(false);
            setMinimum(0);
            setMaximum(10000);
            setToolTip(checkBox->text());

            layout->addWidget( this );

            inputStart = new InputStart( devices.at(i) );
            connect( inputStart, SIGNAL( signal_level(int) ), this, SLOT( setValue(int) ) );
            connect( inputStart, SIGNAL( signal_level(int) ), this, SLOT( update() ) );
            inputStart->slot_start();
        }
    }
}
#endif


#ifdef Q_OS_WIN
void QvkLevelMeterController::add_ProgressBar( QCheckBox *checkBox, QVBoxLayout *layout )
{

    qDebug() << checkBox->objectName() << layout->objectName();

    QList<QAudioDevice > devices = QMediaDevices::audioInputs();
    for ( int i = 0; i < devices.count(); i++ ) {
        QAudioDevice audioDevice = devices.at(i);
//        if ( audioDevice.id() == checkBox->accessibleName() ) {
        if ( checkBox->accessibleName().contains( audioDevice.id() ) == true )  {
            qDebug().noquote() << global::nameOutput << "[Audio] [Levelmeter]" << devices.at(i).description() << devices.at(i).id();

            setObjectName( "progressBarAudioDevice-" + checkBox->objectName().right(2) );
            setFixedHeight(4);
            setTextVisible(false);
            setMinimum(0);
            setMaximum(10000);
            setToolTip(checkBox->text());

            layout->addWidget( this );

            inputStart = new InputStart( devices.at(i) );
            connect( inputStart, SIGNAL( signal_level(int) ), this, SLOT( setValue(int) ) );
            connect( inputStart, SIGNAL( signal_level(int) ), this, SLOT( update() ) );
            inputStart->slot_start();
        }
    }
}
#endif


void QvkLevelMeterController::remove_ProgressBar()
{
    inputStart->slot_stop();
    disconnect( inputStart, SIGNAL( signal_level(int) ), nullptr, nullptr );
}


/*
void QvkLevelMeterController::add_ProgressBar( QCheckBox *checkBox, QVBoxLayout *layout )
{
    lineEditProgressBar = new QLineEdit;

    setObjectName( "progressBarAudioDevice-" + checkBox->objectName().right(2) );
    setFixedHeight(4);
    setTextVisible(false);
    setMinimum(0);
    setMaximum(10000);
    setToolTip(checkBox->text());
    layout->addWidget( this );

    connect( lineEditProgressBar, SIGNAL( textChanged(QString) ), this, SLOT( slot_textChanged(QString) ) );

    test_gstreamer_level( checkBox->accessibleName() );

}

void QvkLevelMeterController::remove_ProgressBar()
{

}

void QvkLevelMeterController::slot_textChanged( QString value )
{
//    qDebug() << "---" << value << objectName();
    qreal level = value.toDouble();
    setValue( level*10000 );
}


gboolean QvkLevelMeterController::message_handler( GstBus *bus, GstMessage *message, gpointer data )
{
    Q_UNUSED(data)

    qDebug() << bus->object.name;

    if ( message->type == GST_MESSAGE_ELEMENT ) {
        const GstStructure *s = gst_message_get_structure( message );
        const gchar *name = gst_structure_get_name(s);
        if ( strcmp( name, "level" ) == 0 ) {
            gint channels;
            gdouble rms_dB;
            gdouble rms;
            const GValue *value;
            const GValue *array_val;
            GValueArray *rms_arr;
            gint i;

            array_val = gst_structure_get_value (s, "rms");
            rms_arr = (GValueArray *) g_value_get_boxed (array_val);

            channels = rms_arr->n_values;
            for ( i = 0; i < channels; ++i ) {
                value = g_value_array_get_nth( rms_arr, i);
                rms_dB = g_value_get_double( value );
                rms = pow( 10, rms_dB / 20 );
                g_print( "    normalized rms value: %f\n", rms );
                lineEditProgressBar->setText( QString::number( rms ) );
            }
        }
    }
    return TRUE;
}


void QvkLevelMeterController::test_gstreamer_level( QString device )
{
    GstElement *pulsesrc, *audioconvert, *level, *fakesink;
    GstElement *pipeline;
    GstCaps *caps;
    GstBus *bus;
    guint watch_id;

    caps = gst_caps_from_string ("audio/x-raw,channels=2");

    pipeline = gst_pipeline_new (NULL);
    g_assert (pipeline);
    pulsesrc = gst_element_factory_make ("pulsesrc", NULL);
    g_assert (pulsesrc);
    audioconvert = gst_element_factory_make ("audioconvert", NULL);
    g_assert (audioconvert);
    level = gst_element_factory_make ("level", NULL);
    g_assert (level);
    fakesink = gst_element_factory_make ("fakesink", NULL);
    g_assert (fakesink);

    gst_bin_add_many (GST_BIN (pipeline), pulsesrc, audioconvert, level,
                     fakesink, NULL);
    if (!gst_element_link (pulsesrc, audioconvert))
        g_error ("Failed to link audiotestsrc and audioconvert");
    if (!gst_element_link_filtered (audioconvert, level, caps))
        g_error ("Failed to link audioconvert and level");
    if (!gst_element_link (level, fakesink))
        g_error ("Failed to link level and fakesink");

    // g_object_set( G_OBJECT( pulsesrc ), "device", "alsa_input.usb-046d_0809_A6307261-02.mono-fallback", NULL );
    g_object_set( G_OBJECT( pulsesrc ), "device", device, NULL );

    // make sure we'll get messages
    g_object_set (G_OBJECT (level), "post-messages", TRUE, NULL);
    // run synced and not as fast as we can
    g_object_set (G_OBJECT (fakesink), "sync", TRUE, NULL);

    bus = gst_pipeline_get_bus (GST_PIPELINE(pipeline));
    gst_object_unref( bus );
    qDebug() << "11111111111111111111" << bus->object.name;

    QString *msg = new QString("");
    msg->append(device);
    qDebug() << "..................................." << *msg;

    watch_id = gst_bus_add_watch( bus, &message_handler, nullptr );
    qDebug() << "watch_id:" << watch_id;

    gst_element_set_state (pipeline, GST_STATE_PLAYING);
}
*/
