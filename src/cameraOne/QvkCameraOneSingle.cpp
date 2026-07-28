
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
    vkCameraOneOptions->set_init(device);
    GuiUi->verticalLayout_39->addWidget(vkCameraOneOptions);

    connect(ui->comboBoxCameraOnePixelformat,
            &QComboBox::currentIndexChanged,
            this,
            [=](int index){
        Q_UNUSED(index)
        ui->comboBoxCameraOneResolution->blockSignals(true);
        set_resolution_into_comboBox(device);
        ui->comboBoxCameraOneResolution->blockSignals(false);
        if (ui->checkBoxCameraOneOnOff->isChecked() == true){
            ui->checkBoxCameraOneOnOff->click();
            ui->checkBoxCameraOneOnOff->click();
        }
    });
    set_pixelformat_into_comboBox(device);

    connect(ui->comboBoxCameraOneResolution,
            &QComboBox::currentIndexChanged,
            this,
            [=](){
        slot_slider_min_max();
        if (ui->checkBoxCameraOneOnOff->isChecked() == true){
            ui->checkBoxCameraOneOnOff->click();
            ui->checkBoxCameraOneOnOff->click();
        }
    });

    // Fenster ON/Off
    connect(ui->checkBoxCameraOneOnOff, &QCheckBox::clicked, this, [=](bool value){
        if (value == true){
            if (vkCameraOneWindow == nullptr){
                vkCameraOneWindow = new QvkCameraOneWindow(this, device.section(":::", 0, 0));
                vkCameraOneWindow->show();
                vkCameraOneWindow->move(cameraOneWindow_X, cameraOneWindow_Y);
            }
        }else{
            if (vkCameraOneWindow != nullptr){
                cameraOneWindow_X = vkCameraOneWindow->get_camera_window_x();
                cameraOneWindow_Y = vkCameraOneWindow->get_camera_window_y();
                // Kein deletelater verwenden da dadurch das Fenster erst in der Hauptschleife zerstört wird
                // wenn checkBox_Frame_OnOff getätigt wird
                delete vkCameraOneWindow;
                vkCameraOneWindow = nullptr;
            }
        }
    });

    // Frame On/Off
    connect(vkCameraOneOptions->ui->toolButtonCameraOneFramelessOnOff,
            &QToolButton::clicked,
            this,
            [=](){
        if(ui->checkBoxCameraOneOnOff->isChecked() == true){
            if (vkCameraOneWindow != nullptr){
                ui->checkBoxCameraOneOnOff->click(); // Erster Klick zum entfernen des Fensters
                ui->checkBoxCameraOneOnOff->click(); // Zweiter Klick zum anzeigen des Fensters
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
            slot_slider_min_max();
        }
    });

    connect(vkCameraOneOptions->ui->lineEditCameraOneTitel,
            &QLineEdit::textChanged,
            this,
            [=](){
        if (vkCameraOneWindow != nullptr){
            vkCameraOneWindow->setWindowTitle(vkCameraOneOptions->ui->lineEditCameraOneTitel->text());
        }
    });

    connect(vkCameraOneOptions->ui->toolButtonCameraOneTitelReset, &QToolButton::clicked, this, [=](){
        vkCameraOneOptions->ui->lineEditCameraOneTitel->setText(global::name + "  " + global::version);
    });
    vkCameraOneOptions->ui->toolButtonCameraOneTitelReset->click();

    // Rectangle: Fenster verschieben so das das Bild "stehen" bleibt
    connect(vkCameraOneOptions->ui->toolButtonCameraOneViewRectangle,
            &QToolButton::clicked,
            this,
            [=](){
        if (vkCameraOneWindow != nullptr){
            int x = vkCameraOneWindow->x();
            int y = vkCameraOneWindow->y();
            int width = vkCameraOneWindow->image.width();
            int height = vkCameraOneWindow->image.height();
            if (vkCameraOneWindow->width() != vkCameraOneWindow->image.width()){
                vkCameraOneWindow->move(x - (width-height)/2, y);
            }
        }
    });

    // Ellipse: Fenster verschieben so das das Bild "stehen" bleibt
    connect(vkCameraOneOptions->ui->toolButtonCameraOneViewEllipse,
            &QToolButton::clicked,
            this,
            [=](){
        if (vkCameraOneWindow != nullptr){
            int x = vkCameraOneWindow->x();
            int y = vkCameraOneWindow->y();
            int width = vkCameraOneWindow->image.width();
            int height = vkCameraOneWindow->image.height();
            if (vkCameraOneWindow->width() != vkCameraOneWindow->image.width()){
                vkCameraOneWindow->move(x - (width-height)/2, y);
            }
        }
    });

    // Circle: Fenster verschieben so das das Bild "stehen" bleibt
    connect(vkCameraOneOptions->ui->toolButtonCameraOneViewCircle,
            &QToolButton::clicked,
            this,
            [=](){
        if (vkCameraOneWindow != nullptr){
            int x = vkCameraOneWindow->x();
            int y = vkCameraOneWindow->y();
            int width = vkCameraOneWindow->width();
            int height = vkCameraOneWindow->height();
            vkCameraOneWindow->move(x + (width-height)/2, y);
        }
    });

    connect(ui->checkBoxCameraOneOnOff,
            &QCheckBox::clicked,
            this,
            [=](bool value){
        slot_checkBoxCameraOnOff(value);
    });
}

