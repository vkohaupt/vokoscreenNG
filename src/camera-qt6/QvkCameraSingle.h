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

#ifndef QVKCAMERASINGLE_H
#define QVKCAMERASINGLE_H

#include "ui_formMainWindow.h"
#include "QvkSpezialSlider.h"
#include "QvkCameraWindow.h"

#include <QObject>
#include <QCamera>
#include <QCameraDevice>
#include <QVideoSink>
#include <QMediaCaptureSession>


class QvkCameraSingle : public QObject
{
    Q_OBJECT

public:
    QvkCameraSingle( Ui_formMainWindow *ui_surface,  QCameraDevice m_cameraDevice );
    virtual ~QvkCameraSingle();
    QRadioButton *radioButtonCamera;
    QvkCameraWindow *vkCameraWindow;


public slots:


private slots:
    void slot_checkBoxCameraOnOff( bool value );
    void slot_videoFrameChanged(QVideoFrame videoFrame );
    void slot_radioButtonCurrentCameraClicked( bool value );
    void slot_frameOnOff( bool value );
    void slot_toolButtonClicked( bool value );


private:
    Ui_formMainWindow *ui;
    QCameraDevice cameraDevice;
    QCamera *camera;
    QMediaCaptureSession *captureSession;
    QVideoSink *videoSink;
    QToolButton *toolButton_camera_view_rectangle;
    QToolButton *toolButton_camera_view_ellipse;
    QToolButton *toolButton_camera_view_circle;
    QCheckBox *checkBoxCameraOnOff;
    QCheckBox *checkBoxCameraWindowFrame;
    QCheckBox *checkBoxCameraMirrorHorizontal;
    QCheckBox *checkBoxCameraMirrorVertical;
    QCheckBox *checkBoxCameraInvert;
    QCheckBox *checkBoxCameraGray;
    QCheckBox *checkBoxCameraMono;

protected:


signals:


};

#endif
