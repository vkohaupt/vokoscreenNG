/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2023 Volker Kohaupt
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

#ifndef QVKCAMERACONTROLLER_H
#define QVKCAMERACONTROLLER_H

#include "ui_formMainWindow_wl.h"

#include "QvkSpezialSlider.h"

#include <QObject>
#include <QLabel>
#include <QButtonGroup>
#include <QList>

#include <gst/gst.h>

class QvkCameraController_wl : public QObject
{
    Q_OBJECT

public:
    QvkCameraController_wl( Ui_formMainWindow_wl *ui_surface );
    virtual ~QvkCameraController_wl();


public slots:


private:
    Ui_formMainWindow_wl *ui;
    GstElement *pipelineCamera = nullptr;
    void get_allCameraDevices();


private slots:
    void slot_checkBoxCameraOnOff( bool bo );


protected:


signals:


};

#endif
