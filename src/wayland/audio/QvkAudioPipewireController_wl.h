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

#ifndef QVKAUDIOPIPEWIRECONTROLLER_WL_H
#define QVKAUDIOPIPEWIRECONTROLLER_WL_H

#include "ui_formMainWindow_wl.h"
#include "QvkAudioPipewireWatcher_wl.h"
//#include "QvkSpezialProgressBarAudio.h"

#include <QObject>
#include <QCheckBox>

class QvkAudioPipewireController_wl: public QObject
{
    Q_OBJECT

public:
    QvkAudioPipewireController_wl(Ui_formMainWindow_wl *ui_mainwindow);
    virtual ~QvkAudioPipewireController_wl();
    QvkAudioPipewireWatcher *vkAudioPipewireWatcher;
    bool wantCountdown = true;


public slots:
    void slot_audioDeviceSelected();


private:
    Ui_formMainWindow_wl *ui;


private slots:
    void slot_pluggedInOutDevice( QString string );


protected:


signals:
    void signal_haveAudioDeviceSelected( bool );
    void signal_newAudioDevice(QCheckBox *checkBox);

};

#endif
