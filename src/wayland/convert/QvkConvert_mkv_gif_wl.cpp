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
#include "QvkConvert_mkv_gif_wl.h"

#include <QTime>
#include <QMetaObject>
#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QList>
#include <QCheckBox>
#include <QTimer>

QvkConvert_mkv_gif_wl::QvkConvert_mkv_gif_wl(Ui_formMainWindow_wl *m_ui)
{
    ui = m_ui;

    m_timer = new QTimer(this);
    connect(this,
            &QvkConvert_mkv_gif_wl::signal_gst_stream_start_progressbar,
            this,
            [=](){
        m_timer->start(100);
    });

    connect(this,
            &QvkConvert_mkv_gif_wl::signal_gst_eos,
            this,
            [=](){
        m_timer->stop();
    });

    connect(m_timer,
            &QTimer::timeout,
            this,
            &QvkConvert_mkv_gif_wl::slot_onTick100ms);
}


QvkConvert_mkv_gif_wl::~QvkConvert_mkv_gif_wl()
{
}


void QvkConvert_mkv_gif_wl::slot_onTick100ms()
{
    if (!pipelineGIF){
        return;
    }

    gint64 current_position = 0;
    gint64 total_duration = 0;

    // Position im GST_FORMAT_TIME (Nanosekunden) abfragen
    if (gst_element_query_position(pipelineGIF, GST_FORMAT_TIME, &current_position) &&
            gst_element_query_duration(pipelineGIF, GST_FORMAT_TIME, &total_duration))
    {
        // Umrechnung von Nanosekunden in Millisekunden
        qreal pos_ms = current_position / 1000000;
        qreal dur_ms = total_duration / 1000000;

        // Umrechnen in Prozent
        qreal percent = 100 / dur_ms * pos_ms;

        // Ein Signal mit den Prozenten als Parameter auslösen
        emit signal_progress_changed(percent);
    }
}


gboolean QvkConvert_mkv_gif_wl::set_pipeline_null_idle(gpointer data)
{
    GstElement *pipeline = GST_ELEMENT(data);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    return G_SOURCE_REMOVE;
}


