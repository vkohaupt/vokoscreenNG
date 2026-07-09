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

    // Varibale device enthält zum Beispiel folgenden Inhalt "/dev/video1:::UVC Camera (046d:0809):::added"
    connect(vkCameraOneWatcher,
            &QvkCameraOneWatcher::signal_cameraChanged,
            this,
            [=](QString device){
        if (device.section(":::", 2, 2) == "added"){
            QVBoxLayout *layoutForCameras = parent->topLevelWidget()->findChild<QVBoxLayout *>("verticalLayout_28");
            vkCameraOneSingle = new QvkCameraOneSingle(this);
            QString name = vkCameraOneSingle->objectName() + "_" + device.section(":::", 0, 0);
            vkCameraOneSingle->setObjectName(name);

            //vkCameraOneSingle->checkBox_Camera_OnOff

            layoutForCameras->addWidget(vkCameraOneSingle);
        }
    });
    vkCameraOneWatcher->init();

}

QvkCameraOneController::~QvkCameraOneController()
{
}

