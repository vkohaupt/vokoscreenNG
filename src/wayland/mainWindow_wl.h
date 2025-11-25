#ifndef MAINWINDOW_WL_H
#define MAINWINDOW_WL_H

#include "ui_formMainWindow_wl.h"
#include "portal_wl.h"
#include "QvkSettings_wl.h"
#include "QvkContainerController_wl.h"
#include "QvkRegionChoise_wl.h"
#include "QvkCountdown_wl.h"
#include "QvkHelp_wl.h"
#include "QvkSystray_wl.h"
#include "QvkRegionMargins_wl.h"

#include "QvkSpezialSlider.h"
#include "QvkLogController.h"

#include <QMainWindow>
#include <QWidget>
#include <QTranslator>
#include <QStringList>
#include <QFileSystemWatcher>
#include <QCloseEvent>

#include <gst/gst.h>

namespace Ui
{
class QvkMainWindow_wl;
}


class QvkMainWindow_wl : public QMainWindow
{
    Q_OBJECT
public:
    QvkMainWindow_wl( QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags() );
    ~QvkMainWindow_wl();
    Ui::formMainWindow_wl *ui;
    Portal_wl *portal_wl = new Portal_wl();
    QvkSpezialSlider *sliderFrames;
    QvkSpezialSlider *sliderScreencastCountDown;
    QvkSpezialSlider *sliderSecondWaitBeforeRecording;
    QvkSpezialSlider *sliderOpenh264;
    QvkSpezialSlider *sliderWaitBeforeSnapshot;
    QvkHelp_wl *vkHelp;
    QvkSystray_wl *vkSystray;


private:
    QvkSettings_wl vkSettings_wl;
    QTranslator translator;
    QTranslator qtTranslator;
    QvkRegionChoise_wl *vkRegionChoise_wl;
    QvkLogController *vkLogController;
    GstElement *pipeline = nullptr;
    QStringList videoFormatsList;
    QFileSystemWatcher *videoFileSystemWatcher;
    QvkContainerController_wl *vkContainerController_wl;
    QStringList VK_getSelectedAudioDevice();
    QString Pipeline_structured_output( QString pipeline );
    QvkRegionMargins_wl *vkRegionMargins_wl;

    void set_WindowTitle();
    void set_CornerWidget();
    void set_SpezialSliders();
    void set_Connects();
    void set_check_screencast_elements_available();
    void set_check_GIF_convert_elements_available();
    void set_check_MP4_convert_elements_available();
    void set_check_WEBM_convert_elements_available();

    void set_RegionChoice();

    void messageBox( QString value );

    QString get_Area_Videocrop();
    void get_system_info();
    QString get_KDE_Version();
    QString get_pipewire_version();

    bool isFlatpak = false;

    QString path_to_snapshot_folder;

    static GstBusSyncReply call_bus_message( GstBus *bus, GstMessage *message, gpointer user_data );

    // Snapshot
    void supportedImageFormats();
    // Snapshot


public Q_SLOTS:


private Q_SLOTS:
    void slot_portal_start();
    void slot_portal_dialog_aborted();
    void slot_pre_start( QString vk_fd, QString vk_path );
    void slot_start_gst( QString vk_fd, QString vk_path );
    void slot_stop();
    void slot_frames_Reset();
    void slot_openh264Reset();
    void slot_newVideoPath();
    void slot_videoFileSystemWatcherSetNewPath();
    void slot_log_folder();
    void slot_log_refresh();
    void slot_folder();
    void slot_Pause();
    void slot_Continue();

    // Snapshot
    void slot_snapshotHideBeforeRecording( bool bo );
    void slot_pushButton_snapshot();
    void slot_handle_response_snapshot( uint responseCode, QVariantMap results );
    void slot_path_to_snapshot_folder( bool bo );
    // Snapshot

    void slot_area_margins_start();


protected:
    void closeEvent( QCloseEvent *event );


signals:
    void signal_newVideoFilename( QString );
    void signal_beginRecordTime( QString );

};

#endif 
