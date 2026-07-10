#include "QvkCameraOneController.h"
#include "QvkCameraOneWatcher.h"
#include "QvkCameraOneSingle.h"

#include <QWidget>
#include <QCheckBox>
#include <QList>
#include <QVBoxLayout>

QvkCameraOneController::QvkCameraOneController(QWidget *parent) : QWidget(parent)
{
    QvkCameraOneWatcher *vkCameraOneWatcher = new QvkCameraOneWatcher;

    // Varibale device enthält zum Beispiel folgenden Inhalt
    // "/dev/video1:::UVC Camera (046d:0809):::added" or removed
    connect(vkCameraOneWatcher,
            &QvkCameraOneWatcher::signal_cameraChanged,
            this,
            [=](QString device){
        if (device.section(":::", 2, 2) == "added"){
            QVBoxLayout *layoutForCameras = parent->topLevelWidget()->findChild<QVBoxLayout *>("verticalLayout_28");
            vkCameraOneSingle = new QvkCameraOneSingle(this);
            vkCameraOneSingle->setObjectName(vkCameraOneSingle->objectName() + "_" + device.section(":::", 0, 0));
            vkCameraOneSingle->ui->checkBoxCameraOneOnOff->setText(device.section(":::", 1, 1));
            layoutForCameras->addWidget(vkCameraOneSingle);
        }
        if (device.section(":::", 2, 2) == "removed"){
            QString removedDevice = "QvkCameraOneSingle_" + device.section(":::", 0, 0);
            QvkCameraOneSingle *vkCameraOneSingle = parent->topLevelWidget()->findChild<QvkCameraOneSingle *>(removedDevice);
            if (vkCameraOneSingle != nullptr){
                vkCameraOneSingle->deleteLater();
            }
        }
    });
    vkCameraOneWatcher->init();

}

QvkCameraOneController::~QvkCameraOneController()
{
}