GstBusSyncReply QvkConvert_mkv_gif_wl::call_bus_message_convert_gif(GstBus *bus, GstMessage *message, gpointer data)
{
    Q_UNUSED(bus);
    static QTime timeStart;

    switch(GST_MESSAGE_TYPE (message))
    {
    case GST_MESSAGE_ELEMENT:{
        QvkConvert_mkv_gif_wl *self = static_cast<QvkConvert_mkv_gif_wl*>(data);
         const GstStructure *structure = gst_message_get_structure(message);
         qDebug() << gst_structure_to_string(structure);
         if (gst_structure_has_name(structure, "progress")){
             gint percent = 0;
             if (gst_structure_get_int(structure, "percent", &percent)){
                 QMetaObject::invokeMethod(self, [self, percent](){
                     emit self->signal_gst_progressbar_convert_gif(percent);
                 }, Qt::QueuedConnection);
             }
         }
        break;
    }
    case GST_MESSAGE_ERROR:{
        qDebug().noquote() << global::nameOutput << "[Convert] mkv to gif GST_MESSAGE_ERROR";
        break;
    }
    case GST_MESSAGE_EOS:{
        // ---------------- Begin pipeline auf null setzen -----------------------------
        QvkConvert_mkv_gif_wl *self = static_cast<QvkConvert_mkv_gif_wl*>(data);


        QMetaObject::invokeMethod(self, [self](){
            emit self->signal_progress_changed(100);
        }, Qt::QueuedConnection);


        GstElement *pipeline = self->pipelineGIF;
        g_idle_add(set_pipeline_null_idle, pipeline);
        // ---------------- End pipeline auf null setzen -----------------------------

        // ---------------- Begin Zeit für das Remuxen ermitteln -----------------------------
        qDebug().noquote() << global::nameOutput << "[Convert] mkv to gif GST_MESSAGE_EOS";
        QTime timeEnd = QTime::currentTime();
        qreal timeDiv = timeStart.msecsTo(timeEnd);
        QString msg = "[Convert] mkv to gif in " + QString::number(timeDiv/1000) + " seconds";
        qDebug().noquote() << global::nameOutput << "[Convert] mkv to gif in" << timeDiv/1000 << "seconds";
        // WICHTIG: Signal über einen Thread-Wechsel (QueuedConnection) senden.
        // Qt erledigt das automatisch, wenn Signal und Slot in verschiedenen Threads leben,
        // oder wenn wir invokeMethod nutzen:
        QMetaObject::invokeMethod(self, [self, msg](){
            emit self->signal_gst_eos(msg);
        }, Qt::QueuedConnection);
        // ---------------- Ende Zeit für das Remuxen ermitteln -----------------------------

        break;
    }
    case GST_MESSAGE_STREAM_START:{
        // ---------------- Begin Zeit für das Remuxen ermitteln -----------------------------
        qDebug().noquote() << global::nameOutput << "[Convert] mkv to gif GST_MESSAGE_STREAM_START";
        timeStart = QTime::currentTime();
        // ---------------- Ende Zeit für das Remuxen ermitteln -----------------------------

        // ---------------- Begin emit für Progressbar ----------------------------------
        QvkConvert_mkv_gif_wl *self = static_cast<QvkConvert_mkv_gif_wl*>(data);
        QMetaObject::invokeMethod(self, [self](){
            emit self->signal_gst_stream_start_progressbar();
        }, Qt::QueuedConnection);
        // ---------------- End emit für Progressbar ----------------------------------


        break;
    }
    case GST_MESSAGE_STATE_CHANGED:{
        // Nach Abschluß des remuxen wird die mkv gelöscht.
        // Dabei muß sichergestellt sein das GST_STATE_NULL für die gif Pipeline erreicht wurde.
        // Und mit der Funktion is_FileOpenByAnyProcess wird überprüft das ja kein Process
        // mehr auf die gif zugreift.
        QvkConvert_mkv_gif_wl *self = static_cast<QvkConvert_mkv_gif_wl*>(data);
        GstElement *pipeline = self->pipelineGIF;
        if (GST_MESSAGE_SRC(message) == GST_OBJECT(pipeline)){
            GstState old_state, new_state, pending;
            gst_message_parse_state_changed(message, &old_state, &new_state, &pending);
            qDebug().noquote() << global::nameOutput << "[Convert] mkv to gif Pipeline state changed from:"
                               << gst_element_state_get_name(old_state)
                               << "to" << gst_element_state_get_name(new_state);
        }

        GstState old_state, new_state, pending;
        gst_message_parse_state_changed(message, &old_state, &new_state, &pending);
        if (new_state == GST_STATE_NULL){
            QString muxerVideoFilename_MKV = self->muxerVideoFilename;
            QString muxerVideoFilename_GIF = self->muxerVideoFilename;;
            muxerVideoFilename_GIF.replace(".mkv", ".gif");

            QFile file(muxerVideoFilename_MKV);
            if (file.exists() == true){
                bool bo = is_FileOpenByAnyProcess(muxerVideoFilename_GIF);
                qDebug().noquote() << global::nameOutput
                                   << "[Convert] mkv to gif File is not open and ready to use:"
                                   << muxerVideoFilename_GIF;
                if (bo == false){
                    QFile file(muxerVideoFilename_MKV);
                    if (file.exists() == true){
                        if (file.remove() == true){
                            qDebug().noquote() << global::nameOutput
                                               << "[Convert] mkv to gif File was deleted:"
                                               << muxerVideoFilename_MKV;
                        }else{
                            qDebug().noquote() << global::nameOutput
                                               << "[Convert] mkv to gif File could not be deleted:"
                                               << muxerVideoFilename_MKV;
                        }
                        emit self->signal_gst_pipeline_finished();
                    }
                }
            }
        }

        break;
    }
    default:
        break;
    }
    return GST_BUS_PASS;
}


