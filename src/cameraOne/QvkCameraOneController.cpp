#include "QvkCameraOneController.h"
#include "QvkCameraOneWatcher.h"
#include "QvkCameraOneSingle.h"

#include <QWidget>
#include <QString>

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

