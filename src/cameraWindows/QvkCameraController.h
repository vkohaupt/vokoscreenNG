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

#ifndef QVKCAMERACONTROLLER_H
#define QVKCAMERACONTROLLER_H

#include "ui_formMainWindow.h"

#include "QvkCameraWatcher.h"
#include "QvkCameraWindow.h"
#include "QvkVideoSurface.h"
#include "QvkSpezialSlider.h"
#include "QvkCameraSettingsDialog.h"

#include <QCamera>
#include <QComboBox>

class QvkCameraController : public QObject
{
    Q_OBJECT

public:
    QvkCameraController( Ui_formMainWindow *ui_surface );
    virtual ~QvkCameraController();

    QvkCameraWindow *vkCameraWindow;
    void getAllDevices();
    QvkCameraSettingsDialog *vkCameraSettingsDialog;
    Ui_cameraSettingsDialog *uiCameraSettingsDialog;
    QPoint old_XY;
    Ui_formMainWindow *ui_formMainWindow;


public slots:


private slots:
    void slot_addedCamera( QString description, QString device );
    void slot_removedCamera( QString device );
    void slot_startCamera( bool value );
    void slot_statusChanged( QCamera::Status status );
    void slot_stateChanged( QCamera::State state );
    void slot_error( QCamera::Error error );
    void slot_setNewImage( QImage image );
    void slot_frameOnOff( bool value );
    void slot_resolutionChanged( int index);
    void slot_cameraWindow_move();


private:
    QCamera *camera = nullptr;
    QvkCameraWatcher *cameraWatcher;
    QvkVideoSurface *videoSurface;
    QvkSpezialSlider *sliderCameraWindowSize;
    QvkSpezialSlider *sliderCameraWindowZoom;
    int cameraWindow_X;
    int cameraWindow_Y;
    int oldCameraWindow_X;
    int oldVameraWindow_Y;


protected:


signals:
    void signal_setNewImage( QImage );


};

#endif
