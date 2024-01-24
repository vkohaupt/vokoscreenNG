#ifndef MAINWINDOW_WL_H
#define MAINWINDOW_WL_H

#include "ui_formMainWindow_wl.h"
#include "portal_wl.h"
#include "QvkSettings_wl.h"
#include "QvkContainerController_wl.h"
#include "QvkRegionChoise_wl.h"
#include "QvkCountdown_wl.h"

#include "QvkSpezialSlider.h"
#include "QvkLogController.h"

#include <QMainWindow>
#include <QTranslator>
#include <QFileSystemWatcher>
#include <QScreen>
#include <QThread>
#include <QTime>


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


private:
    QvkSettings_wl vkSettings;
    QTranslator translator;
    QTranslator qtTranslator;
    QvkRegionChoise_wl *vkRegionChoise_wl;
    QvkLogController *vklogController;
    GstElement *pipeline = nullptr;
    GstElement *pipelineCamera = nullptr;
    QStringList videoFormatsList;
    QFileSystemWatcher *videoFileSystemWatcher;
    QvkContainerController_wl *vkContainerController_wl;

    void set_LogController();
    void set_WindowTitle();
    void set_CornerWidget();
    void set_system_info();
    void set_SpezialSliders();
    void set_Connects();
    void set_check_all_Elements_available();
    void set_RegionChoice();

    void messageBox( QString value );

    QString get_Videocodec_Encoder();
    QString get_Area_Videocrop();
    QString get_Muxer();

    bool isFlatpak = false;


public Q_SLOTS:


private Q_SLOTS:
    void slot_portal_start();
    void slot_portal_cancel( uint );
    void slot_pre_start( QString vk_fd, QString vk_path );
    void slot_start_gst( QString vk_fd, QString vk_path );
    void slot_stop();
    void slot_frames_Reset();
    void slot_newVideoPath();
    void slot_videoFileSystemWatcherSetNewPath();
    void slot_logFolder();
    void slot_checkBoxCameraOnOff( bool bo );


protected:
    void closeEvent( QCloseEvent *event );


signals:
    void signal_newVideoFilename( QString );
    void signal_beginRecordTime( QString );


};

#endif 
