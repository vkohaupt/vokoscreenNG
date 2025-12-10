/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2025 Volker Kohaupt
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

#ifndef QVKREGIONMARGINS_WL_H
#define QVKREGIONMARGINS_WL_H

#include "ui_formMainWindow_wl.h"

#include <QWidget>
#include <QPixmap>
#include <QVariantMap>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QColor>

class QvkRegionMargins_wl: public QWidget
{
    Q_OBJECT
public:
    QvkRegionMargins_wl( Ui_formMainWindow_wl *ui_mainwindow );
    virtual ~QvkRegionMargins_wl();
    int screenWidth = 400;
    int screenHeight = 400;
    QPixmap pixmap;
    int get_top();
    int get_right();
    int get_bottom();
    int get_left();
    QString whatWasClicked;


public slots:
    void slot_margins_response_snapshot( uint responseCode, QVariantMap results );
    void slot_snapshot();
    void slot_singleShot(bool bo);


private slots:


signals:
    void signal_regionMargins();


protected:  
    void paintEvent( QPaintEvent *event );
    void resizeEvent( QResizeEvent *event );


private:
    Ui_formMainWindow_wl *ui;
    int top = 0;
    int right = 0;
    int bottom = 0;
    int left = 0;
    QColor color;
    int lineWidth = 30;

};

#endif
