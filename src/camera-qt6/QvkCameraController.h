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
#include "QvkSpezialSlider.h"
#include "QvkCameraSingle.h"

#include <QObject>
#include <QComboBox>
#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QCamera>
#include <QMediaDevices>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QLabel>
#include <QRadioButton>

class QvkCameraController : public QObject
{
    Q_OBJECT

public:
    QvkCameraController( Ui_formMainWindow *ui_surface );
    virtual ~QvkCameraController();


public slots:


private slots:
//    void slot_radioButtonCameraTextClicked( bool value );
//    void slot_checkBoxCameraOnOff( bool value );
//    void slot_videoFrameChanged(QVideoFrame videoFrame);

private:
    Ui_formMainWindow *ui;
//    QCamera *camera = nullptr;
//    QCamera *camera;
//    QVideoWidget *videoWidget;
//    QMediaCaptureSession *captureSession;
//    QMediaDevices m_devices;
//    QVideoSink *videoSink;
//    QLabel *videolabel;
//    QComboBox *comboBoxCameraFormat;

    QvkCameraSingle *vkCameraSingle;
    QvkSpezialSlider *sliderCameraWindowSize;
    QvkSpezialSlider *sliderCameraWindowZoom;
    QButtonGroup *buttonGroup = new QButtonGroup;
    QLabel *labelCurrentCamera = new QLabel;

//    void addCamera( QString description, QByteArray device );
//    void removedCamera( QString device );
//    QString getComboBoxFormat();



protected:
//    void mousePressEvent( QMouseEvent *event );


signals:
//    void signal_addedCamera( QString, QString );
//    void signal_removedCamera( QString );


};

#endif




/*
#include "ui_formMainWindow.h"

#include "QvkCameraWatcher.h"
#include "QvkCameraWindow.h"
#include "QvkVideoSurface.h"
#include "QvkSpezialSlider.h"

#include <QCamera>
#include <QComboBox>

class QvkCameraController : public QObject
{
    Q_OBJECT

public:
    QvkCameraController( Ui_formMainWindow *ui_surface );
    virtual ~QvkCameraController();

    QvkCameraWindow *cameraWindow;
    void getAllDevices();
    cameraSettingsDialog *vkCameraSettingsDialog;
    Ui_cameraSettingsDialog *uiCameraSettingsDialog;


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


private:
    Ui_formMainWindow *ui_formMainWindow;
    QCamera *camera = nullptr;
    QvkCameraWatcher *cameraWatcher;
    QvkVideoSurface *videoSurface;
    QvkSpezialSlider *sliderCameraWindowSize;
    QvkSpezialSlider *sliderCameraWindowZoom;


protected:


signals:
    void signal_setNewImage( QImage );


};

#endif
*/
