#ifndef MAINWINDOW_WL_H
#define MAINWINDOW_WL_H

#include "ui_formMainWindow_wl.h"
#include "portal_wl.h"
#include "QvkSpezialSlider.h"

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
    GstElement *vk_gstElement = nullptr;
    QStringList videoFormatsList;
    QvkSpezialSlider *sliderFrames;

    void check_all_Elements_available();
    void check_all_Formats_available();

    void set_supported_Formats_And_Codecs();
    void set_available_Formats_in_ComboBox();
    void set_SpezialSlider();
    void set_Connects();

    QString get_Videocodec_Encoder();
    void messageBox( QString value );


public Q_SLOTS:


private Q_SLOTS:
    void slot_start();
    void slot_stop();
    void slot_start_gst( QString vk_fd, QString vk_path );
    void slot_framesReset();
    void slot_textToGuiLog( QString value );
    void slot_set_available_VideoCodecs_in_ComboBox( QString suffix );

};

#endif 
