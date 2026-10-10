#include "QvkAudioWindowsSingle.h"
#include "ui_QvkAudioWindowsSingle.h"
#include "QvkSettings.h"

#include <QString>
#include <QFrame>
#include <QCheckBox>
#include <QSize>
#include <QIcon>
#include <QMouseEvent>
#include <QToolButton>

QvkAudioWindowsSingle::QvkAudioWindowsSingle(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::QvkAudioWindowsSingle)
{
    ui->setupUi(this);
    show();
}


QvkAudioWindowsSingle::~QvkAudioWindowsSingle()
{
    delete ui;
}


void QvkAudioWindowsSingle::set_GUIui(Ui_formMainWindow *ui)
{
    GuiUi = ui;
}


void QvkAudioWindowsSingle::init(QString string)
{
    QString deviceID    = string.section( ":::", 0, 0 ); // DeviceID
    QString description = string.section( ":::", 1, 1 ); // Beschreibung
    QString type        = string.section( ":::", 2, 2 ); // Microphone or speaker
    QString api         = string.section( ":::", 3, 3 ); // alsa
    QString action      = string.section( ":::", 4, 4 ); // Action: Added or removed
    Q_UNUSED(api)
    Q_UNUSED(action)

    connect(ui->checkBoxAudioDevice,
            &QCheckBox::clicked,
            this,
            [=](bool value){emit signal_haveAudioDeviceSelected(value);}
    );

    ui->checkBoxAudioDevice->setAccessibleName(string);
    // Hier wird das device mit "--" benötigt wegen Settings
    ui->checkBoxAudioDevice->setObjectName(ui->checkBoxAudioDevice->objectName() + "--" + deviceID);
    ui->checkBoxAudioDevice->setToolTip(tr("Select one or more devices"));
    ui->checkBoxAudioDevice->setText("");
    ui->checkBoxAudioDevice->setToolTip("ID: " + deviceID + " \n" + "Device: " + description);

    ui->labelAudioDevice->setObjectName(ui->labelAudioDevice->objectName() + "_" + deviceID);
    if ( description.length() > 45 ){
        description = description.first(45);
    }
    ui->labelAudioDevice->setText(description);
    ui->labelAudioDevice->setToolTip("ID: " + deviceID + " \n" + "Device: " + description);

    ui->progressBarAudioDevice->setObjectName(ui->progressBarAudioDevice->objectName() + "_" + deviceID);
    ui->progressBarAudioDevice->setValue(0);
    ui->progressBarAudioDevice->setToolTip("ID: " + deviceID + " \n" + "Device: " + description);

    ui->toolButtonAudioLevelmeter->setObjectName(ui->toolButtonAudioLevelmeter->objectName() + "--" + deviceID);
    ui->toolButtonAudioLevelmeter->setToolTip(ui->toolButtonAudioLevelmeter->objectName());

    vkAudioWindowsLevelMeter = new QvkAudioWindowsLevelMeter;

    connect( ui->toolButtonAudioLevelmeter,
             &QToolButton::clicked,
             this,
             [=](bool value){
        if ( value == true ){
            vkAudioWindowsLevelMeter->start(deviceID);
        }
        if ( value == false ){
            vkAudioWindowsLevelMeter->stop();
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

    connect(vkAudioWindowsLevelMeter,
            &QvkAudioWindowsLevelMeter::signal_levelmeter,
            this,
            [=](qreal value){
        ui->progressBarAudioDevice->setValue(value * 1000);
    });

    QvkSettings vkSettings;
    bool bo = vkSettings.readAudioWindowsDevice(ui->checkBoxAudioDevice->objectName());
    if (bo == true){
        ui->checkBoxAudioDevice->click();
    }

    bo = vkSettings.readAudioWindowsDevice(ui->toolButtonAudioLevelmeter->objectName());
    if (bo == true){
        ui->toolButtonAudioLevelmeter->click();
    }

}


void QvkAudioWindowsSingle::mouseReleaseEvent( QMouseEvent *event )
{
    if( event->button() == Qt::LeftButton) {
        if ( event->type() == QMouseEvent::MouseButtonRelease ) {
            if ( rect().contains( event->pos() ) ) {
                ui->checkBoxAudioDevice->click();
            }
        }
    }
}
