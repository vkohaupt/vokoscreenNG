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
#include "mainWindow_wl.h"

#include <QWidget>
#include <QPalette>
#include <QTimer>

//----------------------------------------- Begin discover ----------------------------------------------------------------------------

// https://github.com/GStreamer/gst-docs/blob/master/examples/tutorials/basic-tutorial-9.c
#include <string.h>
#include <gst/gst.h>
#include <gst/pbutils/pbutils.h>

// Structure to contain all our information, so we can pass it around
typedef struct _CustomDataMP4
{
    GstDiscoverer *discoverer;
    GMainLoop *loop;
} CustomDataMP4;

//----------------------------------------- End discover ----------------------------------------------------------------------------


class QvkConvert_mkv_mp4_wl: public QWidget
{
    Q_OBJECT
public:
    QvkConvert_mkv_mp4_wl(QvkMainWindow_wl *vkMainWindow, Ui_formMainWindow_wl *vk_ui );
    virtual ~QvkConvert_mkv_mp4_wl();
    QvkMainWindow_wl *mainWindow;


private:
    static GstBusSyncReply call_bus_message_convert_mp4(GstBus *bus, GstMessage *message, gpointer user_data);
    static void print_tag_foreach(const GstTagList *tags, const gchar *tag, gpointer user_data);
    static void print_stream_info(GstDiscovererStreamInfo *info, gint depth);
    static void print_topology(GstDiscovererStreamInfo *info, gint depth);
    static void on_discovered_cb(GstDiscoverer *discoverer, GstDiscovererInfo *info, GError *err, CustomDataMP4 *data);
    static void on_finished_cb(GstDiscoverer *discoverer, CustomDataMP4 *data);

    QPalette paletteConvertWidget;
    QPalette paletteConvertLabel;
    QTimer *timer;
    GstElement *pipeline = nullptr;
    void msgbox_mkv_to_mp4();

public slots:


private slots:
    void slot_convert_openfiledialog_mkv_to_mp4();
    void slot_convert_mkv_to_mp4();
    void slot_lineEdit_Convert_eos_MP4();
    void slot_discover_start();
    void slot_timer();


signals:


protected:  


private:
    Ui_formMainWindow_wl *ui;


};

#endif
