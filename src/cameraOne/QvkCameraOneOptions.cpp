#include "QvkCameraOneOptions.h"
#include "QvkSpezialSlider.h"

#include "ui_QvkCameraOneOptions.h"

QvkCameraOneOptions::QvkCameraOneOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QvkCameraOneOptions)
{
    ui->setupUi(this);
}

QvkCameraOneOptions::~QvkCameraOneOptions()
{
    delete ui;
}

// Varibale m_device enthält zum Beispiel folgenden Inhalt
// "/dev/video1:::UVC Camera (046d:0809):::added" or removed
void QvkCameraOneOptions::set_init(QString m_device)
{
    QString device = "_" + m_device.section(":::", 0, 0);

    connect(ui->toolButtonCameraOneFramelessOnOff,
            &QToolButton::toggled,
            this, [=](bool value){
        if (value == false){
            ui->toolButtonCameraOneFramelessOnOff->setIcon(QIcon(":/pictures/camera/window-frameless-off.png"));
        }else{
            ui->toolButtonCameraOneFramelessOnOff->setIcon(QIcon(":/pictures/camera/window-frameless-on.png"));
        }
    });

    QString m_objectName = objectName() + device.section(":::", 0, 0);
    setObjectName(m_objectName);

    m_objectName = ui->labelCameraOneOptions->objectName();
    ui->labelCameraOneOptions->setObjectName(m_objectName);
    ui->labelCameraOneOptions->setText(m_device.section(":::", 1, 1));

    m_objectName = ui->toolButtonCameraOneViewRectangle->objectName() + device;
    ui->toolButtonCameraOneViewRectangle->setObjectName(m_objectName);

    m_objectName = ui->toolButtonCameraOneViewEllipse->objectName() + device;
    ui->toolButtonCameraOneViewEllipse->setObjectName(m_objectName);

    m_objectName = ui->toolButtonCameraOneViewCircle->objectName() + device;
    ui->toolButtonCameraOneViewCircle->setObjectName(m_objectName);

    m_objectName = ui->lineEditCameraOneTitel->objectName() + device;
    ui->lineEditCameraOneTitel->setObjectName(m_objectName);

    m_objectName = ui->toolButtonCameraOneTitelReset->objectName() + device;
    ui->toolButtonCameraOneTitelReset->setObjectName(m_objectName);

    m_objectName = ui->toolButtonCameraOneFramelessOnOff->objectName() + device;
    ui->toolButtonCameraOneFramelessOnOff->setObjectName(m_objectName);

    m_objectName = ui->toolButtonCameraOneMirrorVertical->objectName() + device;
    ui->toolButtonCameraOneMirrorVertical->setObjectName(m_objectName);

    m_objectName = ui->toolButtonCameraOneMirrorHorizontal->objectName() + device;
    ui->toolButtonCameraOneMirrorHorizontal->setObjectName(m_objectName);

    m_objectName = ui->labelCameraOneWindowSize->objectName() + device;
    ui->labelCameraOneWindowSize->setObjectName(m_objectName);

    m_objectName = ui->comboBoxCameraOneColorTemperature->objectName() + device;
    ui->comboBoxCameraOneColorTemperature->setObjectName(device);

    sliderCameraOneWindowSize = new QvkSpezialSlider(Qt::Horizontal);
    ui->horizontalLayout_2->insertWidget(0, sliderCameraOneWindowSize);
    sliderCameraOneWindowSize->setObjectName("sliderCameraOneWindowSize" + device);
    sliderCameraOneWindowSize->setMinimum(0);
    sliderCameraOneWindowSize->setMaximum(1);
    sliderCameraOneWindowSize->setValue(0);
    sliderCameraOneWindowSize->show();
    sliderCameraOneWindowSize->setShowValue(true);
    sliderCameraOneWindowSize->setBigHandel(true);
    sliderCameraOneWindowSize->setEnabled(true);

    sliderCameraOneWindowZoom = new QvkSpezialSlider(Qt::Horizontal);
    ui->horizontalLayout_5->insertWidget(0, sliderCameraOneWindowZoom);
    sliderCameraOneWindowZoom->setObjectName("sliderCameraOneWindowZoom" + device);;
    sliderCameraOneWindowZoom->setMinimum(0);
    sliderCameraOneWindowZoom->setMaximum(1);
    sliderCameraOneWindowZoom->setValue(0);
    sliderCameraOneWindowZoom->show();
    sliderCameraOneWindowZoom->setShowValue(true);
    sliderCameraOneWindowZoom->setEnabled(true);

    sliderCameraOneColorTemperature = new QvkSpezialSlider(Qt::Horizontal);
    ui->horizontalLayout_7->addWidget(sliderCameraOneColorTemperature);
    sliderCameraOneColorTemperature->setObjectName("sliderCameraOneColorTemperature" + device);;
    sliderCameraOneColorTemperature->setMinimum(0);
    sliderCameraOneColorTemperature->setMaximum(1);
    sliderCameraOneColorTemperature->setValue(0);
    sliderCameraOneColorTemperature->show();
    sliderCameraOneColorTemperature->setShowValue(true);
    sliderCameraOneColorTemperature->setEnabled(false);
/*
    // Jedem Widget muß ein eindeutiger Objectname zugewiesen werden
    // damit dieser in den Settings gspeichert werden kann.
    QObjectList widgetList = children();
    for (int i = 0; i < widgetList.count(); i++){
        widgetList.at(i)->setObjectName(widgetList.at(i)->objectName() + device);
    }

    QObjectList widget = children();
    qDebug() << widget;
*/

}
