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
#include "QvkConvert_mkv_mp4_wl.h"

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

QvkConvert_mkv_mp4_wl::QvkConvert_mkv_mp4_wl(Ui_formMainWindow_wl *m_ui)
{
    ui = m_ui;

    m_timer = new QTimer(this);
    connect(this,
            &QvkConvert_mkv_mp4_wl::signal_gst_stream_start_progressbar,
            this,
            [=](){
        m_timer->start(100);
    });

    connect(this,
            &QvkConvert_mkv_mp4_wl::signal_gst_eos,
            this,
            [=](){
        m_timer->stop();
    });

    connect(m_timer,
            &QTimer::timeout,
            this,
            &QvkConvert_mkv_mp4_wl::slot_onTick100ms);
}


QvkConvert_mkv_mp4_wl::~QvkConvert_mkv_mp4_wl()
{
}


void QvkConvert_mkv_mp4_wl::slot_onTick100ms()
{
    if (!pipelineMP4){
        return;
    }

    gint64 current_position = 0;
    gint64 total_duration = 0;

    // Position im GST_FORMAT_TIME (Nanosekunden) abfragen
    if (gst_element_query_position(pipelineMP4, GST_FORMAT_TIME, &current_position) &&
            gst_element_query_duration(pipelineMP4, GST_FORMAT_TIME, &total_duration))
    {
        // Umrechnung von Nanosekunden in Millisekunden
        qreal pos_ms = current_position / 1000000;
        qreal dur_ms = total_duration / 1000000;

        // Umrechnen in Prozent
        qreal percent = 100/dur_ms*pos_ms;

        // Ein Sihnal mit den Prozenten als Parameter auslösen
        emit signal_progress_changed(percent);
    }
}


gboolean QvkConvert_mkv_mp4_wl::set_pipeline_null_idle(gpointer data)
{
    GstElement *pipeline = GST_ELEMENT(data);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    return G_SOURCE_REMOVE;
}