QvkCameraOneSingle::~QvkCameraOneSingle()
{
    delete ui;
}


void QvkCameraOneSingle::slot_slider_min_max()
{
    int value = ui->comboBoxCameraOneResolution->currentData().toSize().width() - 100;
    vkCameraOneOptions->sliderCameraOneWindowSize->setMinimum(0);
    vkCameraOneOptions->sliderCameraOneWindowSize->setMaximum(value);

    value = ui->comboBoxCameraOneResolution->currentData().toSize().width() / 2;
    vkCameraOneOptions->sliderCameraOneWindowZoom->setMinimum(0);
    vkCameraOneOptions->sliderCameraOneWindowZoom->setMaximum(value-1);
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

        // Farbtemperatur ermitteln
        connect(camera, &QCamera::activeChanged, this, [=](bool active){
            if (active == true){
                /*
                In Qt ist die Standard-Farbtemperatur typischerweise an den automatischen Weißabgleich (AWB) gekoppelt.
                Sie wird standardmäßig nicht als fester Kelvin-Wert definiert,
                sondern vom Kameratreiber dynamisch anhand der aktuellen Umgebung berechnet.

                Für die direkte manuelle Steuerung (in Kelvin) muss der manuelle Weißabgleich aktiviert werden.
                Hier ist die genaue Funktionsweise der Farbtemperatur in
                Qt (Qt 6):Standardverhalten: Wenn der Weißabgleich auf WhiteBalanceAuto steht,
                wird die Farbtemperatur von der Hardware dynamisch geregelt.

                Manuelle Anpassung: Sie können die Eigenschaft colorTemperature (in Kelvin) nur dann setzen,
                wenn Sie den Weißabgleich explizit auf WhiteBalanceManual
                (oder den entsprechenden C++-Modus) umstellen.

                Details zum automatischen Zurücksetzen: In C++ bewirkt das Setzen der Temperatur auf 0,
                dass das System automatisch wieder in den Modus WhiteBalanceAuto wechselt.
                */
            /*
                qDebug().noquote() << global::nameOutput << "[Camera] is active";

                //QCamera::WhiteBalanceManual muß true sein damit die Temperatur ausgelesen und gesetzt werden kann
                bool m_modeSupported = camera->isWhiteBalanceModeSupported(QCamera::WhiteBalanceManual);
                qDebug().noquote() << global::nameOutput << "[Camera] WhiteBalanceModeSupported:" << m_modeSupported;
                if (m_modeSupported != true){
                    return;
                }else{
                    vkCameraOneOptions->ui->comboBoxCameraOneColorTemperature->setEnabled(true);
                    int index = vkCameraOneOptions->ui->comboBoxCameraOneColorTemperature->findText("Manual");
                    if (vkCameraOneOptions->ui->comboBoxCameraOneColorTemperature->currentIndex() == index){
                        vkCameraOneOptions->sliderCameraOneColorTemperature->setEnabled(true);
                    }else{
                        vkCameraOneOptions->sliderCameraOneColorTemperature->setEnabled(false);
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
                vkCameraOneOptions->sliderCameraOneColorTemperature->setMinimum(min_ColorTemperatur);
                vkCameraOneOptions->sliderCameraOneColorTemperature->setMaximum(max_ColorTemperatur);
                QvkSpezialSlider *slider = vkCameraOneOptions->sliderCameraOneColorTemperature;
                connect(slider, &QvkSpezialSlider::valueChanged, this, [=](int value){
                    int color = value * 100;
                    camera->setWhiteBalanceMode(QCamera::WhiteBalanceManual);
                    camera->setColorTemperature(color);
                    vkCameraOneOptions->sliderCameraOneColorTemperature->setToolTip(QString::number(color) + " Kelvin");
                    qDebug().noquote() << global::nameOutput << "[Camera] Color valueChanged:" << color;
                });

                comboBoxTemperatureConnect = connect(vkCameraOneOptions->ui->comboBoxCameraOneColorTemperature,
                                                     &QComboBox::currentIndexChanged,
                                                     this,
                                                     [=](int index){
                    if (index == 0){
                        vkCameraOneOptions->sliderCameraOneColorTemperature->setEnabled(false);
                        camera->setWhiteBalanceMode(QCamera::WhiteBalanceManual);
                        camera->setColorTemperature(0); // resetten
                        qDebug().noquote() << global::nameOutput << "[Camera] Set ColorTemperature Automatic";
                    }else{
                        vkCameraOneOptions->sliderCameraOneColorTemperature->setEnabled(true);
                        camera->setWhiteBalanceMode(QCamera::WhiteBalanceManual);
                        int color = vkCameraOneOptions->sliderCameraOneColorTemperature->value() * 100;
                        camera->setColorTemperature(color);
                        vkCameraOneOptions->sliderCameraOneColorTemperature->setToolTip(QString::number(color) + " Kelvin");
                        qDebug().noquote() << global::nameOutput << "[Camera] Set ColorTemperature Manual:" << color;
                    }
                });

                // Colortemperatur unabhängig von einem ComboBox Signal setzen
                int index = vkCameraOneOptions->ui->comboBoxCameraOneColorTemperature->findText("Manual");
                int currentIndex = vkCameraOneOptions->ui->comboBoxCameraOneColorTemperature->currentIndex();
                if (currentIndex == index){
                    camera->setWhiteBalanceMode(QCamera::WhiteBalanceManual);
                    int color = vkCameraOneOptions->sliderCameraOneColorTemperature->value() * 100;
                    camera->setColorTemperature(color);
                    vkCameraOneOptions->sliderCameraOneColorTemperature->setToolTip(QString::number(color) + " Kelvin");
                    qDebug().noquote() << global::nameOutput << "[Camera] Set ColorTemperature Manual:" << color;
                }else{
                    camera->setWhiteBalanceMode(QCamera::WhiteBalanceManual);
                    camera->setColorTemperature(0);
                    qDebug().noquote() << global::nameOutput << "[Camera] Set ColorTemperature Automatic";
                }
            */
            }

            if (active == false){
                vkCameraOneOptions->ui->comboBoxCameraOneColorTemperature->setEnabled(false);
                vkCameraOneOptions->sliderCameraOneColorTemperature->setEnabled(false);
                QObject::disconnect(comboBoxTemperatureConnect);
                qDebug().noquote() << global::nameOutput << "[Camera] not active";
            }
        });

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
                    break;
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
        captureSession->setCamera(camera);
        captureSession->setVideoOutput(videoSink);

        camera->start();
        qDebug();
    };

    // Camera stoppen
    if (value == false){
        disconnect(videoSink);
        delete videoSink;

        camera->stop();
        delete camera;
        camera = nullptr;

        delete captureSession;

        qDebug().noquote() << global::nameOutput << "[Camera] Stop";
        qDebug();
    }
}


