#include "QvkAudioPipewireSingle.h"
#include "ui_QvkAudioPipewireSingle.h"
#include "QvkSettings.h"

#include <QString>
#include <QFrame>
#include <QCheckBox>
#include <QSize>
#include <QIcon>
#include <QMouseEvent>
#include <QToolButton>

QvkAudioPipewireSingle::QvkAudioPipewireSingle(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::QvkAudioPipewireSingle)
{
    ui->setupUi(this);
    show();
}


QvkAudioPipewireSingle::~QvkAudioPipewireSingle()
{
    delete ui;
}


void QvkAudioPipewireSingle::set_GUIui(Ui_formMainWindow *ui)
{
    GuiUi = ui;
}


void QvkAudioPipewireSingle::init(QString string)
{
    QString deviceID    = string.section( ":::", 0, 0 ); // DeviceID
    QString description = string.section( ":::", 1, 1 ); // Beschreibung
    QString type        = string.section( ":::", 2, 2 ); // Microphone or speaker
    QString api         = string.section( ":::", 3, 3 ); // alsa
    QString action      = string.section( ":::", 4, 4 ); // Action: Added or removed
    QString device      = string.section( ":::", 5, 5 ); // DeviceName
    Q_UNUSED(api)
    Q_UNUSED(action)

    connect(ui->checkBoxAudioDevice,
            &QCheckBox::clicked,
            this,
            [=](bool value){emit signal_haveAudioDeviceSelected(value);}
    );

    ui->checkBoxAudioDevice->setAccessibleName(string);
    // Hier wird das device mit "--" benötigt wegen Settings
    ui->checkBoxAudioDevice->setObjectName(ui->checkBoxAudioDevice->objectName() + "--" + device);
    ui->checkBoxAudioDevice->setToolTip(tr("Select one or more devices"));
    ui->checkBoxAudioDevice->setText("");
    ui->checkBoxAudioDevice->setToolTip("ID: " + deviceID + " \n" + "Device: " + device);

    ui->labelAudioDevice->setObjectName(ui->labelAudioDevice->objectName() + "_" + deviceID);
    if ( description.length() > 45 ){
        description = description.first(45);
    }
    ui->labelAudioDevice->setText(description);
    ui->labelAudioDevice->setToolTip("ID: " + deviceID + " \n" + "Device: " + device);

    ui->progressBarAudioDevice->setObjectName(ui->progressBarAudioDevice->objectName() + "_" + deviceID);
    ui->progressBarAudioDevice->setValue(0);
    ui->progressBarAudioDevice->setToolTip("ID: " + deviceID + " \n" + "Device: " + device);

    ui->toolButtonAudioLevelmeter->setObjectName(ui->toolButtonAudioLevelmeter->objectName() + "--" + device);
    ui->toolButtonAudioLevelmeter->setToolTip(ui->toolButtonAudioLevelmeter->objectName());

    vkAudioPipewireLevelMeter = new QvkAudioPipewireLevelMeter;

    connect( ui->toolButtonAudioLevelmeter,
             &QToolButton::clicked,
             this,
             [=](bool value){
        if ( value == true ){
            vkAudioPipewireLevelMeter->start(deviceID, "Levelmeter");
        }
        if ( value == false ){
            vkAudioPipewireLevelMeter->stop();
            ui->progressBarAudioDevice->setValue(0);
        }}
    );

    if (type == "Playback"){
        ui->checkBoxAudioDevice->setIconSize(QSize(16, 16));
        ui->checkBoxAudioDevice->setIcon(QIcon(":/pictures/screencast/speaker.png"));
    }
    if (type == "Source"){
        ui->checkBoxAudioDevice->setIconSize(QSize(16, 16));
        ui->checkBoxAudioDevice->setIcon(QIcon(":/pictures/screencast/microphone.png"));
    }

    connect(vkAudioPipewireLevelMeter,
            &QvkAudioPipewireLevelMeter::signal_levelmeter,
            this,
            [=](qreal value){
        ui->progressBarAudioDevice->setValue(value * 1000);
    });
/*
    QvkSettings vkSettings;
    bool bo = vkSettings.readAudioDevice(ui->checkBoxAudioDevice->objectName());
    if (bo == true){
        ui->checkBoxAudioDevice->click();
    }

    bo = vkSettings.readAudioDevice(ui->toolButtonAudioLevelmeter->objectName());
    if (bo == true){
        ui->toolButtonAudioLevelmeter->click();
    }
*/
}


void QvkAudioPipewireSingle::mouseReleaseEvent( QMouseEvent *event )
{
    if( event->button() == Qt::LeftButton) {
        if ( event->type() == QMouseEvent::MouseButtonRelease ) {
            if ( rect().contains( event->pos() ) ) {
                ui->checkBoxAudioDevice->click();
            }
        }
    }
}
