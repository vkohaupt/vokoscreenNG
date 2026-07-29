#ifndef QVKCAMERAONESINGLE_H
#define QVKCAMERAONESINGLE_H

#include <QString>
#include <QWidget>
#include <QCheckBox>
#include <QResizeEvent>
#include <QCamera>
#include <QVideoSink>
#include <QMediaCaptureSession>
#include <QVideoFrame>


#include "QvkCameraOneWindow.h"
#include "QvkCameraOneOptions.h"

#include "ui_QvkCameraOneSingle.h"
#include "ui_formMainWindow.h"

namespace Ui {
class QvkCameraOneSingle;
}

class QvkCameraOneSingle : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCameraOneSingle(QWidget *parent, QString device, Ui_formMainWindow *m_GuiUi);
    ~QvkCameraOneSingle();

    QvkCameraOneWindow *vkCameraOneWindow = nullptr;
    QvkCameraOneOptions *vkCameraOneOptions = nullptr;

    Ui::QvkCameraOneSingle *ui;

    int cameraOneWindow_X = 200;
    int cameraOneWindow_Y = 200;


private:
    Ui_formMainWindow *GuiUi;
    QCamera *camera;
    QVideoSink *videoSink;
    QMediaCaptureSession *captureSession;

    void set_pixelformat_into_comboBox(QString device);
    void set_resolution_into_comboBox(QString device);

    QMetaObject::Connection comboBoxTemperatureConnect;
    QMetaObject::Connection sliderTemperatureConnect;


private slots:
    void slot_checkBoxCameraOnOff(bool value);
    void slot_videoFrameChanged(QVideoFrame videoFrame);
    void slot_slider_min_max();


protected:
    void resizeEvent(QResizeEvent *event);


signals:


};

#endif // QVKCAMERAONESINGLE_H
