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

QvkLevelMeterController::QvkLevelMeterController( QvkMainWindow *myParent )
{
    vkMainWindow = myParent;
    ui = vkMainWindow->ui;

    QList<QAudioDevice> devices = QMediaDevices::audioInputs();
    for ( int i = 0; i < devices.count(); i++ ) {
        qDebug().noquote() << global::nameOutput << "[Audio] [Levelmeter]" << devices.at(i).description() << devices.at(i).id();
    }

    QStringList list;
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QCheckBox *>();
    for ( int i = 0; i < listQCheckBox.count(); i++ ) {
        QCheckBox *checkBox = listQCheckBox.at(i);
        list << checkBox->accessibleName();
    }

    // Geräte ID vergleichen
    for ( int i = 0; i < listQCheckBox.count(); i++ ) {
        for ( int x = 0; x < devices.count(); x++ ) {
            QCheckBox *checkBox = listQCheckBox.at(i);
            if ( checkBox->accessibleName() == devices.at(x).id() ) {
                InputStart *inputStart = new InputStart( devices.at(x) );

                QProgressBar *progressBar = new QProgressBar;
                progressBar->setFixedWidth(160);
                progressBar->setFixedHeight(8);
                progressBar->setTextVisible( false);

                QToolButton *toolButton = new QToolButton;
                toolButton->setCheckable( true );
                toolButton->setFixedSize( 16, 16 );

                QHBoxLayout *hBoxLayout = new QHBoxLayout;
                hBoxLayout->addWidget( toolButton );
                hBoxLayout->addWidget( progressBar );
                ui->verticalLayout_volumeter->addLayout( hBoxLayout );

                connect( inputStart, SIGNAL( signal_level(int) ), progressBar, SLOT( setValue(int) ) );
                connect( toolButton, &QToolButton::clicked, this, [=]( bool bo ) {
                    if ( bo == false ) {
                        inputStart->slot_stop();
                        progressBar->setValue(0);
                    } else {
                        inputStart->slot_start();
                    }
                } );
            }
        }
    }

//    test_gstreamer_level();
}


QvkLevelMeterController::~QvkLevelMeterController()
{
}

static gboolean message_handler (GstBus * bus, GstMessage * message, gpointer data)
{
    Q_UNUSED(bus)
    Q_UNUSED(data)
    if (message->type == GST_MESSAGE_ELEMENT) {
        const GstStructure *s = gst_message_get_structure (message);
        const gchar *name = gst_structure_get_name (s);

        if (strcmp (name, "level") == 0) {
            gint channels;
            GstClockTime endtime;
            gdouble rms_dB, peak_dB, decay_dB;
            gdouble rms;
            const GValue *array_val;
            const GValue *value;
            GValueArray *rms_arr, *peak_arr, *decay_arr;
            gint i;

            if (!gst_structure_get_clock_time (s, "endtime", &endtime))
                g_warning ("Could not parse endtime");

            /* the values are packed into GValueArrays with the value per channel */
            array_val = gst_structure_get_value (s, "rms");
            rms_arr = (GValueArray *) g_value_get_boxed (array_val);

            array_val = gst_structure_get_value (s, "peak");
            peak_arr = (GValueArray *) g_value_get_boxed (array_val);

            array_val = gst_structure_get_value (s, "decay");
            decay_arr = (GValueArray *) g_value_get_boxed (array_val);

            /* we can get the number of channels as the length of any of the value
       * arrays */
            channels = rms_arr->n_values;
            g_print ("endtime: %" GST_TIME_FORMAT ", channels: %d\n",
                    GST_TIME_ARGS (endtime), channels);
            for (i = 0; i < channels; ++i) {

                g_print ("channel %d\n", i);
                value = g_value_array_get_nth (rms_arr, i);
                rms_dB = g_value_get_double (value);

                value = g_value_array_get_nth (peak_arr, i);
                peak_dB = g_value_get_double (value);

                value = g_value_array_get_nth (decay_arr, i);
                decay_dB = g_value_get_double (value);
                g_print ("    RMS: %f dB, peak: %f dB, decay: %f dB\n",
                        rms_dB, peak_dB, decay_dB);

                /* converting from dB to normal gives us a value between 0.0 and 1.0 */
                rms = pow (10, rms_dB / 20);
                g_print ("    normalized rms value: %f\n", rms);
            }
        }
    }
    /* we handled the message we want, and ignored the ones we didn't want.
   * so the core can unref the message for us */
    return TRUE;
}


void QvkLevelMeterController::test_gstreamer_level()
{
    GstElement *pulsesrc, *audioconvert, *level, *fakesink;
    GstElement *pipeline;
    GstCaps *caps;
    GstBus *bus;

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

    g_object_set( G_OBJECT( pulsesrc ), "device", "alsa_input.usb-046d_0809_A6307261-02.mono-fallback", NULL );

    /* make sure we'll get messages */
    g_object_set (G_OBJECT (level), "post-messages", TRUE, NULL);
    /* run synced and not as fast as we can */
    g_object_set (G_OBJECT (fakesink), "sync", TRUE, NULL);

    bus = gst_pipeline_get_bus (GST_PIPELINE(pipeline));
    gst_bus_add_watch( bus, &message_handler, nullptr);

    gst_element_set_state (pipeline, GST_STATE_PLAYING);
}
