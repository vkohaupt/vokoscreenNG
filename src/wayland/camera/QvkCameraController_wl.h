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

#ifndef QVKCAMERACONTROLLER_WL_H
#define QVKCAMERACONTROLLER_WL_H

#include "ui_formMainWindow_wl.h"

#include "QvkSpezialSlider.h"
#include "QvkCameraSingle_wl.h"

#include <QObject>
#include <QCheckBox>
#include <QCameraDevice>

class QvkCameraController_wl : public QObject
{
    Q_OBJECT

public:
    QvkCameraController_wl( Ui_formMainWindow_wl *m_GuiUi );
    virtual ~QvkCameraController_wl();
    void init();
    QvkCameraSingle_wl *vkCameraSingle_wl = nullptr;
    Ui_formMainWindow_wl *GuiUi;
    QvkSpezialSlider *vkSliderCameraWindowSize;
    QvkSpezialSlider *vkSliderCameraWindowZoom;
    QvkSpezialSlider *sliderCameraColorTemperature;


public slots:


private:


private slots:
    void slot_camera_added_or_removed(QString);
    void slot_disableEnableCameras(bool checked);


protected:


signals:
    void signal_forSystrayCameraOnOff(QCheckBox *checkBoxCameraOnOff);
    void signal_forSystrayCameraAdded(QCheckBox *checkBox);
    void signal_forSystrayCameraRemoved(QString value);


};

#endif
