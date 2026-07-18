
#include <QWidget>
#include <QString>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QComboBox>
#include <QList>
#include <QLineEdit>
#include <QRadioButton>
#include <QToolButton>
#include <QPainter>
#include <QPainterPath>
#include <QRectF>

#include <QCamera>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QVideoFrameFormat>
#include <QVideoSink>
#include <QMediaCaptureSession>
#include <QVideoFrame>

#include "global.h"
#include "QvkCameraOneSingle.h"
#include "QvkCameraOneWindow.h"
#include "QvkCameraOneOptions.h"
#include "QvkSpezialSlider.h"

#include "ui_QvkCameraOneSingle.h"
#include "ui_QvkCameraOneOptions.h"

// Varibale device enthält zum Beispiel folgenden Inhalt
// "/dev/video1:::UVC Camera (046d:0809):::added" or removed
QvkCameraOneSingle::QvkCameraOneSingle(QWidget *parent, QString device, Ui_formMainWindow *m_GuiUi) :
    QWidget(parent),
    ui(new Ui::QvkCameraOneSingle)
{
    ui->setupUi(this);
    GuiUi = m_GuiUi;

    connect(ui->checkBoxCameraOneOnOff,
            &QCheckBox::clicked,
            this,
            [=](bool value){
        slot_checkBoxCameraOnOff(value);
    });

    connect(ui->comboBoxCameraOnePixelformat,
            &QComboBox::currentIndexChanged,
            this,
            [=](int index){
        Q_UNUSED(index)
        set_resolution_into_comboBox(device);
        if (ui->checkBoxCameraOneOnOff->isChecked() == true){
            qDebug() << "11111111111111111111111111111111111" << camera->isActive();
            ui->checkBoxCameraOneOnOff->click();
            ui->checkBoxCameraOneOnOff->click();
        }
    });
    set_pixelformat_into_comboBox(device);

    connect(ui->comboBoxCameraOneResolution,
            &QComboBox::currentIndexChanged,
            this,
            [=](){
        if (ui->checkBoxCameraOneOnOff->isChecked() == true){
            ui->checkBoxCameraOneOnOff->click();
            ui->checkBoxCameraOneOnOff->click();
        }
    });

    setObjectName(objectName() + "_" + device.section(":::", 0, 0));
    ui->radioButtonCameraOneSelect->setObjectName(ui->radioButtonCameraOneSelect->objectName() + "_" + device.section(":::", 0, 0));

    QString m_objectName = ui->checkBoxCameraOneOnOff->objectName() + "_" + device.section(":::", 0, 0);
    ui->checkBoxCameraOneOnOff->setObjectName(m_objectName);
    ui->checkBoxCameraOneOnOff->setText(device.section(":::", 1, 1));

    m_objectName = ui->comboBoxCameraOnePixelformat->objectName() + "_" + device.section(":::", 0, 0);
    ui->comboBoxCameraOnePixelformat->setObjectName(m_objectName);

    m_objectName = ui->comboBoxCameraOneResolution->objectName() + "_" + device.section(":::", 0, 0);
    ui->comboBoxCameraOneResolution->setObjectName(m_objectName);

    vkCameraOneOptions = new QvkCameraOneOptions(this);
    {
        m_objectName = vkCameraOneOptions->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->setObjectName(m_objectName);
        vkCameraOneOptions->ui->labelCameraOneOptions->setText(device.section(":::", 1, 1));

        m_objectName = vkCameraOneOptions->ui->toolButtonCameraOneViewRectangle->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->ui->toolButtonCameraOneViewRectangle->setObjectName(m_objectName);

        m_objectName = vkCameraOneOptions->ui->toolButtonCameraOneViewEllipse->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->ui->toolButtonCameraOneViewEllipse->setObjectName(m_objectName);

        m_objectName = vkCameraOneOptions->ui->toolButtonCameraOneViewCircle->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->ui->toolButtonCameraOneViewCircle->setObjectName(m_objectName);

        m_objectName = vkCameraOneOptions->ui->lineEditCameraOneTitel->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->ui->lineEditCameraOneTitel->setObjectName(m_objectName);

        m_objectName = vkCameraOneOptions->ui->toolButtonCameraOneTitelReset->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->ui->toolButtonCameraOneTitelReset->setObjectName(m_objectName);

        m_objectName = vkCameraOneOptions->ui->toolButtonCameraOneFramelessOnOff->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->ui->toolButtonCameraOneFramelessOnOff->setObjectName(m_objectName);

        m_objectName = vkCameraOneOptions->ui->toolButtonCameraOneMirrorVertical->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->ui->toolButtonCameraOneMirrorVertical->setObjectName(m_objectName);

        m_objectName = vkCameraOneOptions->ui->toolButtonCameraOneMirrorHorizontal->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->ui->toolButtonCameraOneMirrorHorizontal->setObjectName(m_objectName);

        QLabel *labelCameraWindowSize = new QLabel;
        vkCameraOneOptions->ui->horizontalLayout_3->addWidget(labelCameraWindowSize);
        labelCameraWindowSize->setObjectName("labelCameraOneWindowSize_" + device.section(":::", 0, 0));
        labelCameraWindowSize->setText("---x---");

        QvkSpezialSlider *sliderCameraOneWindowSize = new QvkSpezialSlider(Qt::Horizontal);
        vkCameraOneOptions->ui->horizontalLayout_2->insertWidget(0, sliderCameraOneWindowSize);
        sliderCameraOneWindowSize->setObjectName("sliderCameraOneWindowSize_" + device.section(":::", 0, 0));
        sliderCameraOneWindowSize->setMinimum(0);
        sliderCameraOneWindowSize->setMaximum(1);
        sliderCameraOneWindowSize->setValue(0);
        sliderCameraOneWindowSize->show();
        sliderCameraOneWindowSize->setShowValue(false);
        sliderCameraOneWindowSize->setBigHandel(true);
        sliderCameraOneWindowSize->setEnabled(true);

        QvkSpezialSlider *sliderCameraOneWindowZoom = new QvkSpezialSlider(Qt::Horizontal);
        vkCameraOneOptions->ui->horizontalLayout_5->insertWidget(0, sliderCameraOneWindowZoom);
        sliderCameraOneWindowZoom->setObjectName("sliderCameraOneWindowZoom_" + device.section(":::", 0, 0));;
        sliderCameraOneWindowZoom->setMinimum(0);
        sliderCameraOneWindowZoom->setMaximum(1);
        sliderCameraOneWindowZoom->setValue(0);
        sliderCameraOneWindowZoom->show();
        sliderCameraOneWindowZoom->setShowValue(true);
        sliderCameraOneWindowZoom->setEnabled(true);
    }
    GuiUi->verticalLayout_39->addWidget(vkCameraOneOptions);

    // Window ON/Off
    connect(ui->checkBoxCameraOneOnOff, &QCheckBox::clicked, this, [=](bool value){
        if (value == true){
            if (vkCameraOneWindow == NULL){
                vkCameraOneWindow = new QvkCameraOneWindow(this, device.section(":::", 0, 0));
                vkCameraOneWindow->show();
                vkCameraOneWindow->move(cameraOneWindow_X, cameraOneWindow_Y);
            }
        }else{
            if (vkCameraOneWindow != NULL){
                cameraOneWindow_X = vkCameraOneWindow->get_camera_window_x();
                cameraOneWindow_Y = vkCameraOneWindow->get_camera_window_y();
                // Kein deletelater verwenden da dadurch das Fenster erst in der Hauptschleife zerstört wird
                // wenn checkBox_Frame_OnOff getätigt wird
                delete vkCameraOneWindow;
                vkCameraOneWindow = NULL;
            }
        }
    });

    // Frame On/Off
    connect(vkCameraOneOptions->ui->toolButtonCameraOneFramelessOnOff,
            &QToolButton::clicked,
            this,
            [=](){
        if(ui->checkBoxCameraOneOnOff->isChecked() == true){
            if (vkCameraOneWindow != NULL){
                ui->checkBoxCameraOneOnOff->click(); // Erster Klick zum entfernen des Fensters
                ui->checkBoxCameraOneOnOff->click(); // Zweiter Klick zum anzeigen des Fensters
                vkCameraOneWindow->move(cameraOneWindow_X, cameraOneWindow_Y);
            }
        }
    });

    // Options On/Off
    connect(ui->radioButtonCameraOneSelect,
            &QRadioButton::toggled,
            this,
            [=](bool checked){
        if (checked == true){
            // device ist z.b /dev/video1
            // Unter Windows hat das Device viele "_" deswegen die -1
            QString device = vkCameraOneOptions->objectName().section("_", 1, -1);
            QList<QvkCameraOneOptions *> listOptions = topLevelWidget()->findChildren<QvkCameraOneOptions *>();
            for (int i = 0; i < listOptions.count(); i++){
                if (listOptions.at(i)->objectName().section("_", 1, -1) == device){
                    listOptions.at(i)->show();
                }else{
                    listOptions.at(i)->hide();
                }
            }
        }
    });

    connect(vkCameraOneOptions->ui->lineEditCameraOneTitel,
            &QLineEdit::textChanged,
            this,
            [=](){
        if (vkCameraOneWindow != NULL){
            vkCameraOneWindow->setWindowTitle(vkCameraOneOptions->ui->lineEditCameraOneTitel->text());
        }
    });

    connect(vkCameraOneOptions->ui->toolButtonCameraOneTitelReset, &QToolButton::clicked, this, [=](){
        vkCameraOneOptions->ui->lineEditCameraOneTitel->setText(global::name + "  " + global::version);
    });
    vkCameraOneOptions->ui->toolButtonCameraOneTitelReset->click();
}

