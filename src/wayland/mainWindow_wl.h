#ifndef MAINWINDOW_WL_H
#define MAINWINDOW_WL_H

#include "QvkSpezialSlider.h"
#include "QvkLicenses.h"

#include "ui_formMainWindow_wl.h"
#include "portal_wl.h"
#include "QvkSettings_wl.h"
#include "QvkContainerController_wl.h"
#include "QvkRegionChoise_wl.h"
#include "QvkCountdown_wl.h"
#include "QvkSystray_wl.h"
#include "QvkRegionMargins_wl.h"
#include "QvkLogController_wl.h"
#include "QvkInformation_wl.h"
#include "QvkImageFromTabs_wl.h"
#include "QvkCameraController_wl.h"
#include "QvkAudioPipewireController_wl.h"
#include "QvkHelpBrowser_wl.h"


#include <QMainWindow>
#include <QWidget>
#include <QTranslator>
#include <QStringList>
#include <QFileSystemWatcher>
#include <QVariantMap>
#include <QCloseEvent>
#include <QEvent>

#include <gst/gst.h>

namespace Ui
{
class QvkMainWindow_wl;
}


class QvkMainWindow_wl : public QMainWindow
{
    Q_OBJECT
public:
    QvkMainWindow_wl( QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags() );
    ~QvkMainWindow_wl();
    Ui::formMainWindow_wl *ui;
    Portal_wl *portal_wl = new Portal_wl();
    QvkSpezialSlider *sliderFrames;
    QvkSpezialSlider *sliderScreencastCountDown;
    QvkSpezialSlider *sliderSecondWaitBeforeRecording;
    QvkSpezialSlider *sliderOpenh264;
    QvkSpezialSlider *sliderWaitBeforeSnapshot;
    QvkSystray_wl *vkSystray;
    QvkCameraController_wl *vkCameraController_wl = nullptr;
    QvkHelpBrowser_wl *vkHelpBrowser_wl;

private:
    QvkSettings_wl vkSettings_wl;
    QTranslator translator;
    QTranslator qtTranslator;
    QvkRegionChoise_wl *vkRegionChoise_wl;
    QvkLogController_wl *vkLogController_wl;
    GstElement *pipeline = nullptr;
    QStringList videoFormatsList;
    QFileSystemWatcher *videoFileSystemWatcher;
    QvkContainerController_wl *vkContainerController_wl;
    QStringList VK_getSelectedAudioDevice();
    QString Pipeline_structured_output( QString pipeline );
    QvkRegionMargins_wl *vkRegionMargins_wl;
    QvkInformation_wl *vkInformation;
    QvkAudioPipewireController_wl *vkAudioController;
    QvkLicenses *vkLicenses;
    QvkImageFromTabs_wl *vkImageFromTabs_wl;

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

    QString whatWasClicked;

    QString muxerVideoFilename = "";
    static gboolean set_pipeline_null_idle(gpointer data);
    static GstBusSyncReply call_bus_message_convert_mp4(GstBus *bus, GstMessage *message, gpointer data);
    static bool is_FileOpenByAnyProcess(QString targetFilePath);
    GstElement *pipelineMP4;

    QString getPipelineStructure(GstElement *pipeline);


public slots:


private slots:
    void slot_pre_portal_start();
    void slot_portal_start();
    void slot_portal_dialog_aborted();
    void slot_pre_start( QString vk_fd, QString vk_path );
    void slot_start_gst( QString vk_fd, QString vk_path );
    void slot_stop();
    void slot_newVideoPath();
    void slot_videoFileSystemWatcherSetNewPath();
    void slot_log_folder();
    void slot_log_refresh();
    void slot_folder();
    void slot_Pause();
    void slot_Continue();

    // Snapshot
    void slot_snapshotHideBeforeRecording();
    void slot_pushButton_snapshot();
    void slot_handle_response_snapshot( uint responseCode, QVariantMap results );
    void slot_path_to_snapshot_folder();
    // Snapshot

    void slot_languageChanged(int);

    void slot_remux_mkv_to_mp4(QString filePath);


protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);


signals:
    void signal_newVideoFilename(QString filename);
    void signal_beginRecordTime(QString time);
    void signal_gst_eos(QString msg);
    void signal_gstPipelineFinished();


};

#endif 
