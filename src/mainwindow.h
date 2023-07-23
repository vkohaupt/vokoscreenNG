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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QvkWinInfo.h"
#include "QvkCountdown.h"
#include "QvkRegionChoise.h"
#include "QvkCameraController.h"
#include "QvkSettings.h"
#include "QvkHelp.h"
#include "QvkSystray.h"
#include "QvkMagnifierController.h"
#if (QT_VERSION <= QT_VERSION_CHECK(6, 0, 0))
    // Qt5
    #include "QvkPlayer.h"
#else
    // Qt6
#endif

#include "QvkLimitDiskFreeSpace.h"
#include "QvkSpezialSlider.h"
#include "QvkSystrayAlternative.h"
#include "QvkAudioController.h"
#include "QvkShowClick.h"
#include "QvkHalo.h"
#include "QvkGlobalShortcut.h"
#include "QvkLogController.h"

#include "QvkContainerController.h"

#ifdef Q_OS_WIN
  #include "QvkCiscoOpenh264Controller.h"
#endif

#include <QMainWindow>
#include <QFileSystemWatcher>
#include <QSoundEffect>
#include <QShowEvent>

#include <gst/gst.h>

namespace Ui {
class QvkMainWindow;
}

class QvkMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QvkMainWindow( QWidget *parent = Q_NULLPTR );
    ~QvkMainWindow();
    Ui::formMainWindow *ui;
    QvkWinInfo *vkWinInfo;
    QvkCountdown *vkCountdown;
    QvkRegionChoise *vkRegionChoise;
    QvkSystray *vkSystray;
    QvkAudioController *vkAudioController;
    QvkSpezialSlider *sliderFrames;
    QvkMagnifierController *vkMagnifierController;
    QvkGlobalShortcut *vkGlobalShortcut;
    QvkLogController *vklogController;
    bool wantRecording = true;
    bool isFlatpak = false;


private:
    QvkShowClick *vkShowClick;

    QvkSpezialSlider *sliderScreencastCountDown;
    QvkSpezialSlider *sliderX264;
    QvkSpezialSlider *sliderOpenh264;
    QvkSpezialSlider *sliderVp8;
    QvkSpezialSlider *sliderLimitOfFreeDiskSpace;
    QvkSpezialSlider *sliderShowInSystrayAlternative;
    QvkSpezialSlider *sliderHour;
    QvkSpezialSlider *sliderMinute;
    QvkSpezialSlider *sliderSecondWaitBeforeRecording;
    QvkSpezialSlider *sliderStopRecordingAfterHouers;
    QvkSpezialSlider *sliderStopRecordingAfterMinutes;
    QvkSpezialSlider *sliderStopRecordingAfterSeconds;
    QvkSpezialSlider *sliderWaitBeforeSnapshot;
    QvkSpezialSlider *sliderGstDebugLevel;

    QvkSettings vkSettings;

    const QString VK_Gstr_Pipe = " ! ";
    QString VK_GStreamer_Version();
    QString VK_getXimagesrc();
    QString VK_getCapsFilter();
    QString VK_getMuxer();
    QString Vk_get_Videocodec_Encoder();
    QString VK_scale();

    QFileSystemWatcher *videoFileSystemWatcher;

    GstElement *pipeline;
    GError *error = Q_NULLPTR;

    QStringList videoFormatsList;

    void VK_set_available_Formats_in_Combox();
    QString VK_get_AudioSystem();
    QStringList VK_getSelectedAudioDevice();
    QStringList VK_getSelectedAudioDeviceName();


    QString get_x_From_Screen();
    QString get_y_From_Screen();
    QString get_width_From_Screen();
    QString get_height_From_Screen();

    void VK_gst_Elements_available();
    QvkContainerController *vkContainerController;

    QTimer *timerStartTimer;
    QTimer *timerStopRecordingAfter = new QTimer( this );

    void vk_setCornerWidget( QTabWidget *tabWidget );

#if (QT_VERSION <= QT_VERSION_CHECK(6, 0, 0))
    // Qt5
    QvkPlayer *vkPlayer;
#else
    // Qt6
#endif
    QvkHalo *vkHalo;

    QvkCameraController *vkCameraController;
    QvkSystrayAlternative *vkSystrayAlternative;

    QvkLimitDiskFreeSpace *vkLimitDiskFreeSpace;

    void checkVideoPath();
    bool isAudioDeviceSelected();
    QString Pipeline_structured_output( QString pipeline );
    QSoundEffect *soundEffect;
    bool cancel = false;
    bool is_videoFolderExists_and_haveWritePermission();
    QPalette appPalette;
    QString lastButtonPressed = "start";

    bool openh264ProfileTest;

    bool gst_appsrc = false;

#ifdef Q_OS_WIN
    QvkCiscoOpenh264Controller *vkCiscoOpenh264Controller;
    QTemporaryDir wasapiTemporaryDir;
#endif


private slots:
    void slot_textToGuiLog( QString value );

    void slot_preStart();
    void slot_Start();
    void slot_preStop();
    void slot_Stop();
    void slot_Pause();
    void slot_Continue();
    void slot_Play();
    void slot_Folder();
    void slot_logFolder();
    void slot_screenCountChanged( QString, QString );
    void slot_screenCountChangedArea( QString stringText, QString stringData );
    void slot_newVideoPath();
    void slot_videoFileSystemWatcherSetButtons();
    void slot_videoFileSystemWatcherSetNewPath();
    void slot_startCounter( bool value );
    void slot_comboBoxScreencastScreenCountdown( bool );

    void slot_videoCodecChanged( QString codec );
    void slot_framesReset();
    void slot_x264Reset();
    void slot_openh264Reset();
    void slot_vp8Reset();

    void slot_GstreamerDebugLevel( bool );
    void slot_GstreamerDebugLevelDialog( bool );
    void slot_GstreamerOpenFolder( bool );

    void slot_StartTimer( bool value );
    void slot_startTime();
    void slot_areaSetResolution( QString value );

    void slot_IfStartAudioCodecWidgetsSetEnabled();
    void slot_IfStopAudioCodecWidgetsSetDisabled();

    void slot_setMaxFPS( int );

    void slot_cancel( bool value );

    void slot_haveAudioDeviceSelected( bool bo );

    void slot_comboBoxOpenh264Profile( QStringList list );

    void slot_disableShowclickHalo( bool bo );

    void slot_currentTextChangedToGIF( QString value);

signals:
    void signal_close();
    void signal_close( bool value );
    void signal_newVideoFilename( QString );
    void signal_resizeEvent( QResizeEvent *event );


protected:
    void closeEvent( QCloseEvent *event );
    void resizeEvent( QResizeEvent *event );


};

#endif // MAINWINDOW_H
