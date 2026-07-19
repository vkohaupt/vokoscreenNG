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


void QvkCameraOneOptions::set_init(QString device)
{
    connect(ui->toolButtonCameraOneFramelessOnOff,
            &QToolButton::toggled,
            this, [=](bool value){
        if (value == false){
            ui->toolButtonCameraOneFramelessOnOff->setIcon(QIcon(":/pictures/camera/window-frameless-off.png"));
        }else{
            ui->toolButtonCameraOneFramelessOnOff->setIcon(QIcon(":/pictures/camera/window-frameless-on.png"));
        }
    });

    QString m_objectName;
    m_objectName = objectName() + "_" + device.section(":::", 0, 0);

    setObjectName(m_objectName);
    ui->labelCameraOneOptions->setText(device.section(":::", 1, 1));

    m_objectName = ui->toolButtonCameraOneViewRectangle->objectName() + "_" + device.section(":::", 0, 0);
    ui->toolButtonCameraOneViewRectangle->setObjectName(m_objectName);

    m_objectName = ui->toolButtonCameraOneViewEllipse->objectName() + "_" + device.section(":::", 0, 0);
    ui->toolButtonCameraOneViewEllipse->setObjectName(m_objectName);

    m_objectName = ui->toolButtonCameraOneViewCircle->objectName() + "_" + device.section(":::", 0, 0);
    ui->toolButtonCameraOneViewCircle->setObjectName(m_objectName);

    m_objectName = ui->lineEditCameraOneTitel->objectName() + "_" + device.section(":::", 0, 0);
    ui->lineEditCameraOneTitel->setObjectName(m_objectName);

    m_objectName = ui->toolButtonCameraOneTitelReset->objectName() + "_" + device.section(":::", 0, 0);
    ui->toolButtonCameraOneTitelReset->setObjectName(m_objectName);

    m_objectName = ui->toolButtonCameraOneFramelessOnOff->objectName() + "_" + device.section(":::", 0, 0);
    ui->toolButtonCameraOneFramelessOnOff->setObjectName(m_objectName);

    m_objectName = ui->toolButtonCameraOneMirrorVertical->objectName() + "_" + device.section(":::", 0, 0);
    ui->toolButtonCameraOneMirrorVertical->setObjectName(m_objectName);

    m_objectName = ui->toolButtonCameraOneMirrorHorizontal->objectName() + "_" + device.section(":::", 0, 0);
    ui->toolButtonCameraOneMirrorHorizontal->setObjectName(m_objectName);

    QLabel *labelCameraWindowSize = new QLabel;
    ui->horizontalLayout_3->addWidget(labelCameraWindowSize);
    labelCameraWindowSize->setObjectName("labelCameraOneWindowSize_" + device.section(":::", 0, 0));
    labelCameraWindowSize->setText("---x---");

    QvkSpezialSlider *sliderCameraOneWindowSize = new QvkSpezialSlider(Qt::Horizontal);
    ui->horizontalLayout_2->insertWidget(0, sliderCameraOneWindowSize);
    sliderCameraOneWindowSize->setObjectName("sliderCameraOneWindowSize_" + device.section(":::", 0, 0));
    sliderCameraOneWindowSize->setMinimum(0);
    sliderCameraOneWindowSize->setMaximum(1);
    sliderCameraOneWindowSize->setValue(0);
    sliderCameraOneWindowSize->show();
    sliderCameraOneWindowSize->setShowValue(false);
    sliderCameraOneWindowSize->setBigHandel(true);
    sliderCameraOneWindowSize->setEnabled(true);

    QvkSpezialSlider *sliderCameraOneWindowZoom = new QvkSpezialSlider(Qt::Horizontal);
    ui->horizontalLayout_5->insertWidget(0, sliderCameraOneWindowZoom);
    sliderCameraOneWindowZoom->setObjectName("sliderCameraOneWindowZoom_" + device.section(":::", 0, 0));;
    sliderCameraOneWindowZoom->setMinimum(0);
    sliderCameraOneWindowZoom->setMaximum(1);
    sliderCameraOneWindowZoom->setValue(0);
    sliderCameraOneWindowZoom->show();
    sliderCameraOneWindowZoom->setShowValue(true);
    sliderCameraOneWindowZoom->setEnabled(true);
}
