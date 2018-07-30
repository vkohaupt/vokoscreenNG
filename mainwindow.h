#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QvkWinInfo.h"
#include "QvkCountdown.h"
#include "QvkWebcamController.h"
#include "QvkRegionChoise.h"

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


private:
    Ui::MainWindow *ui;
    const QString VK_Gstr_Pipe = " ! ";
    QString VK_GStreamer_Version();
    QString VK_getXimagesrc();
    QString VK_getCapsFilter();
    QString VK_getMuxer();
    QString Vk_get_Videocodec_Encoder();
    void makeAndSetValidIcon( int index );
    void makeValidIconForSideBar( int index );

    QFileSystemWatcher *videoFileSystemWatcher;

    GstElement *pipeline;
    GError *error = NULL;

    QvkWinInfo *vkWinInfo;
    QvkCountdown *vkCountdown;

    QStringList videoFormatsList;
    QStringList globalFormatsList;

    void VK_Supported_Formats_And_Codecs();
    void VK_Check_is_Format_available();
    void VK_set_available_Formats_in_Combox();
    QString VK_get_AudioDevice();
    QString VK_get_AudioSystem();

    QString get_AudioDeviceString( GstDevice * device );
    QStringList get_all_Audio_devices();

    QString get_x_From_Screen();
    QString get_y_From_Screen();
    QString get_width_From_Screen();
    QString get_height_From_Screen();

    void VK_gst_Elements_available();

    QvkWebcamController *webcamController;
    QvkRegionChoise *regionChoise;

    QPixmap pixmap;



private slots:
    void slot_preStart();
    void slot_Start();
    void slot_preStop();
    void slot_Stop();
    void slot_Pause();
    void slot_Continue();
    void slot_Play();
    void slot_screenCountChanged(int newCount );
    void slot_setFramesStandard( bool value );
    void slot_audioIconOnOff(bool state );
    void slot_clearVerticalLayoutAudioDevices(bool value);
    void slot_getPulsesDevices(bool value);
    void slot_getAlsaDevices( bool value );
    void slot_newVideoPath();
    void slot_videoFileSystemWatcherSetButtons();
    void slot_videoFileSystemWatcherSetNewPath();
    void slot_startCounter( bool value );

    void slot_set_available_VideoCodecs_in_Combox( QString suffix );
    void slot_set_available_AudioCodecs_in_Combox( QString suffix );

    void slot_audioHelp();
    void slot_availableHelp();

    void slot_formats_Screenshot();
    void slot_preshot_Screenshot();
    void slot_shot_Screenshot();
    void slot_show_Screenshoot();
    void slot_Screenshot_count_changed( int newCount );


signals:
    void signal_close();
    void signal_close_webcam( bool value );
    void signal_finish_screenshot( bool );


protected:
    void closeEvent( QCloseEvent * event );
    void resizeEvent( QResizeEvent *event );


};

#endif // MAINWINDOW_H
