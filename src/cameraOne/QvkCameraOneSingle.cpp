#include "QvkCameraOneSingle.h"
#include "QvkCameraOneWindow.h"
#include "ui_QvkCameraOneSingle.h"

QvkCameraOneSingle::QvkCameraOneSingle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QvkCameraOneSingle)
{
    ui->setupUi(this);

    QList<QCheckBox *> listCheckBox = parent->topLevelWidget()->findChildren<QCheckBox *>();
    for (int i = 0; i < listCheckBox.count(); i++){
        QCheckBox *checkBox = listCheckBox.at(i);
        if (checkBox->objectName() == "checkBox_Frame_OnOff"){
            checkBox_Frame_OnOff = checkBox;
            break;
        }
    }

    // Window ON/Off
    connect(ui->checkBoxCameraOneOnOff,&QCheckBox::clicked, this, [=](bool value){
        if (value == true){
            if (vkCameraOneWindow == NULL){
                vkCameraOneWindow = new QvkCameraOneWindow(this);
                vkCameraOneWindow->show();
                vkCameraOneWindow->move(cameraOneWindow_X, cameraOneWindow_Y);
            }
        }else{
            if (vkCameraOneWindow != NULL){
                cameraOneWindow_X = vkCameraOneWindow->get_camera_window_x();
                cameraOneWindow_Y = vkCameraOneWindow->get_camera_window_y();
                // Kein deletelater verwenden da dadurch das Fenster erst in der Hauptschleife zerstört wird
                // wenn checkBox_Frame_OnOff getätigt wird
                delete vkCameraOneWindow;
                vkCameraOneWindow = NULL;
            }
        }
    });

    // Frame On/Off
    connect(checkBox_Frame_OnOff,
            &QCheckBox
            ::clicked,
            this, [=](){
        if(ui->checkBoxCameraOneOnOff->isChecked() == true){
            if (vkCameraOneWindow != NULL){
                ui->checkBoxCameraOneOnOff->click(); // Erster Klick zum entfernen des Fensters
                ui->checkBoxCameraOneOnOff->click(); // Zweiter Klick zum anzeigen des Fensters
                vkCameraOneWindow->move(cameraOneWindow_X, cameraOneWindow_Y);
            }
        }
    });
}

QvkCameraOneSingle::~QvkCameraOneSingle()
{
    delete ui;
}
