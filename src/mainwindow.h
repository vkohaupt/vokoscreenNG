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

#ifdef Q_OS_LINUX
#include "QvkAudioPulse.h"
#include "QvkAudioAlsa.h"
#endif
#ifdef Q_OS_WIN
#include "QvkAudioWindows.h"
#endif

#include <QMainWindow>
#include <QFileSystemWatcher>

#include <gst/gst.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow *ui;
    QvkWinInfo *vkWinInfo;
    QvkCountdown *vkCountdown;
    QvkRegionChoise *vkRegionChoise;
#ifdef Q_OS_LINUX
    QvkAudioPulse *vkAudioPulse;
    QvkAudioAlsa *vkAudioAlsa;
#endif
#ifdef Q_OS_WIN
    QvkAudioWindows *vkAudioWindows;
#endif
    QvkHelp *vkHelp;
    QvkSystray *vkSystray;

    void makeAndSetValidIcon(QTabWidget *widget, int index, QIcon icon );
    void makeAndSetValidIconForSideBar( int index, QIcon icon );

private:
    QvkSettings vkSettings;
    QStringList resolutionStringList;

    const QString VK_Gstr_Pipe = " ! ";
    QString VK_GStreamer_Version();
    QString VK_getXimagesrc();
    QString VK_getCapsFilter();
    QString VK_getMuxer();
    QString Vk_get_Videocodec_Encoder();
    QString VK_getVideoScale();

    QFileSystemWatcher *videoFileSystemWatcher;

    GstElement *pipeline;
    GError *error = NULL;

    QStringList videoFormatsList;
    QStringList globalFormatsList;

    void VK_Supported_Formats_And_Codecs();
    void VK_Check_is_Format_available();
    void VK_set_available_Formats_in_Combox();
    QString VK_get_AudioDevice();
    QString VK_get_AudioSystem();
    QStringList VK_getSelectedAudioDevice();


    QString get_x_From_Screen();
    QString get_y_From_Screen();
    QString get_width_From_Screen();
    QString get_height_From_Screen();

    void VK_gst_Elements_available();

    QTimer *timerStartTimer;
    QTimer *timerStopRecordingAfter = new QTimer( this );

    void vk_setCornerWidget( QTabWidget *tabWidget );


private slots:
    void slot_preStart();
    void slot_Start();
    void slot_preStop();
    void slot_Stop();
    void slot_Pause();
    void slot_Continue();
    void slot_Play();
    void slot_Folder();
    void slot_screenCountChanged(int value );
    void slot_setFramesStandard( bool value );
    void slot_audioIconOnOff(bool state );
    void slot_newVideoPath();
    void slot_videoFileSystemWatcherSetButtons();
    void slot_videoFileSystemWatcherSetNewPath();
    void slot_startCounter( bool value );

    void slot_set_available_VideoCodecs_in_Combox( QString suffix );
    void slot_set_available_AudioCodecs_in_Combox( QString suffix );

    void slot_StartTimer( bool value );
    void slot_startTime();
    void slot_setHour( int value );
    void slot_setMinute( int value );
    void slot_areaSetResolution( QString value );
    void slot_areaReset();
    void slot_disableAreaWidgets();
    void slot_enableAreaWidgets();

    void slot_IfStartAudioCodecWidgetsSetEnabled();
    void slot_IfStopAudioCodecWidgetsSetDisabled();

    void slot_setVisibleSystray( bool value );

    void slot_sendReport();


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
