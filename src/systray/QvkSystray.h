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

#ifndef QVKSYSTRAY_H
#define QVKSYSTRAY_H

#include "ui_formMainWindow.h"

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>

class QvkSystray: public QSystemTrayIcon
{
    Q_OBJECT

public:
    QvkSystray( Ui_formMainWindow *ui_mainwindow );
    virtual ~QvkSystray();
    void init();
    QAction *startAction;
    void setMenueText();

    
public slots:
    void slot_closeSystray();


private:
    Ui_formMainWindow *ui;
    QMenu *menu;
    QAction *stopAction;
    QAction *pauseAction;
    QAction *continueAction;
    QAction *exitAction;

    QAction *cameraAction;
    QAction *magnifierAction;
    QAction *showclickAction;
    QAction *haloAction;
    QAction *snapshotAction;


private slots:
    void slot_hide();
    void slot_setRecordIcon( bool );
    void slot_setSystrayIcon( bool );
    void slot_setPauseIcon( bool );
    void slot_currentIndexChanged( int index );
    void slot_shortcutSystray( QString device, QString shortcut );


protected:
  
  
signals:
    void signal_SystemtrayIsClose();
    
};

#endif