GstBusSyncReply QvkConvert_mkv_mp4_wl::call_bus_message_convert_mp4(GstBus *bus, GstMessage *message, gpointer data)
{
    Q_UNUSED(bus);
    static QTime timeStart;

    switch(GST_MESSAGE_TYPE (message))
    {
    case GST_MESSAGE_ELEMENT:{
        break;
    }
    case GST_MESSAGE_ERROR:{
        qDebug().noquote() << global::nameOutput << "[Remux] mkv to mp4 GST_MESSAGE_ERROR";
        break;
    }
    case GST_MESSAGE_EOS:{
        // ---------------- Begin pipeline auf null setzen -----------------------------
        QvkConvert_mkv_mp4_wl *self = static_cast<QvkConvert_mkv_mp4_wl*>(data);
        GstElement *pipeline = self->pipelineMP4;
        g_idle_add(set_pipeline_null_idle, pipeline);
        // ---------------- End pipeline auf null setzen -----------------------------

        // ---------------- Begin Zeit für das Remuxen ermitteln -----------------------------
        qDebug().noquote() << global::nameOutput << "[Remux] mkv to mp4 GST_MESSAGE_EOS";
        QTime timeEnd = QTime::currentTime();
        qreal timeDiv = timeStart.msecsTo(timeEnd);
        QString msg = "[Remux] mkv to mp4 in " + QString::number(timeDiv/1000) + " seconds";
        qDebug().noquote() << global::nameOutput << "[Remux] mkv to mp4 in" << timeDiv/1000 << "seconds";
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
        qDebug().noquote() << global::nameOutput << "[Remux] mkv to mp4 GST_MESSAGE_STREAM_START";
        timeStart = QTime::currentTime();
        // ---------------- Ende Zeit für das Remuxen ermitteln -----------------------------

        // ---------------- Begin emit für Progressbar ----------------------------------
        QvkConvert_mkv_mp4_wl *self = static_cast<QvkConvert_mkv_mp4_wl*>(data);
        QMetaObject::invokeMethod(self, [self](){
            emit self->signal_gst_stream_start_progressbar();
        }, Qt::QueuedConnection);
        // ---------------- End emit für Progressbar ----------------------------------


        break;
    }
    case GST_MESSAGE_STATE_CHANGED:{
        // Nach Abschluß des remuxen wird die mkv gelöscht.
        // Dabei muß sichergestellt sein das GST_STATE_NULL für die MP4 Pipeline erreicht wurde.
        // Und mit der Funktion is_FileOpenByAnyProcess wird überprüft das ja kein Process
        // mehr auf die MP4 zugreift.
        QvkConvert_mkv_mp4_wl *self = static_cast<QvkConvert_mkv_mp4_wl*>(data);
        GstElement *pipeline = self->pipelineMP4;
        if (GST_MESSAGE_SRC(message) == GST_OBJECT(pipeline)){
            GstState old_state, new_state, pending;
            gst_message_parse_state_changed(message, &old_state, &new_state, &pending);
            qDebug().noquote() << global::nameOutput << "[Remux] mkv to mp4 Pipeline state changed from:"
                               << gst_element_state_get_name(old_state)
                               << "to" << gst_element_state_get_name(new_state);
        }

        GstState old_state, new_state, pending;
        gst_message_parse_state_changed(message, &old_state, &new_state, &pending);
        if (new_state == GST_STATE_NULL){
            QString muxerVideoFilename_MKV = self->muxerVideoFilename;
            QString muxerVideoFilename_MP4 = self->muxerVideoFilename;;
            muxerVideoFilename_MP4.replace(".mkv", ".mp4");

            QFile file(muxerVideoFilename_MKV);
            if (file.exists() == true){
                bool bo = is_FileOpenByAnyProcess(muxerVideoFilename_MP4);
                qDebug().noquote() << global::nameOutput
                                   << "[Remux] mkv to mp4 File is not open and ready to use:"
                                   << muxerVideoFilename_MP4;
                if (bo == false){
                    QFile file(muxerVideoFilename_MKV);
                    if (file.exists() == true){
                        if (file.remove() == true){
                            qDebug().noquote() << global::nameOutput
                                               << "[Remux] mkv to mp4 File was deleted:"
                                               << muxerVideoFilename_MKV;
                        }else{
                            qDebug().noquote() << global::nameOutput
                                               << "[Remux] mkv to mp4 File could not be deleted:"
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


void QvkConvert_mkv_mp4_wl::slot_remux_mkv_to_mp4(QString filePath)
{
    qDebug().noquote();

    muxerVideoFilename = filePath;

    QString audio_codec = "";
    if (get_SelectedAudioDevice().empty() == false){
        audio_codec = ui->comboBoxAudioCodec->currentText();
    }

    QFileInfo fileInfo(filePath);
    QString path = fileInfo.path();

    QString VK_Pipeline;
    if (audio_codec == ""){
        QString fileNameMP4 = fileInfo.baseName() + ".mp4";
        VK_Pipeline = "filesrc location=" + filePath +
                " ! matroskademux" +
                " ! h264parse" +
                " ! queue max-size-buffers=0 max-size-time=0 max-size-bytes=104857600" +
                " ! mp4mux name=mux" +
                " ! filesink location=" + path + "/" + fileNameMP4;
    }

    // gst-launch-1.0 -e filesrc location=/home/vk/Videos/vokoscreenNG-mit-audio.mkv ! matroskademux name=demux
    // mp4mux name=mux ! filesink location=test2.mp4
    // demux.video_0 ! queue ! h264parse ! mux.
    // demux.audio_0 ! queue ! mpegaudioparse ! mux.
    if (audio_codec == "mp3"){
        QString fileNameMP4 = fileInfo.baseName() + ".mp4";
        VK_Pipeline = "filesrc location=" + filePath +
                " ! matroskademux name=demux mp4mux name=mux" +
                " ! filesink location=\"" + path + "/" + fileNameMP4 + "\" " +
                "demux.video_0 ! queue max-size-buffers=0 max-size-time=0 max-size-bytes=104857600 ! h264parse ! mux.video_0" + " " +
                "demux.audio_0 ! queue max-size-buffers=0 max-size-time=0 max-size-bytes=104857600 ! mpegaudioparse ! mux.audio_0";
    }

    // gst-launch-1.0 -e filesrc location=/home/vk/Videos/vokoscreenNG-mit-audio.mkv ! matroskademux name=demux
    // mp4mux name=mux ! filesink location=test2.mp4
    // demux.video_0 ! queue ! h264parse ! mux.
    // demux.audio_0 ! queue ! opusparse ! mux.
    if (audio_codec == "opus"){
        QString fileNameMP4 = fileInfo.baseName() + ".mp4";
        VK_Pipeline = "filesrc location=" + filePath +
                " ! matroskademux name=demux mp4mux name=mux" +
                " ! filesink location=\"" + path + "/" + fileNameMP4 + "\" " +
                "demux.video_0 ! queue max-size-buffers=0 max-size-time=0 max-size-bytes=104857600 ! h264parse ! mux.video_0" + " " +
                "demux.audio_0 ! queue max-size-buffers=0 max-size-time=0 max-size-bytes=104857600 ! opusparse ! mux.audio_0";
    }

    qDebug().noquote() << global::nameOutput << "[Remux]" << VK_Pipeline;

    QByteArray byteArray = VK_Pipeline.toUtf8();
    const gchar *line = byteArray.constData();
    GError *error = nullptr;
    this->pipelineMP4  = gst_parse_launch(line, &error);


    // progressreport anweisen, ELEMENT-Nachrichten auf den Bus zu werfen
    GstElement *progressreport = gst_bin_get_by_name(GST_BIN(pipelineMP4), "prog_report");
    if (progressreport) {
        g_object_set(progressreport, "do-query", FALSE, nullptr);
        gst_object_unref(progressreport);
    }

    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipelineMP4));
    gst_bus_set_sync_handler(bus, (GstBusSyncHandler)call_bus_message_convert_mp4, this, nullptr);
    gst_object_unref(bus);

    // Start playing
    GstStateChangeReturn ret = gst_element_set_state( pipelineMP4, GST_STATE_PLAYING );
    if (ret == GST_STATE_CHANGE_FAILURE)   { qDebug().noquote() << global::nameOutput << "[Remux] mkv to mp4" << "GST_STATE_CHANGE_FAILURE" << "Returncode =" << ret;   } // 0
    if (ret == GST_STATE_CHANGE_SUCCESS)   { qDebug().noquote() << global::nameOutput << "[Remux] mkv to mp4" << "GST_STATE_CHANGE_SUCCESS" << "Returncode =" << ret;   } // 1
    if (ret == GST_STATE_CHANGE_ASYNC)     { qDebug().noquote() << global::nameOutput << "[Remux] mkv to mp4" << "GST_STATE_CHANGE_ASYNC"   << "Returncode =" << ret;   } // 2
    if (ret == GST_STATE_CHANGE_NO_PREROLL){ qDebug().noquote() << global::nameOutput << "[Remux] mkv to mp4" << "GST_STATE_CHANGE_NO_PREROLL" << "Returncode =" << ret; }// 3
    if (ret == GST_STATE_CHANGE_FAILURE)   {
        qDebug().noquote() << global::nameOutput << "[Remux] mkv to mp4 unable to set the pipeline to the playing state.";
        gst_object_unref(pipelineMP4);
        return;
    }
}


// Prüft, ob eine bestimmte Datei aktuell von IRGENDEINEM Prozess im System geöffnet ist.
// @param targetFilePath Der absolute Pfad zur zu prüfenden Datei.
// @return true, wenn die Datei geöffnet ist, sonst false.
bool QvkConvert_mkv_mp4_wl::is_FileOpenByAnyProcess(QString targetFilePath)
{
    // Sicherstellen, dass wir den absoluten, bereinigten Pfad vergleichen
    QString cleanTargetPath = QFileInfo(targetFilePath).absoluteFilePath();
    if (cleanTargetPath.isEmpty()) return false;

    // 1. Das /proc Verzeichnis öffnen
    QDir procDir("/proc");

    // Wir suchen nur nach Ordnern, die rein aus Zahlen bestehen (Prozess-IDs)
    QStringList pidDirs = procDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &pid : pidDirs) {
        bool isNumber;
        pid.toInt(&isNumber);
        if (!isNumber) continue; // Überspringe Ordner wie /proc/driver, /proc/sys etc.

        // Pfad zum File-Descriptor-Ordner des Prozesses (z.B. /proc/1234/fd)
        QString fdPath = QString("/proc/%1/fd").arg(pid);
        QDir fdDir(fdPath);

        // Falls wir keine Leserechte für den Prozess haben (z.B. Root-Prozesse)
        if (!fdDir.exists()) continue;

        // Alle File Descriptors (Symlinks) in diesem Ordner auflisten
        QStringList fds = fdDir.entryList(QDir::Files | QDir::System | QDir::NoDotAndDotDot);

        for (int i = 0; i < fds.count(); ++i) {
            QString linkPath = fdDir.absoluteFilePath(fds[i]);

            // QFileInfo::symLinkTarget() liest aus, wohin der Symlink im System zeigt
            QString openedFile = QFileInfo(linkPath).symLinkTarget();

            if (openedFile == cleanTargetPath) {
                qDebug().noquote() << global::nameOutput << "This file is open:" << openedFile;
                return true; // Gefunden! Ein Prozess hat diese Datei offen.
            }
        }
    }

    return false; // Keine Übereinstimmung im gesamten System gefunden
}


QStringList QvkConvert_mkv_mp4_wl::get_SelectedAudioDevice()
{
    QStringList list;
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QCheckBox *>();
    for ( int i = 0; i < listQCheckBox.count(); i++ ) {
        QCheckBox *checkBox = listQCheckBox.at(i);
        if ( checkBox->checkState() == Qt::Checked ) {
            list << checkBox->accessibleName();
        }
    }
    return list;
}