QvkCameraOneSingle::~QvkCameraOneSingle()
{
    delete ui;
}


// Varibale device enthält zum Beispiel folgenden Inhalt
// "/dev/video1:::UVC Camera (046d:0809):::added" or removed
void QvkCameraOneSingle::set_pixelformat_into_comboBox(QString device)
{
    QCameraDevice cameraDevice;
    QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    for (int i = 0; i < cameras.count(); i++){
        if (cameras.at(i).id() == device.section(":::", 0, 0)){
            cameraDevice = cameras.at(i);
            break;
        }
    }

    for (int i = 0; i < cameraDevice.videoFormats().count(); i++){
        QString pixelFormat = QVideoFrameFormat::pixelFormatToString(
                    cameraDevice.videoFormats().at(i).pixelFormat()).toUpper();

        if(ui->comboBoxCameraOnePixelformat->findText(pixelFormat) == -1){
            // bei der Philips wird der Wert 17 für YUYV eingetragen und für JPEG der Wert 29
            ui->comboBoxCameraOnePixelformat->addItem(pixelFormat, cameraDevice.videoFormats().at(i).pixelFormat());
        }
    }
}

void QvkCameraOneSingle::set_resolution_into_comboBox(QString device)
{
    ui->comboBoxCameraOneResolution->blockSignals(true);

    QCameraDevice cameraDevice;
    QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    for (int i = 0; i < cameras.count(); i++){
        if (cameras.at(i).id() == device.section(":::", 0, 0)){
            cameraDevice = cameras.at(i);
            break;
        }
    }

    ui->comboBoxCameraOneResolution->clear();
    const QList<QCameraFormat> cameraFormatList = cameraDevice.videoFormats();
    for (int i = 0; i < cameraFormatList.count(); i++){
        if (cameraFormatList.at(i).pixelFormat() == ui->comboBoxCameraOnePixelformat->currentData()){
            QString resolution;
            resolution = QVideoFrameFormat::pixelFormatToString(
                        cameraDevice.videoFormats().at(i).pixelFormat()).toUpper();
            resolution.append(" ");
            resolution.append(QString::number(cameraFormatList.at(i).resolution().width()));
            resolution.append("x");
            resolution.append(QString::number(cameraFormatList.at(i).resolution().height()));
            if (ui->comboBoxCameraOneResolution->findText(resolution) == -1){
                ui->comboBoxCameraOneResolution->addItem(resolution, cameraFormatList.at(i).resolution());
            }
        }
    }

    ui->comboBoxCameraOneResolution->blockSignals(false);
}


