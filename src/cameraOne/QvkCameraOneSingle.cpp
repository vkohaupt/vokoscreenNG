
#include <QWidget>
#include <QString>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QLabel>

#include "global.h"
#include "QvkCameraOneSingle.h"
#include "QvkCameraOneWindow.h"
#include "QvkCameraOneOptions.h"
#include "QvkSpezialSlider.h"

#include "ui_QvkCameraOneSingle.h"
#include "ui_QvkCameraOneOptions.h"

// Varibale device enthält zum Beispiel folgenden Inhalt
// "/dev/video1:::UVC Camera (046d:0809):::added" or removed
QvkCameraOneSingle::QvkCameraOneSingle(QWidget *parent, QString device, Ui_formMainWindow *m_GuiUi) :
    QWidget(parent),
    ui(new Ui::QvkCameraOneSingle)
{
    ui->setupUi(this);
    GuiUi = m_GuiUi;

    setObjectName(objectName() + "_" + device.section(":::", 0, 0));
    ui->radioButtonCameraOneSelect->setObjectName(ui->radioButtonCameraOneSelect->objectName() + "_" + device.section(":::", 0, 0));

    QString m_objectName = ui->checkBoxCameraOneOnOff->objectName() + "_" + device.section(":::", 0, 0);
    ui->checkBoxCameraOneOnOff->setObjectName(m_objectName);
    ui->checkBoxCameraOneOnOff->setText(device.section(":::", 1, 1));

    m_objectName = ui->comboBoxCameraOnePixelformat->objectName() + "_" + device.section(":::", 0, 0);
    ui->comboBoxCameraOnePixelformat->setObjectName(m_objectName);

    m_objectName = ui->comboBoxCameraOneResolution->objectName() + "_" + device.section(":::", 0, 0);
    ui->comboBoxCameraOneResolution->setObjectName(m_objectName);

    vkCameraOneOptions = new QvkCameraOneOptions(this);
    {
        m_objectName = vkCameraOneOptions->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->setObjectName(m_objectName);
        vkCameraOneOptions->ui->labelCameraOneOptions->setText(device.section(":::", 1, 1));

        m_objectName = vkCameraOneOptions->ui->toolButtonCameraOneViewRectangle->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->ui->toolButtonCameraOneViewRectangle->setObjectName(m_objectName);

        m_objectName = vkCameraOneOptions->ui->toolButtonCameraOneViewEllipse->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->ui->toolButtonCameraOneViewEllipse->setObjectName(m_objectName);

        m_objectName = vkCameraOneOptions->ui->toolButtonCameraOneViewCircle->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->ui->toolButtonCameraOneViewCircle->setObjectName(m_objectName);

        m_objectName = vkCameraOneOptions->ui->lineEditCameraOneTitel->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->ui->lineEditCameraOneTitel->setObjectName(m_objectName);

        m_objectName = vkCameraOneOptions->ui->toolButtonCameraOneTitelReset->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->ui->toolButtonCameraOneTitelReset->setObjectName(m_objectName);

        m_objectName = vkCameraOneOptions->ui->toolButtonCameraOneFramelessOnOff->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->ui->toolButtonCameraOneFramelessOnOff->setObjectName(m_objectName);

        m_objectName = vkCameraOneOptions->ui->toolButtonCameraOneMirrorVertical->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->ui->toolButtonCameraOneMirrorVertical->setObjectName(m_objectName);

        m_objectName = vkCameraOneOptions->ui->toolButtonCameraOneMirrorHorizontal->objectName() + "_" + device.section(":::", 0, 0);
        vkCameraOneOptions->ui->toolButtonCameraOneMirrorHorizontal->setObjectName(m_objectName);

        QLabel *labelCameraWindowSize = new QLabel;
        vkCameraOneOptions->ui->horizontalLayout_3->addWidget(labelCameraWindowSize);
        labelCameraWindowSize->setObjectName("labelCameraOneWindowSize_" + device.section(":::", 0, 0));
        labelCameraWindowSize->setText("---x---");

        QvkSpezialSlider *sliderCameraOneWindowSize = new QvkSpezialSlider(Qt::Horizontal);
        vkCameraOneOptions->ui->horizontalLayout_2->insertWidget(0, sliderCameraOneWindowSize);
        sliderCameraOneWindowSize->setObjectName("sliderCameraOneWindowSize_" + device.section(":::", 0, 0));
        sliderCameraOneWindowSize->setMinimum(0);
        sliderCameraOneWindowSize->setMaximum(1);
        sliderCameraOneWindowSize->setValue(0);
        sliderCameraOneWindowSize->show();
        sliderCameraOneWindowSize->setShowValue(false);
        sliderCameraOneWindowSize->setBigHandel(true);
        sliderCameraOneWindowSize->setEnabled(true);

        QvkSpezialSlider *sliderCameraOneWindowZoom = new QvkSpezialSlider(Qt::Horizontal);
        vkCameraOneOptions->ui->horizontalLayout_5->insertWidget(0, sliderCameraOneWindowZoom);
        sliderCameraOneWindowZoom->setObjectName("sliderCameraOneWindowZoom_" + device.section(":::", 0, 0));;
        sliderCameraOneWindowZoom->setMinimum(0);
        sliderCameraOneWindowZoom->setMaximum(1);
        sliderCameraOneWindowZoom->setValue(0);
        sliderCameraOneWindowZoom->show();
        sliderCameraOneWindowZoom->setShowValue(true);
        sliderCameraOneWindowZoom->setEnabled(true);
    }
    GuiUi->verticalLayout_39->addWidget(vkCameraOneOptions);

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
    connect(vkCameraOneOptions->ui->toolButtonCameraOneFramelessOnOff,
            &QToolButton::clicked,
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
    connect(ui->radioButtonCameraOneSelect,
            &QRadioButton::toggled,
            this,
            [=](bool checked){
        if (checked == true){
            // device ist z.b /dev/video1
            QString device = vkCameraOneOptions->objectName().section("_", 1, 1);
            QList<QvkCameraOneOptions *> listOptions = topLevelWidget()->findChildren<QvkCameraOneOptions *>();
            for (int i = 0; i < listOptions.count(); i++){

                // "radioButtonCameraOneSelect_" ist der ObjectName
                QString nameObject = listOptions.at(i)->objectName().section("_", 0, 0) + "_";
                QString nameDevice = listOptions.at(i)->objectName().replace(nameObject, "");
                if (nameDevice == device){
                    listOptions.at(i)->show();
                }else{
                    listOptions.at(i)->hide();
                }

/*
                if (listOptions.at(i)->objectName().section("_", 1, 1) == device){
                    listOptions.at(i)->show();
                }else{
                    listOptions.at(i)->hide();
                }
*/
            }
        }
    });

    connect(vkCameraOneOptions->ui->lineEditCameraOneTitel,
            &QLineEdit::textChanged,
            this,
            [=](){
        if (vkCameraOneWindow != NULL){
            vkCameraOneWindow->setWindowTitle(vkCameraOneOptions->ui->lineEditCameraOneTitel->text());
        }
    });

    connect(vkCameraOneOptions->ui->toolButtonCameraOneTitelReset, &QToolButton::clicked, this, [=](){
        vkCameraOneOptions->ui->lineEditCameraOneTitel->setText(global::name + "  " + global::version);
    });
    vkCameraOneOptions->ui->toolButtonCameraOneTitelReset->click();
}

QvkCameraOneSingle::~QvkCameraOneSingle()
{
    delete ui;
}
