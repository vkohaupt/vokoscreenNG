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

    connect(ui->checkBoxCameraOnOff, &QCheckBox::clicked, this, [=](bool value){
        slot_checkBoxCameraOnOff(value, ui->checkBoxCameraOnOff);
        emit signal_forSystrayCameraOnOff(ui->checkBoxCameraOnOff);
    });
}


QvkCameraSingle_wl::~QvkCameraSingle_wl()
{
    delete ui;
}


void QvkCameraSingle_wl::slot_checkBoxCameraOnOff(bool checked, QCheckBox *checkBoxCameraOnOff)
{
    qDebug() << "1111111111111111111111111111111111111111111111111111111111";

    // Disable all other cameras
    if ( checked == true ){
        QList<QCheckBox *> listCheckBox = findChildren<QCheckBox *>();
        for ( int i = 0; i < listCheckBox.count(); i++ ) {
            QCheckBox *checkBox = listCheckBox.at(i);
            if ( checkBox->objectName().contains("checkBoxCameraVideoID_") ){
                if ( checkBox->isChecked() == false ){
                    checkBox->setDisabled(true);
                }
            }
        }
    }

    // Enable all cameras
    if ( checked == false ){
        QList<QCheckBox *> listCheckBox = findChildren<QCheckBox *>();
        for ( int i = 0; i < listCheckBox.count(); i++ ) {
            QCheckBox *checkBox = listCheckBox.at(i);
            if ( checkBox->objectName().contains("checkBoxCameraVideoID_") ){
                if ( checkBox->isChecked() == false ){
                    checkBox->setEnabled(true);
                }
            }
        }
    }
}
