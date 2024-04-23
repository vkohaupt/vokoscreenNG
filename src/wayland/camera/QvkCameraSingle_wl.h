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

#ifndef QVKCAMERASINGLE_H
#define QVKCAMERASINGLE_H

#include "ui_formMainWindow_wl.h"
#include "QvkSpezialSlider.h"
#include "QvkCameraWindow_wl.h"

#include <QObject>
#include <QLabel>
#include <QRadioButton>
#include <QToolButton>
#include <QCheckBox>
#include <QWidget>
#include <QComboBox>

#include "glib.h"
#include <gst/gst.h>
#include <gst/pbutils/pbutils.h>
#include "gst/video/videooverlay.h"

class QvkCameraSingle_wl : public QObject
{
    Q_OBJECT

public:
    QvkCameraSingle_wl( Ui_formMainWindow_wl *ui_surface, QString device );
    virtual ~QvkCameraSingle_wl();


public slots:


private slots:
    void slot_checkBoxCameraOnOff( bool bo );


private:
    Ui_formMainWindow_wl *ui;
    GstElement *pipeline;
    GstElement *pipewiresrc;
    GstElement *videoconvert;
    GstElement *videosink;;
    QString device_id;
    QString device_name;



protected:


signals:


};

#endif
