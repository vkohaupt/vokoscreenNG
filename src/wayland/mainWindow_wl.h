#ifndef MAINWINDOW_WL_H
#define MAINWINDOW_WL_H

#include "ui_formMainWindow_wl.h"
#include "portal_wl.h"
#include "QvkSpezialSlider.h"
#include "QvkRegionChoise_wl.h"
#include "QvkContainerController.h"
#include "QvkLogController.h"

#include <QMainWindow>

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


private:
    QvkLogController *vklogController;
    GstElement *pipeline = nullptr;
    QStringList videoFormatsList;
    QvkSpezialSlider *sliderFrames;
    QvkRegionChoise_wl *vkRegionChoise;
    QWidget *testWidget;

    void set_LogController();
    void set_WindowTitle();
    void set_CornerWidget();
    void set_system_info();
    void set_SpezialSliders();
    void set_Connects();
    void set_check_all_Elements_available();
    QvkContainerController *vkContainerController;
    void set_RegionChoice();
    void set_test_available_geometry();

    void messageBox( QString value );

    QString get_Videocodec_Encoder();
    QString get_Area_Videocrop();


public Q_SLOTS:


private Q_SLOTS:
    void slot_start();
    void slot_stop();
    void slot_start_gst( QString vk_fd, QString vk_path );
    void slot_frames_Reset();
    void slot_portal_cancel( uint );
    void slot_set_panel_values_in_spinboxes();


protected:
    void closeEvent( QCloseEvent *event );

};

#endif 