void QvkConvert_mkv_gif_wl::slot_convert_mkv_to_gif(QString filePath)
{
    qDebug().noquote();
    muxerVideoFilename = filePath;

    QFileInfo fileInfo(filePath);
    QString path = fileInfo.path();

    // gst-launch-1.0 -ev filesrc location=/home/vk/Videos/vokoscreenNG-ohne-audio.mkv
    // ! matroskademux
    // ! h264parse
    // ! openh264dec
    // ! queue
    // ! videoconvert
    // ! gifenc speed=30 repeat=-1
    // ! filesink location=test2.gif
    QString VK_Pipeline;
    QString fileNameGIF = fileInfo.baseName() + ".gif";
    VK_Pipeline = "filesrc location=\"" + filePath + "\""
            " ! matroskademux name=demux demux.video_0"
            " ! queue max-size-buffers=0 max-size-time=0 max-size-bytes=104857600"
            " ! h264parse"
            " ! openh264dec"
            " ! progressreport update-freq=1"
            " ! queue max-size-buffers=0 max-size-time=0 max-size-bytes=104857600"
            " ! videoconvert"
            " ! gifenc speed=30 repeat=-1"
            " ! filesink location=\"" + path + "/" + fileNameGIF + "\"";

    qDebug().noquote() << global::nameOutput << "[Convert]" << VK_Pipeline;

    QByteArray byteArray = VK_Pipeline.toUtf8();
    const gchar *line = byteArray.constData();
    GError *error = nullptr;
    this->pipelineGIF  = gst_parse_launch(line, &error);


    // progressreport anweisen, ELEMENT-Nachrichten auf den Bus zu werfen
    GstElement *progressreport = gst_bin_get_by_name(GST_BIN(pipelineGIF), "prog_report");
    if (progressreport) {
        g_object_set(progressreport, "do-query", FALSE, nullptr);
        gst_object_unref(progressreport);
    }

    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipelineGIF));
    gst_bus_set_sync_handler(bus, (GstBusSyncHandler)call_bus_message_convert_gif, this, nullptr);
    gst_object_unref(bus);

    // Start playing
    GstStateChangeReturn ret = gst_element_set_state( pipelineGIF, GST_STATE_PLAYING );
    if (ret == GST_STATE_CHANGE_FAILURE)   { qDebug().noquote() << global::nameOutput << "[Convert] mkv to gif" << "GST_STATE_CHANGE_FAILURE" << "Returncode =" << ret;   } // 0
    if (ret == GST_STATE_CHANGE_SUCCESS)   { qDebug().noquote() << global::nameOutput << "[Convert] mkv to gif" << "GST_STATE_CHANGE_SUCCESS" << "Returncode =" << ret;   } // 1
    if (ret == GST_STATE_CHANGE_ASYNC)     { qDebug().noquote() << global::nameOutput << "[Convert] mkv to gif" << "GST_STATE_CHANGE_ASYNC"   << "Returncode =" << ret;   } // 2
    if (ret == GST_STATE_CHANGE_NO_PREROLL){ qDebug().noquote() << global::nameOutput << "[Convert] mkv to gif" << "GST_STATE_CHANGE_NO_PREROLL" << "Returncode =" << ret; }// 3
    if (ret == GST_STATE_CHANGE_FAILURE)   {
        qDebug().noquote() << global::nameOutput << "[Convert] mkv to gif unable to set the pipeline to the playing state.";
        gst_object_unref(pipelineGIF);
        return;
    }
}


// Prüft, ob eine bestimmte Datei aktuell von IRGENDEINEM Prozess im System geöffnet ist.
// @param targetFilePath Der absolute Pfad zur zu prüfenden Datei.
// @return true, wenn die Datei geöffnet ist, sonst false.
bool QvkConvert_mkv_gif_wl::is_FileOpenByAnyProcess(QString targetFilePath)
{
    // Sicherstellen, dass wir den absoluten, bereinigten Pfad vergleichen
    QString cleanTargetPath = QFileInfo(targetFilePath).absoluteFilePath();
    if (cleanTargetPath.isEmpty()){
        return false;
    }

    // 1. Das /proc Verzeichnis öffnen
    QDir procDir("/proc");

    // Wir suchen nur nach Ordnern, die rein aus Zahlen bestehen (Prozess-IDs)
    QStringList pidDirs = procDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(const QString &pid : pidDirs){
        bool isNumber;
        pid.toInt(&isNumber);
        // Überspringe Ordner wie /proc/driver, /proc/sys etc.
        if (!isNumber){
            continue;
        }

        // Pfad zum File-Descriptor-Ordner des Prozesses (z.B. /proc/1234/fd)
        QString fdPath = QString("/proc/%1/fd").arg(pid);
        QDir fdDir(fdPath);

        // Falls wir keine Leserechte für den Prozess haben (z.B. Root-Prozesse)
        if (!fdDir.exists()){
            continue;
        }

        // Alle File Descriptors (Symlinks) in diesem Ordner auflisten
        QStringList fds = fdDir.entryList(QDir::Files | QDir::System | QDir::NoDotAndDotDot);

        for(int i = 0; i < fds.count(); ++i){
            QString linkPath = fdDir.absoluteFilePath(fds[i]);

            // QFileInfo::symLinkTarget() liest aus, wohin der Symlink im System zeigt
            QString openedFile = QFileInfo(linkPath).symLinkTarget();

            if (openedFile == cleanTargetPath){
                qDebug().noquote() << global::nameOutput << "This file is open:" << openedFile;
                return true; // Gefunden! Ein Prozess hat diese Datei offen.
            }
        }
    }

    return false; // Keine Übereinstimmung im gesamten System gefunden
}
