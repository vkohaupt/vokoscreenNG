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

#ifndef QVKSYSTRAY_WL_H
#define QVKSYSTRAY_WL_H

#include "ui_formMainWindow_wl.h"

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>

class QvkSystray_wl: public QSystemTrayIcon
{
    Q_OBJECT

public:
    QvkSystray_wl( Ui_formMainWindow_wl *ui_mainwindow );
    virtual ~QvkSystray_wl();
    void init();
    QAction *startAction;
    QAction *stopAction;
    QAction *pauseAction;
    QAction *continueAction;
    void setMenuText();

    
private:
    Ui_formMainWindow_wl *ui;
    QMenu *menu;

    QAction *exitAction;
//    QAction *cameraAction;
    QAction *snapshotAction;


public slots:
    void slot_closeSystray();
    void slot_setSystrayIcon( bool );


private slots:
    void slot_hide();
    void slot_setRecordIcon( bool );
    void slot_setPauseIcon( bool );
//    void slot_currentIndexChanged( int index );


protected:
  
  
signals:
    void signal_SystemtrayIsClose();
    
};

#endif