void QvkCameraOneSingle::slot_videoFrameChanged(QVideoFrame videoFrame)
{
    QImage image = videoFrame.toImage();

    // Bild muß hier und in QvkCameraOneWindow verworfen werden wenn invalid
    if (image.format() == QImage::Format_Invalid){
        return;
    }

    // Zoom
    // Wenn der Wert des Schiebereglers größer Null ist soll skaliert werden
    if (vkCameraOneOptions->sliderCameraOneWindowZoom->value() > 0){
        qreal width = image.width();
        qreal height = image.height();
        qreal quotient = width / height;
        qreal minusPixel = vkCameraOneOptions->sliderCameraOneWindowZoom->value();
        QImage image_zoom = image.copy( minusPixel,
                                        minusPixel / quotient,
                                        width - (2 * minusPixel),
                                        height - (2 * minusPixel / quotient)
                                        );
        image = image_zoom.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    // Zoom end

    // Wenn der Wert des Schiebereglers größer Null ist soll skaliert werden
    if(vkCameraOneOptions->sliderCameraOneWindowSize->value() > 0){
        qreal quotient = (qreal)image.width() / (qreal)image.height();
        int w1 = ui->comboBoxCameraOneResolution->currentData().toSize().width();
        int w2 = vkCameraOneOptions->sliderCameraOneWindowSize->value();
        int w3 = w1 - w2;
        int h1 = ui->comboBoxCameraOneResolution->currentData().toSize().height();
        int h2 = vkCameraOneOptions->sliderCameraOneWindowSize->value() / quotient;
        int h3 = h1 - h2;
        image = image.scaled(w3, h3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

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
