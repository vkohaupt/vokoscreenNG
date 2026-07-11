
#include <QWidget>
#include <QCheckBox>
#include <QHBoxLayout>

#include "QvkCameraOneSingle.h"
#include "QvkCameraOneWindow.h"
#include "QvkCameraOneOptions.h"

#include "ui_QvkCameraOneSingle.h"
#include "ui_QvkCameraOneOptions.h"

// Varibale device enthält zum Beispiel folgenden Inhalt
// "/dev/video1:::UVC Camera (046d:0809):::added" or removed
QvkCameraOneSingle::QvkCameraOneSingle(QWidget *parent, QString device) :
    QWidget(parent),
    ui(new Ui::QvkCameraOneSingle)
{
    ui->setupUi(this);

    setObjectName(objectName() + "_" + device.section(":::", 0, 0));
    ui->checkBoxCameraOneOnOff->setText(device.section(":::", 1, 1));
    ui->radioButton->setObjectName(ui->radioButton->objectName() + "_" + device.section(":::", 0, 0));

    QvkCameraOneOptions *vkCameraOneOptions = new QvkCameraOneOptions(this);
    vkCameraOneOptions->setObjectName( vkCameraOneOptions->objectName() + "_" + device.section(":::", 0, 0));
    QVBoxLayout *layout = parent->topLevelWidget()->findChild<QVBoxLayout *>("verticalLayout_42");

    QString m_objectName = vkCameraOneOptions->ui->checkBox_Frame_OnOff->objectName() + "_" + device.section(":::", 0, 0);
    vkCameraOneOptions->ui->checkBox_Frame_OnOff->setObjectName(m_objectName);

    layout->addWidget(vkCameraOneOptions);

    // Window ON/Off
    connect(ui->checkBoxCameraOneOnOff,&QCheckBox::clicked, this, [=](bool value){
        if (value == true){
            if (vkCameraOneWindow == NULL){
                // deviceName ist z.b "/dev/video0"
                QString deviceName = objectName().section("_", 1, 1);
                vkCameraOneWindow = new QvkCameraOneWindow(this, deviceName);
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
    connect(vkCameraOneOptions->ui->checkBox_Frame_OnOff,
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
