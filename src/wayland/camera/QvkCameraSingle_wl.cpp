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
#include <QResizeEvent>


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

    connect(ui->comboBoxCameraPixelformat,
            &QComboBox::currentTextChanged,
            this,
            [=](){set_ResolutionOnComboBox(ui->comboBoxCameraPixelformat->objectName().section("_", 1, 1));}
    );
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


void QvkCameraSingle_wl::set_PixelFormatOnComboBox(QString ID)
{
    // Varibale ID enthält zum Beispiel folgenden Inhalt "/dev/video1"

    // Zuerst die Camera mithilfe der ID suchen diese befindet sich in cameraDevice.id() ...
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    QCameraDevice cameraDevice;
    for(int x = 0; x < cameras.count(); x++){
        cameraDevice = cameras.at(x);
        if(cameraDevice.id() == ID){
            break;
        }
    }

    // und dann die Pixelformate wie zum Beispiel JPEG und YUYV in die Combobox stellen ...
    for(int i = 0; i < cameraDevice.videoFormats().count(); i++){
        QCameraFormat videoFormat = cameraDevice.videoFormats().at(i);
        QString format = QVideoFrameFormat::pixelFormatToString(videoFormat.pixelFormat()).toUpper();
        if(ui->comboBoxCameraPixelformat->findText(format) == -1){
            ui->comboBoxCameraPixelformat->addItem(format, videoFormat.pixelFormat());
        }
    }
}


void QvkCameraSingle_wl::set_ResolutionOnComboBox(QString ID)
{
    // Varibale ID enthält zum Beispiel folgenden Inhalt "/dev/video1"

    // Zuerst die Camera mithilfe der ID suchen diese befindet sich in cameraDevice.id() ...
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    QCameraDevice cameraDevice;
    for(int x = 0; x < cameras.count(); x++){
        cameraDevice = cameras.at(x);
        if(cameraDevice.id() == ID){
            break;
        }
    }

    ui->comboBoxCameraResolution->clear();

    // dann die Auflösungen der Kamera ermitteln und in die ComboBox stellen
    for(int i = 0; i < cameraDevice.videoFormats().count(); i++){
        QCameraFormat cameraFormat = cameraDevice.videoFormats().at(i);
        QString width = QString::number(cameraFormat.resolution().width());
        QString height = QString::number(cameraFormat.resolution().height());
        QString resolution = width + " x " + height;
        QVariant variantData = QVariant::fromValue(cameraFormat);
        cameraFormat = variantData.value<QCameraFormat>();
        if(QVariant::fromValue(ui->comboBoxCameraPixelformat->currentData()) == cameraFormat.pixelFormat()){
            if(ui->comboBoxCameraResolution->findText(resolution) == -1){
                ui->comboBoxCameraResolution->addItem(resolution + " " + ui->comboBoxCameraPixelformat->currentText(), variantData);
            }
        }
    }
}


void QvkCameraSingle_wl::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    {
        // In der Gui die ComboBoxen für die Auflösung auf eine gemeinsame Breite setzen
        QList<QComboBox *> listComboBox;
        QList<QComboBox *> listComboBoxAll = GuiUi->centralwidget->findChildren<QComboBox *>();
        for(int i = 0; i < listComboBoxAll.count(); i++){
            QComboBox *comboBoxResolution = listComboBoxAll.at(i);
            if(comboBoxResolution->objectName().contains("comboBoxCameraResolutionVideoID_")){
                listComboBox.append(comboBoxResolution);
            }
        }
        int width = 0;
        for(int i = 0; i < listComboBox.count(); i++){
            QComboBox *comboBox = listComboBox.at(i);
            if ( comboBox->width() > width ){
                width = comboBox->width();
            }
        }
        for(int i = 0; i < listComboBox.count(); i++){
            QComboBox *comboBox = listComboBox.at(i);
            comboBox->setMinimumWidth(width);
        }
    }

    // In der Gui die ComboBoxen für das Pixelformat auf eine gemeinsame Breite setzen
    {
        QList<QComboBox *> listComboBox;
        QList<QComboBox *> listComboBoxAll = GuiUi->centralwidget->findChildren<QComboBox *>();
        for(int i = 0; i < listComboBoxAll.count(); i++){
            QComboBox *comboBoxResolution = listComboBoxAll.at(i);
            if(comboBoxResolution->objectName().contains("comboBoxCameraPixelformatVideoID_")){
                listComboBox.append(comboBoxResolution);
            }
        }
        int width = 0;
        for(int i = 0; i < listComboBox.count(); i++){
            QComboBox *comboBox = listComboBox.at(i);
            if ( comboBox->width() > width ){
                width = comboBox->width();
            }
        }
        for(int i = 0; i < listComboBox.count(); i++){
            QComboBox *comboBox = listComboBox.at(i);
            comboBox->setMinimumWidth(width);
        }
    }
}
