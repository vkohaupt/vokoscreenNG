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

#ifndef QVKCONVERT_MKV_MP4_WL_H
#define QVKCONVERT_MKV_MP4_WL_H

#include "ui_formMainWindow_wl.h"

#include <QWidget>
#include <QString>

#include <gst/gst.h>


class QvkConvert_mkv_mp4_wl: public QWidget
{
    Q_OBJECT
public:
    QvkConvert_mkv_mp4_wl(Ui_formMainWindow_wl *m_ui);
    virtual ~QvkConvert_mkv_mp4_wl();


private:
    QStringList get_SelectedAudioDevice();
    QString muxerVideoFilename = "";
    static gboolean set_pipeline_null_idle(gpointer data);
    static GstBusSyncReply call_bus_message_convert_mp4(GstBus *bus, GstMessage *message, gpointer data);
    static bool is_FileOpenByAnyProcess(QString targetFilePath);
    GstElement *pipelineMP4 = nullptr;


public slots:
    void slot_remux_mkv_to_mp4(QString filePath);


private slots:


signals:
    void signal_gst_eos(QString msg);
    void signal_gst_pipeline_finished();
    void signal_gst_progressbar_convert_mp4(int value);


protected:


private:
    Ui_formMainWindow_wl *ui;


};

#endif
