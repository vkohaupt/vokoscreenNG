#include "global.h"
#include "QvkCameraSingle_wl.h"
#include "QvkCameraSurface_wl.h"
#include "QvkSettings_wl.h"
#include "QvkSpezialSlider.h"

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
#include <QTimer>
#include <QPainter>

QvkCameraSingle_wl::QvkCameraSingle_wl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QvkCameraSingle_wl)
{
    ui->setupUi(this);
    ui->comboBoxCameraFPS->hide();
}

void QvkCameraSingle_wl::set_init(QString device)
{
    setObjectName("cameraSingleVideoID_" + device.section(":::", 0, 0)); // Im ObjectName steckt die id
    ui->checkBoxCameraOnOff->setText(device.section(":::", 1, 1 ));
    QString html = QString("<img src=:/pictures/camera/camera.png style=max-width:20%; height:auto;> " + device.section(":::", 0, 0 ));
    ui->checkBoxCameraOnOff->setToolTip(html);
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
            [=](){
        if(ui->checkBoxCameraOnOff->isChecked() == true){
            ui->checkBoxCameraOnOff->click();
            set_ResolutionOnComboBox(ui->comboBoxCameraPixelformat->objectName().section("_", 1, 1));
            ui->checkBoxCameraOnOff->click();
        } else {
            set_ResolutionOnComboBox(ui->comboBoxCameraPixelformat->objectName().section("_", 1, 1));
        }
    });

    connect(ui->comboBoxCameraResolution,
            &QComboBox::currentTextChanged,
            this,
            [=](){
        if(ui->checkBoxCameraOnOff->isChecked() == true){
            ui->checkBoxCameraOnOff->click();
            ui->checkBoxCameraOnOff->click();
        }
    });

    connect(this,
            &QvkCameraSingle_wl::signal_cameraImageAvailable,
            this,
            [=](){
        timerNoImage->stop();
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
    if (checked == true){

        QvkSpezialSlider *vkSpezialSliderWindowSize = GuiUi->centralwidget->findChild<QvkSpezialSlider *>("sliderCameraWindowSize");
        if (vkSpezialSliderWindowSize != nullptr){
            vkSpezialSliderWindowSize->setEnabled(true);
            QVariant variantData = ui->comboBoxCameraResolution->currentData();
            QCameraFormat cameraFormat = variantData.value<QCameraFormat>();
            int m_width = cameraFormat.resolution().width();
            vkSpezialSliderWindowSize->setMinimum(m_width / 4);
            vkSpezialSliderWindowSize->setMaximum(m_width);
            if (oldSpezialSliderValue != 0){
                vkSpezialSliderWindowSize->setValue(oldSpezialSliderValue);
            }else{
                if (vkSpezialSliderWindowSize->value() < m_width){
                    vkSpezialSliderWindowSize->setValue(vkSpezialSliderWindowSize->value());
                }else{
                    vkSpezialSliderWindowSize->setValue(m_width);
                }
            }
        }

        QvkSpezialSlider *vkSpezialSliderZoom = GuiUi->centralwidget->findChild<QvkSpezialSlider *>("sliderCameraWindowZoom");
        if (vkSpezialSliderZoom != nullptr){
            int value = ui->comboBoxCameraResolution->currentData().value<QCameraFormat>().resolution().width() / 2;
            vkSpezialSliderZoom->setMaximum(value-1);
        }

        vkCameraSurface_wl = new QvkCameraSurface_wl(checkBoxCameraOnOff->objectName().section("_", 1, 1));
        vkCameraSurface_wl->set_GUIui(GuiUi);

        // ----- Für kein Bild ------
        int width = ui->comboBoxCameraResolution->currentText().section(" ", 0, 0).toInt();
        int height = ui->comboBoxCameraResolution->currentText().section(" ", 2, 2).toInt();
        QImage image(width, height, QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::lightGray);

        QPainter painter(&image);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QImage imagePicture(":/pictures/camera/camera.png");
        imagePicture = imagePicture.scaled(height/2,height/2);
        painter.drawImage(QPoint((width-imagePicture.width())/2, (height-imagePicture.width())/2), imagePicture);

        vkCameraSurface_wl->slot_setCameraImage(image);
        vkCameraSurface_wl->m_newImageRect=false;

        timerNoImage = new QTimer();
        timerNoImage->setTimerType( Qt::PreciseTimer );
        timerNoImage->setInterval( 20 );
        connect(timerNoImage, &QTimer::timeout, this, [=](){
            vkCameraSurface_wl->m_newImageRect=false;
            vkCameraSurface_wl->slot_setCameraImage(image);
        });
        timerNoImage->start();
        // ----- Für kein Bild ------

        const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
        for ( int x = 0; x < cameras.count(); x++ ){
            QCameraDevice cameraDevice = cameras.at(x);
            if ( cameraDevice.id() == checkBoxCameraOnOff->objectName().section("_", 1, 1) ){
                camera = new QCamera(cameraDevice);


                //-------------------------------------------------------------------------------------------
                set_colorTemperature(camera);


                QVideoSink *videoSink = new QVideoSink;
                connect(videoSink,
                        &QVideoSink::videoFrameChanged,
                        vkCameraSurface_wl,
                        [=](QVideoFrame videoFrame){
                    QImage image = videoFrame.toImage();
                    //                    image = image.convertedTo( QImage::Format_ARGB32 );
                    image = image.convertedTo(QImage::Format_ARGB32_Premultiplied);
                    vkCameraSurface_wl->slot_setCameraImage(image);
                    // Bild von Kamera verfügbar.
                    // Timer für Hinweis das kein Bild vorhanden ist kann nun gestoppt werden
                    emit signal_cameraImageAvailable();
                });

                // Auflösung aus der ComboBox lesen und an der Kamera setzen
                QVariant variantData = ui->comboBoxCameraResolution->currentData();
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
        QvkSpezialSlider *vkSpezialSliderWindowSize = GuiUi->centralwidget->findChild<QvkSpezialSlider *>("sliderCameraWindowSize");
        if (vkSpezialSliderWindowSize != nullptr){
            oldSpezialSliderValue = vkSpezialSliderWindowSize->value();
            vkSpezialSliderWindowSize->setEnabled(false);
        }
        // Beim stoppen der Kamera wird das surface in den Settings gespeichert
        int camera_surface_X = vkCameraSurface_wl->imageRect.x();
        int camera_surface_Y = vkCameraSurface_wl->imageRect.y();
        QvkSettings_wl vkSettings_wl;
        vkSettings_wl.saveCameraSurface(camera_surface_X, camera_surface_Y, checkBoxCameraOnOff->objectName().section("_", 1, 1));

        timerNoImage->stop();

        camera->stop();
        delete camera;
        camera = nullptr;
        qDebug().noquote() << global::nameOutput
                           << "[Camera] Stop:"
                           << checkBoxCameraOnOff->objectName().section("_", 1, 1);

        delete vkCameraSurface_wl;
        vkCameraSurface_wl = nullptr;
    }
}

void QvkCameraSingle_wl::set_colorTemperature(QCamera *camera)
{
        // Farbtemperatur ermitteln
        connect(camera, &QCamera::activeChanged, this, [=](bool active){
            QvkSpezialSlider  *vkSpezialSliderColorTemperature = GuiUi->centralwidget->findChild<QvkSpezialSlider *>("sliderCameraColorTemperature");
            if (active == true){
                qDebug().noquote() << global::nameOutput << "[Camera] is active";

                //QCamera::WhiteBalanceManual muß true sein damit die Temperatur ausgelesen und gesetzt werden kann
                bool m_modeSupported = camera->isWhiteBalanceModeSupported(QCamera::WhiteBalanceManual);
                qDebug().noquote() << global::nameOutput << "[Camera] WhiteBalanceModeSupported:" << m_modeSupported;
                if (m_modeSupported != true){
                    return;
                }else{
                    GuiUi->comboBoxCameraColorTemperature->setEnabled(true);
                    int index = GuiUi->comboBoxCameraColorTemperature->findText("Manual");
                    if (GuiUi->comboBoxCameraColorTemperature->currentIndex() == index){
                        vkSpezialSliderColorTemperature->setEnabled(true);
                    }else{
                        vkSpezialSliderColorTemperature->setEnabled(false);
                    }
                };

                int min_ColorTemperatur = 1;
                int max_ColorTemperatur = 0;

                // colorTemperatur auf sehr hohen Wert setzen....
                camera->setWhiteBalanceMode(QCamera::WhiteBalanceManual);
                camera->setColorTemperature(50000);
                // ... nun den maximalen Wert abfragen
                max_ColorTemperatur = camera->colorTemperature() / 100;
                qDebug().noquote() << global::nameOutput << "[Camera] max. color temperature:" << camera->colorTemperature();

                camera->setWhiteBalanceMode(QCamera::WhiteBalanceManual);
                camera->setColorTemperature(0); // resetten
                qDebug().noquote() << global::nameOutput << "[Camera] Reset temperature. Automatic value:" << camera->colorTemperature();

                // Nun die Werte am Schieberegler setzen
//                QvkSpezialSlider  *vkSpezialSliderColorTemperature = GuiUi->centralwidget->findChild<QvkSpezialSlider *>("sliderCameraColorTemperature");
                vkSpezialSliderColorTemperature->setMinimum(min_ColorTemperatur);
                vkSpezialSliderColorTemperature->setMaximum(max_ColorTemperatur);
//                QvkSpezialSlider *slider = vkCameraOneOptions->sliderCameraOneColorTemperature;
                sliderTemperatureConnect = connect(vkSpezialSliderColorTemperature, &QvkSpezialSlider::valueChanged, this, [=](int value){
                    int color = value * 100;
                    camera->setWhiteBalanceMode(QCamera::WhiteBalanceManual);
                    camera->setColorTemperature(color);
                    vkSpezialSliderColorTemperature->setToolTip(QString::number(color) + " Kelvin");
                    qDebug().noquote() << global::nameOutput << "[Camera] Color valueChanged:" << color;
                });

                comboBoxTemperatureConnect = connect(GuiUi->comboBoxCameraColorTemperature,
                                                     &QComboBox::currentIndexChanged,
                                                     this,
                                                     [=](int index){
                    if (index == 0){
                        vkSpezialSliderColorTemperature->setEnabled(false);
                        camera->setWhiteBalanceMode(QCamera::WhiteBalanceManual);
                        camera->setColorTemperature(0); // resetten
                        qDebug().noquote() << global::nameOutput << "[Camera] Set ColorTemperature Automatic";
                    }else{
                        vkSpezialSliderColorTemperature->setEnabled(true);
                        camera->setWhiteBalanceMode(QCamera::WhiteBalanceManual);
                        int color = vkSpezialSliderColorTemperature->value() * 100;
                        camera->setColorTemperature(color);
                        vkSpezialSliderColorTemperature->setToolTip(QString::number(color) + " Kelvin");
                        qDebug().noquote() << global::nameOutput << "[Camera] Set ColorTemperature Manual:" << color;
                    }
                });

                // Colortemperatur unabhängig von einem ComboBox Signal setzen
                int index = GuiUi->comboBoxCameraColorTemperature->findText("Manual");
                int currentIndex = GuiUi->comboBoxCameraColorTemperature->currentIndex();
                if (currentIndex == index){
                    camera->setWhiteBalanceMode(QCamera::WhiteBalanceManual);
                    int color = vkSpezialSliderColorTemperature->value() * 100;
                    camera->setColorTemperature(color);
                    vkSpezialSliderColorTemperature->setToolTip(QString::number(color) + " Kelvin");
                    qDebug().noquote() << global::nameOutput << "[Camera] Set ColorTemperature Manual:" << color;
                }else{
                    camera->setWhiteBalanceMode(QCamera::WhiteBalanceManual);
                    camera->setColorTemperature(0);
                    qDebug().noquote() << global::nameOutput << "[Camera] Set ColorTemperature Automatic";
                }

            }

            if (active == false){
                GuiUi->comboBoxCameraColorTemperature->setEnabled(false);
                vkSpezialSliderColorTemperature->setEnabled(false);
                QObject::disconnect(comboBoxTemperatureConnect);
                QObject::disconnect(sliderTemperatureConnect);
                qDebug().noquote() << global::nameOutput << "[Camera] not active";
            }
        });
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