void QvkCameraOneSingle::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    {
        // In der Gui die ComboBoxen für die Auflösung auf eine gemeinsame Breite setzen
        QList<QComboBox *> listComboBox;
        QList<QComboBox *> listComboBoxAll = topLevelWidget()->findChildren<QComboBox *>();
        for(int i = 0; i < listComboBoxAll.count(); i++){
            QComboBox *comboBoxResolution = listComboBoxAll.at(i);
            if(comboBoxResolution->objectName().contains("comboBoxCameraOneResolution_")){
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
        QList<QComboBox *> listComboBoxAll = topLevelWidget()->findChildren<QComboBox *>();
        for(int i = 0; i < listComboBoxAll.count(); i++){
            QComboBox *comboBoxResolution = listComboBoxAll.at(i);
            if(comboBoxResolution->objectName().contains("comboBoxCameraOnePixelformat_")){
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


void QvkCameraOneSingle::slot_checkBoxCameraOnOff(bool value)
{
    QCameraDevice cameraDevice;
    QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    for (int i = 0; i < cameras.count(); i++){
        if (cameras.at(i).id() == ui->checkBoxCameraOneOnOff->objectName().section("_", 1, -1)){
            cameraDevice = cameras.at(i);
            break;
        }
    }

    // Camera starten
    if (value == true){
        camera = new QCamera(cameraDevice);
        //connect( camera, SIGNAL( errorChanged() ), this, SLOT( slot_cameraError() ) );

        // Format und Resolution von Widget ermitteln und anwenden
        const QList<QCameraFormat> cameraFormatList = cameraDevice.videoFormats();
        for (int i = 0; i < cameraFormatList.count(); i++){
            if (cameraFormatList.at(i).pixelFormat() == ui->comboBoxCameraOnePixelformat->currentData()){
                if (cameraFormatList.at(i).resolution() == ui->comboBoxCameraOneResolution->currentData()){
                    camera->setCameraFormat(cameraFormatList.at(i));
                    QString width  = QString::number(cameraFormatList.at(i).resolution().width());
                    QString height = QString::number(cameraFormatList.at(i).resolution().height());
                    qDebug().noquote() << global::nameOutput
                                       << "[Camera] Start with format:"
                                       << cameraFormatList.at(i).pixelFormat()
                                       << "and resolution:"
                                       << width + "x" + height;
                }
            }
        }

        videoSink = new QVideoSink;
        connect(videoSink,
                &QVideoSink::videoFrameChanged,
                this,
                [=](QVideoFrame videoFrame){
            slot_videoFrameChanged(videoFrame);
        });

        captureSession = new QMediaCaptureSession;
        captureSession->setCamera( camera );
        captureSession->setVideoOutput( videoSink );

        camera->start();
    }

    // Camera stopen
    if ( value == false ) {
        disconnect( videoSink );
        delete videoSink;

        camera->stop();
        delete camera;
        camera = Q_NULLPTR;

        delete captureSession;

        vkCameraOneWindow->hide();
        qDebug().noquote() << global::nameOutput << "[Camera] Stop";
    }
}


void QvkCameraOneSingle::slot_videoFrameChanged(QVideoFrame videoFrame)
{
    QImage image = videoFrame.toImage();
qDebug() << "11111 QvkCameraOneSingle::slot_videoFrameChanged";
    // Bild muß hier und in QvkCameraOneWindow verworfen werden wenn invalid
    if (image.format() == QImage::Format_Invalid){
        return;
    }
qDebug() << "22222 QvkCameraOneSingle::slot_videoFrameChanged";

    // Rectangle
    if (vkCameraOneOptions->ui->toolButtonCameraOneViewRectangle->isChecked() == true){
    }
    // Rectangle end

    // Ellipse
    if (vkCameraOneOptions->ui->toolButtonCameraOneViewEllipse->isChecked() == true){
        QPixmap pixmap(image.width(), image.height());
        pixmap.fill(Qt::transparent);
        QPainter painter;
        {
            painter.begin(&pixmap);
            painter.setRenderHints(QPainter::Antialiasing, true);
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
            QPainterPath path;
            path.addEllipse(0, 0, image.width(), image.height());
            painter.setClipPath(path);
            painter.drawImage(QPoint(0, 0), image);
        }
        painter.end();
        image = pixmap.toImage();
    }
    // Ellipse end

    // Circle
    if (vkCameraOneOptions->ui->toolButtonCameraOneViewCircle->isChecked() == true){
        int w = image.width();
        int h = image.height();

        QPixmap pixmap(w, h);
        pixmap.fill(Qt::transparent);

        QPainter painter;
        painter.begin(&pixmap);
        {
            painter.setRenderHints(QPainter::Antialiasing, true);
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
            QPainterPath path;
            path.addEllipse((w-h)/2, 0, h, h);
            painter.setClipPath(path);
            painter.drawImage(QPoint(0, 0), image);
        }
        painter.end();
        image = pixmap.toImage();
    }
    // Circle end

    // Mirror vertical
    if (vkCameraOneOptions->ui->toolButtonCameraOneMirrorVertical->isChecked() == true){
        image = image.flipped(Qt::Vertical);
    }

    // Mirror horizontal
    if (vkCameraOneOptions->ui->toolButtonCameraOneMirrorHorizontal->isChecked() == true ) {
        image = image.flipped(Qt::Horizontal);
    }

    vkCameraOneWindow->set_newImage(image);
}
