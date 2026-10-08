/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2026 Volker Kohaupt
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

#ifndef QVKAUDIOWINDOWSWATCHER_H
#define QVKAUDIOWINDOWSWATCHER_H

#include <gst/gst.h>
#include "ui_formMainWindow.h"

#include <QObject>
#include <QString>

class QvkAudioWindowsWatcher: public QObject
{
    Q_OBJECT


public:
    QvkAudioWindowsWatcher(Ui_formMainWindow *ui_mainwindow);
    virtual ~QvkAudioWindowsWatcher();
    void startAudioWindowsMonitoring();
    static GstBusSyncReply my_AudioWindows_func(GstBus *bus, GstMessage *message, gpointer data);


public slots:


private:
    Ui_formMainWindow *ui;


private slots:


protected:
  
  
signals:
    void signal_audio_added_removed(QString device);


};

#endif
