#ifndef MAINWINDOW_WL_H
#define MAINWINDOW_WL_H

#include "ui_formMainWindow_wl.h"
#include "portal_wl.h"
#include "QvkSpezialSlider.h"
#include "QvkRegionChoise_wl.h"

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


private:
    Portal_wl *portal_wl = new Portal_wl();
    GstElement *pipeline = nullptr;
    QStringList videoFormatsList;
    QvkSpezialSlider *sliderFrames;
    QvkRegionChoise_wl *vkRegionChoise;

    void messageBox( QString value );

    void check_all_Elements_available();
    void check_all_Formats_available();

    void set_supported_Formats_And_Codecs();
    void set_available_Formats_in_ComboBox();
    void set_SpezialSlider();
    void set_Connects();

    QString get_Videocodec_Encoder();
    QString get_Area_Videocrop();


public Q_SLOTS:


private Q_SLOTS:
    void slot_start();
    void slot_stop();
    void slot_start_gst( QString vk_fd, QString vk_path );
    void slot_frames_Reset();
    void slot_set_available_VideoCodecs_in_ComboBox( QString suffix );
    void slot_screenCountChangedArea( QString stringText, QString stringData );


protected:
    void closeEvent( QCloseEvent *event );


};

#endif 
