#include "QvkCameraSingle_wl.h"
#include "ui_QvkCameraSingle_wl.h"

QvkCameraSingle_wl::QvkCameraSingle_wl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QvkCameraSingle_wl)
{
    ui->setupUi(this);

}


void QvkCameraSingle_wl::set_objectName(QString device)
{
    setObjectName("cameraSingleVideoID_" + device.section(":::", 0, 0)); // Im ObjectName steckt die id
    ui->checkBoxCameraOnOff->setText(device.section(":::", 1, 1 ));
    ui->checkBoxCameraOnOff->setObjectName("checkBoxCameraVideoID_" + device.section(":::", 0, 0));
    ui->comboBoxCameraPixelformat->setObjectName("comboBoxCameraPixelformatVideoID_" + device.section(":::", 0, 0));
    ui->comboBoxCameraResolution->setObjectName("comboBoxCameraResolutionVideoID_" + device.section(":::", 0, 0));
    ui->comboBoxCameraFPS->setObjectName("comboBoxCameraFPSVideoID_" + device.section(":::", 0, 0));
}


QvkCameraSingle_wl::~QvkCameraSingle_wl()
{
    delete ui;
}
