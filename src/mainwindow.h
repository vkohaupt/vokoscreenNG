/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
 * 
 * Author:
 *      Volker Kohaupt <vkohaupt@freenet.de>
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
#include "QvkPlayer.h"
#include "QvkTheme.h"
#include "QvkLimitDiskFreeSpace.h"
#include "QvkSpezialSlider.h"
#include "QvkSystrayAlternative.h"
#include "QvkAudioController.h"
#include <QMainWindow>
#include <QFileSystemWatcher>
#include <QSoundEffect>

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
    QvkHelp *vkHelp;
    QvkSystray *vkSystray;
    QvkAudioController *vkAudioController;

private:
    QvkSpezialSlider *sliderScreencastCountDown;
    QvkSpezialSlider *sliderFrames;
    QvkSpezialSlider *sliderX264;
    QvkSpezialSlider *sliderOpenh264;
    QvkSpezialSlider *sliderLimitOfFreeDiskSpace;
    QvkSpezialSlider *sliderShowInSystrayAlternative;
    QvkSpezialSlider *sliderHour;
    QvkSpezialSlider *sliderMinute;
    QvkSpezialSlider *sliderSecondWaitBeforeRecording;
    QvkSpezialSlider *sliderStopRecordingAfterHouers;
    QvkSpezialSlider *sliderStopRecordingAfterMinutes;
    QvkSpezialSlider *sliderStopRecordingAfterSeconds;

    QvkSettings vkSettings;
    QStringList resolutionStringList;

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
    QStringList globalFormatsList;

    void VK_Supported_Formats_And_Codecs();
    void VK_Check_is_Format_available();
    void VK_set_available_Formats_in_Combox();
    QString VK_get_AudioSystem();
    QStringList VK_getSelectedAudioDevice();


    QString get_x_From_Screen();
    QString get_y_From_Screen();
    QString get_width_From_Screen();
    QString get_height_From_Screen();

    void VK_gst_formatVideoAudoicodec_available();
    void VK_gst_Elements_available();

    QTimer *timerStartTimer;
    QTimer *timerStopRecordingAfter = new QTimer( this );

    void vk_setCornerWidget( QTabWidget *tabWidget );

    QvkPlayer *vkPlayer;

    QvkTheme *vkTheme;

    QvkCameraController *vkCameraController;
    QvkSystrayAlternative *vkSystrayAlternative;

    QvkLimitDiskFreeSpace *vkLimitDiskFreeSpace;
    bool wantRecording = true;

    void checkVideoPath();

    bool isAudioDeviceSelected();

    QString Pipeline_structured_output( QString pipeline );

    bool onlyOnce = false;

    QSoundEffect *soundEffect;

private slots:
    void slot_preStart();
    void slot_Start();
    void slot_preStop();
    void slot_Stop();
    void slot_Pause();
    void slot_Continue();
    void slot_Play();
    void slot_Folder();
    void slot_screenCountChanged( QString, QString );
    void slot_screenCountChangedArea( QString stringText, QString stringData );
    void slot_audioIconOnOff(bool state );
    void slot_audioRedCross();
    void slot_newVideoPath();
    void slot_videoFileSystemWatcherSetButtons();
    void slot_videoFileSystemWatcherSetNewPath();
    void slot_startCounter( bool value );
    void slot_comboBoxScreencastScreenCountdown( bool );

    void slot_set_available_VideoCodecs_in_Combox( QString suffix );
    void slot_set_available_AudioCodecs_in_Combox( QString suffix );
    void slot_videoCodecChanged( QString codec );
    void slot_framesReset();
    void slot_x264Reset();
    void slot_openh264Reset();

    void slot_StartTimer( bool value );
    void slot_startTime();
    void slot_areaSetResolution( QString value );
    void slot_areaReset();
    void slot_disableAreaWidgets();
    void slot_enableAreaWidgets();

    void slot_IfStartAudioCodecWidgetsSetEnabled();
    void slot_IfStopAudioCodecWidgetsSetDisabled();

    void slot_setVisibleSystray( bool value );

    void slot_sendReport();

    void slot_vokoPlayer();

#ifdef Q_OS_LINUX
    void slot_afterWindowShown();
#endif


signals:
    void signal_close();
    void signal_close( bool value );
    void signal_newVideoFilename( QString );
    void signal_resizeEvent( QResizeEvent *event );


protected:
    void closeEvent( QCloseEvent *event );
    void resizeEvent( QResizeEvent *event );
#ifdef Q_OS_LINUX
    void showEvent( QShowEvent *event );
#endif


};

#endif // MAINWINDOW_H
