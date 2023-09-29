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

#ifndef QVKIMAGEFROMTABS_WL_H
#define QVKIMAGEFROMTABS_WL_H

#include "ui_formMainWindow_wl.h"
#include "mainWindow_wl.h"

#include <QWindow>

class QvkImageFromTabs_wl : public QWindow
{
    Q_OBJECT

public:
    QvkImageFromTabs_wl( QvkMainWindow_wl *parent = 0 );
    virtual ~QvkImageFromTabs_wl();


public slots:


private slots:
    void slot_make_picture_from_tab();
    void slot_make_picture_from_tabs();


signals:


protected:  


private:
    QvkMainWindow_wl *myParent = Q_NULLPTR;
    int counterFile;
    int counter;
    QTimer *timer;

};

#endif
