#include "QvkCameraOneController.h"

#include <QWidget>
#include <QCheckBox>
#include <QList>

QvkCameraOneController::QvkCameraOneController(QWidget *parent) :
    QWidget(parent)
{
    QList<QCheckBox *> listCheckBox = parent->topLevelWidget()->findChildren<QCheckBox *>();
    for (int i = 0; i < listCheckBox.count(); i++){
        QCheckBox *checkBox = listCheckBox.at(i);
        if (checkBox->objectName() == "checkBox_Camera_OnOff"){
            checkBox_Camera_OnOff = checkBox;
            break;
        }
    }

    for (int i = 0; i < listCheckBox.count(); i++){
        QCheckBox *checkBox = listCheckBox.at(i);
        if (checkBox->objectName() == "checkBox_Frame_OnOff"){
            checkBox_Frame_OnOff = checkBox;
            break;
        }
    }

    // Camera ON/Off
    connect(checkBox_Camera_OnOff,&QCheckBox::clicked, this, [=](bool value){
        if (value == true){
            if (vkCameraNG == NULL){
                vkCameraNG = new QvkCameraNG(this);
                vkCameraNG->show();
                vkCameraNG->move(400, 400);
            }
        }else{
            if (vkCameraNG != NULL){
                // Kein deletelater verwenden da die Fenster versetzt angezeigt werden
                // wenn checkBox_Frame_OnOff getätigt wird
                delete vkCameraNG;
                vkCameraNG = NULL;
            }
        }
    });

    // Frame On/Off
    connect(checkBox_Frame_OnOff,
            &QCheckBox
            ::clicked,
            this, [=](){
        if(checkBox_Camera_OnOff->isChecked() == true){
            if (vkCameraNG != NULL){
                cameraNG_X = vkCameraNG->get_camera_window_x();
                cameraNG_Y = vkCameraNG->get_camera_window_y();
                checkBox_Camera_OnOff->click(); // Erster Klick zum entfernen des Fensters
                checkBox_Camera_OnOff->click(); // Zweiter Klick zum anzeigen des Fensters
                vkCameraNG->move(cameraNG_X, cameraNG_Y);
            }
        }
    });
}

QvkCameraOneController::~QvkCameraOneController()
{
}

