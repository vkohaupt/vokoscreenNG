
#include <QWidget>
#include <QString>
#include <QCheckBox>
#include <QVBoxLayout>

#include "global.h"
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
    ui->radioButton->setObjectName(ui->radioButton->objectName() + "_" + device.section(":::", 0, 0));
    ui->checkBoxCameraOneOnOff->setText(device.section(":::", 1, 1));
    QString pixelformat = ui->comboBoxCameraOnePixelformat->objectName() + "_" + device.section(":::", 0, 0);
    ui->comboBoxCameraOnePixelformat->setObjectName(pixelformat);
    QString resolution = ui->comboBoxCameraOneResolution->objectName() + "_" + device.section(":::", 0, 0);
    ui->comboBoxCameraOneResolution->setObjectName(resolution);

    vkCameraOneOptions = new QvkCameraOneOptions(this);
    vkCameraOneOptions->setObjectName( vkCameraOneOptions->objectName() + "_" + device.section(":::", 0, 0));
    vkCameraOneOptions->ui->labelCameraOneOptions->setText(device.section(":::", 1, 1));
    QString m_objectName = vkCameraOneOptions->ui->checkBox_Frame_OnOff->objectName() + "_" + device.section(":::", 0, 0);
    vkCameraOneOptions->ui->checkBox_Frame_OnOff->setObjectName(m_objectName);
    m_objectName = vkCameraOneOptions->ui->lineEditCameraTitel->objectName() + "_" + device.section(":::", 0, 0);
    vkCameraOneOptions->ui->lineEditCameraTitel->setObjectName(m_objectName);

    QVBoxLayout *layout = parent->topLevelWidget()->findChild<QVBoxLayout *>("verticalLayout_42");
    layout->addWidget(vkCameraOneOptions);

    // Window ON/Off
    connect(ui->checkBoxCameraOneOnOff, &QCheckBox::clicked, this, [=](bool value){
        if (value == true){
            if (vkCameraOneWindow == NULL){
                vkCameraOneWindow = new QvkCameraOneWindow(this, device.section(":::", 0, 0));
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
            &QCheckBox::clicked,
            this,
            [=](){
        if(ui->checkBoxCameraOneOnOff->isChecked() == true){
            if (vkCameraOneWindow != NULL){
                ui->checkBoxCameraOneOnOff->click(); // Erster Klick zum entfernen des Fensters
                ui->checkBoxCameraOneOnOff->click(); // Zweiter Klick zum anzeigen des Fensters
                vkCameraOneWindow->move(cameraOneWindow_X, cameraOneWindow_Y);
            }
        }
    });

    // Options On/Off
    connect(ui->radioButton,
            &QRadioButton::toggled,
            this,
            [=](bool checked){
        if (checked == true){
            // device ist z.b /dev/video1
            QString device = vkCameraOneOptions->objectName().section("_", 1, 1);
            QList<QvkCameraOneOptions *> listOptions = parent->topLevelWidget()->findChildren<QvkCameraOneOptions *>();
            for (int i = 0; i < listOptions.count(); i++){
                if (listOptions.at(i)->objectName().section("_", 1, 1) == device){
                    listOptions.at(i)->show();
                }else{
                    listOptions.at(i)->hide();
                }
            }
        }
    });
    ui->radioButton->click();

    connect(vkCameraOneOptions->ui->lineEditCameraTitel,
            &QLineEdit::textChanged,
            this,
            [=](){
        if (vkCameraOneWindow != NULL){
            vkCameraOneWindow->setWindowTitle(vkCameraOneOptions->ui->lineEditCameraTitel->text());
        }
    });

    // Erst suchen dann setzen
    QLineEdit *lineEdit = parent->topLevelWidget()->findChild<QLineEdit *>("lineEditCameraTitel_" + device.section(":::", 0, 0));
    lineEdit->setText(global::name + "  " + global::version);

}

QvkCameraOneSingle::~QvkCameraOneSingle()
{
    delete ui;
}
