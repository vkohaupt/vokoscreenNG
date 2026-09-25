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

#ifndef QVKCONVERT_MKV_GIF_WL_H
#define QVKCONVERT_MKV_GIF_WL_H

#include "ui_formMainWindow_wl.h"

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QTimer>

#include <gst/gst.h>


class QvkConvert_mkv_gif_wl: public QWidget
{
    Q_OBJECT
public:
    QvkConvert_mkv_gif_wl(Ui_formMainWindow_wl *m_ui);
    virtual ~QvkConvert_mkv_gif_wl();


private:
    QString muxerVideoFilename = "";
    static gboolean set_pipeline_null_idle(gpointer data);
    static GstBusSyncReply call_bus_message_convert_gif(GstBus *bus, GstMessage *message, gpointer data);
    static bool is_FileOpenByAnyProcess(QString targetFilePath);
    GstElement *pipelineGIF = nullptr;
    QTimer *m_timer = nullptr;


public slots:
    void slot_convert_mkv_to_gif(QString filePath);


private slots:
    void slot_onTick100ms();


signals:
    void signal_gst_eos(QString msg);
    void signal_gst_pipeline_finished();
    void signal_gst_progressbar_convert_gif(int value);
    void signal_gst_stream_start_progressbar();
    void signal_progress_changed(qreal percent);


protected:


private:
    Ui_formMainWindow_wl *ui;


};

#endif
