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

#ifndef QVKDIRECTSOUNDCONTROLLER_H
#define QVKDIRECTSOUNDCONTROLLER_H

#include "ui_formMainwindow.h"
#include <QObject>
#include "QvkDirectSoundWatcher.h"

class QvkDirectSoundController: public QObject
{
    Q_OBJECT
    
public:
    QvkDirectSoundController( Ui_formMainWindow *ui_mainwindow );
    virtual ~QvkDirectSoundController();
    void init();
    void getAllDevices();
    QvkDirectSoundWatcher *vkDirectSoundWatcher;
    
public slots:
    void slot_audioDeviceSelected();
  
private:
    Ui_formMainWindow *ui;

    
private slots:
    void slot_pluggedInOutDevice( QString string );
    void slot_audioIconOnOff( bool state );


protected:
  
  
signals:
    void signal_haveAudioDeviceSelected( bool );

    
};

#endif
