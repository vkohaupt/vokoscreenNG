#include "global.h"
#include "QvkCameraSingle_wl.h"
#include "QvkCameraSurface_wl.h"
#include "ui_QvkCameraSingle_wl.h"

#include <QString>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QList>
#include <QMediaDevices>
#include <QCameraDevice>
#include <QCameraFormat>
#include <QVideoSink>
#include <QVideoFrame>
#include <QMediaCaptureSession>
#include <QVariant>
#include <QCamera>

QvkCameraSingle_wl::QvkCameraSingle_wl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QvkCameraSingle_wl)
{
    ui->setupUi(this);
    ui->comboBoxCameraFPS->hide();
}


void QvkCameraSingle_wl::set_objectName(QString device)
{
    setObjectName("cameraSingleVideoID_" + device.section(":::", 0, 0)); // Im ObjectName steckt die id
    ui->checkBoxCameraOnOff->setText(device.section(":::", 1, 1 ));
    ui->checkBoxCameraOnOff->setObjectName("checkBoxCameraVideoID_" + device.section(":::", 0, 0));
    ui->comboBoxCameraPixelformat->setObjectName("comboBoxCameraPixelformatVideoID_" + device.section(":::", 0, 0));
    ui->comboBoxCameraResolution->setObjectName("comboBoxCameraResolutionVideoID_" + device.section(":::", 0, 0));
    ui->comboBoxCameraFPS->setObjectName("comboBoxCameraFPSVideoID_" + device.section(":::", 0, 0));

    connect(ui->checkBoxCameraOnOff, &QCheckBox::clicked, this, [=](bool value){
        slot_checkBoxCameraOnOff(value, ui->checkBoxCameraOnOff);
        emit signal_forSystrayCameraOnOff(ui->checkBoxCameraOnOff);
        emit signal_checkBoxCameraOnOff(value);
    });
}


QvkCameraSingle_wl::~QvkCameraSingle_wl()
{
    delete ui;
}


void QvkCameraSingle_wl::set_GUIui(Ui_formMainWindow_wl *ui)
{
    GuiUi = ui;
}


void QvkCameraSingle_wl::slot_checkBoxCameraOnOff(bool checked, QCheckBox *checkBoxCameraOnOff)
{
    if ( checked == true ){
        vkCameraSurface_wl = new QvkCameraSurface_wl;
        vkCameraSurface_wl->set_GUIui(GuiUi);
        const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
        for ( int x = 0; x < cameras.count(); x++ ){
            QCameraDevice cameraDevice = cameras.at(x);
            if ( cameraDevice.id() == checkBoxCameraOnOff->objectName().section("_", 1, 1) ){
                camera = new QCamera(cameraDevice);
                QVideoSink *videoSink = new QVideoSink;
                connect(videoSink,
                        &QVideoSink::videoFrameChanged,
                        vkCameraSurface_wl,
                        [=](QVideoFrame videoFrame){
                    vkCameraSurface_wl->slot_setCameraImage(videoFrame);
                });
/*
                // ComboxBox für die Formate YUYV JPEG etc. ermitteln ...
                QList<QComboBox *> listComboBoxPixelformat = GuiUi->centralwidget->findChildren<QComboBox *>();
                QComboBox *comboBoxPixelformat = NULL;
                for(int i = 0; i < listComboBoxPixelformat.count(); i++){
                    comboBoxPixelformat = listComboBoxPixelformat.at(i);
                    if(comboBoxPixelformat->objectName() == QString("comboBoxCameraPixelformatVideoID_" + checkBoxCameraOnOff->objectName().section("_", 1, 1))){
                        break;
                    }
                }
*/
                // ComboxBox für die Auflösungen ermitteln ...
                QList<QComboBox *> listComboBoxResolution = GuiUi->centralwidget->findChildren<QComboBox *>();
                QComboBox *comboBoxResolution = NULL;
                for(int i = 0; i < listComboBoxResolution.count(); i++){
                    comboBoxResolution = listComboBoxResolution.at(i);
                    if(comboBoxResolution->objectName() == QString("comboBoxCameraResolutionVideoID_" + checkBoxCameraOnOff->objectName().section("_", 1, 1))){
                        break;
                    }
                }
                // und hier die Auflösung aus der ComboBox lesen und an der Kamera setzen
                QVariant variantData = comboBoxResolution->currentData();
                QCameraFormat cameraFormat = variantData.value<QCameraFormat>();
                camera->setCameraFormat(cameraFormat);

                QMediaCaptureSession *captureSession = new QMediaCaptureSession;
                captureSession->setCamera(camera);
                captureSession->setVideoOutput(videoSink);
                qDebug().noquote() << global::nameOutput
                                   << "[Camera] Start with:"
                                   << cameraDevice.id()
                                   << cameraFormat.pixelFormat()
                                   << cameraFormat.resolution()
                                   << cameraFormat.minFrameRate()
                                   << cameraFormat.maxFrameRate();

                camera->start();
            }
        }
    }

    if ( checked == false ){
        camera->stop();
        qDebug().noquote() << global::nameOutput
                           << "[Camera] Stop:"
                           << checkBoxCameraOnOff->objectName().section("_", 1, 1);
        delete camera;
        camera = NULL;
        delete vkCameraSurface_wl;
        vkCameraSurface_wl = NULL;
    }
}
