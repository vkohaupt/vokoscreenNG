#include "QvkCameraOneController.h"
#include "QvkCameraOneWatcher.h"
#include "QvkCameraOneSingle.h"

#include <QWidget>
#include <QString>

/*
ls -l /dev/v4l/by-id/
Ausgabe: usb-Logitech_HD_Pro_Webcam_C920_A1B2C3D4-video-index0 -> ../../video0
Eindeutiges Merkmal: Hier wird die Seriennummer der Hardware
(im Beispiel A1B2C3D4) oder der physische USB-Port eingewebt.
Nutzen Sie diesen Pfad in Ihren Skripten oder Anwendungen,
um immer exakt dieselbe Kamera anzusprechen.
*/

QvkCameraOneController::QvkCameraOneController(
        QWidget *parent, Ui_formMainWindow *m_GuiUi) : QWidget(parent)
{
    GuiUi = m_GuiUi;
    hide();
    QvkCameraOneWatcher *vkCameraOneWatcher = new QvkCameraOneWatcher;

    // Varibale device enthält zum Beispiel folgenden Inhalt
    // "/dev/video1:::UVC Camera (046d:0809):::added" or removed
    connect(vkCameraOneWatcher,
            &QvkCameraOneWatcher::signal_cameraChanged,
            this,
            [=](QString device){
        if (device.section(":::", 2, 2) == "added"){
            vkCameraOneSingle = new QvkCameraOneSingle(this, device, GuiUi);
            buttonGroup->addButton(vkCameraOneSingle->ui->radioButtonCameraOneSelect);
            GuiUi->verticalLayout_28->addWidget(vkCameraOneSingle);
            emit signal_forSystrayCameraAdded(vkCameraOneSingle->ui->checkBoxCameraOneOnOff);
        }

        if (device.section(":::", 2, 2) == "removed"){
            QString removedDevice = "QvkCameraOneSingle_" + device.section(":::", 0, 0);
            QvkCameraOneSingle *vkCameraOneSingle = topLevelWidget()->findChild<QvkCameraOneSingle *>(removedDevice);
            if (vkCameraOneSingle != nullptr){
                delete vkCameraOneSingle;
            }

            removedDevice = "QvkCameraOneOptions_" + device.section(":::", 0, 0);
            QvkCameraOneOptions *vkCameraOneOptions = topLevelWidget()->findChild<QvkCameraOneOptions *>(removedDevice);
            if (vkCameraOneOptions != nullptr){
                delete vkCameraOneOptions;
            }
        }

        // Den Radiobutton und das Label verstecken wenn nur eine Camera angezeigt wird
        QList<QvkCameraOneSingle *> cameraOneSingle = topLevelWidget()->findChildren<QvkCameraOneSingle *>();
        for(int i = 0; i < cameraOneSingle.count(); i++){
            if (cameraOneSingle.count() == 1){
                cameraOneSingle.at(i)->ui->radioButtonCameraOneSelect->hide();
                cameraOneSingle.at(i)->vkCameraOneOptions->ui->labelCameraOneOptions->hide();
                cameraOneSingle.at(i)->ui->radioButtonCameraOneSelect->click();
            }else{
                cameraOneSingle.at(i)->ui->radioButtonCameraOneSelect->show();
                cameraOneSingle.at(i)->vkCameraOneOptions->ui->labelCameraOneOptions->show();
                cameraOneSingle.at(i)->ui->radioButtonCameraOneSelect->click();
            }
        }
    });
    vkCameraOneWatcher->init();
}

QvkCameraOneController::~QvkCameraOneController()
{
}

