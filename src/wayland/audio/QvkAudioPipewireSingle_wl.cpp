#include "QvkAudioPipewireSingle_wl.h"
#include "QvkAudioPipewireLevelMeter_wl.h"
#include "ui_QvkAudioPipewireSingle_wl.h"
#include "QvkSettings_wl.h"

#include <QString>
#include <QFrame>
#include <QCheckBox>
#include <QSize>
#include <QIcon>
#include <QMouseEvent>

QvkAudioPipewireSingle_wl::QvkAudioPipewireSingle_wl(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::QvkAudioPipewireSingle_wl)
{
    ui->setupUi(this);
    show();
}


QvkAudioPipewireSingle_wl::~QvkAudioPipewireSingle_wl()
{
    delete ui;
}


void QvkAudioPipewireSingle_wl::set_GUIui(Ui_formMainWindow_wl *ui)
{
    GuiUi = ui;
}


void QvkAudioPipewireSingle_wl::init(QString string)
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
    ui->checkBoxAudioDevice->setObjectName("checkBoxAudioDevice--" + device);
    ui->checkBoxAudioDevice->setToolTip(tr("Select one or more devices"));
    ui->checkBoxAudioDevice->setText("");
    ui->checkBoxAudioDevice->setToolTip(deviceID + " " + device);

    ui->labelAudioDevice->setText(description);
    ui->labelAudioDevice->setToolTip(deviceID + " " + device);

    ui->progressBarAudioDevice->setToolTip(deviceID + " " + device);

    if (type == "Playback"){
        ui->checkBoxAudioDevice->setIconSize(QSize(16, 16));
        ui->checkBoxAudioDevice->setIcon(QIcon(":/pictures/screencast/speaker.png"));
    }
    if (type == "Source"){
        ui->checkBoxAudioDevice->setIconSize(QSize(16, 16));
        ui->checkBoxAudioDevice->setIcon(QIcon(":/pictures/screencast/microphone.png"));
    }


    QvkAudioPipewireLevelMeter_wl *vkAudioPipewireLevelMeter_wl = new QvkAudioPipewireLevelMeter_wl;
    vkAudioPipewireLevelMeter_wl->start(deviceID, "voko", "00");


    QvkSettings_wl vkSettings_wl;
    bool bo = vkSettings_wl.readAudioDevice(ui->checkBoxAudioDevice->objectName());
    if (bo == true){
        ui->checkBoxAudioDevice->click();
    }
}


void QvkAudioPipewireSingle_wl::mouseReleaseEvent( QMouseEvent *event )
{
    if( event->button() == Qt::LeftButton) {
        if ( event->type() == QMouseEvent::MouseButtonRelease ) {
            if ( rect().contains( event->pos() ) ) {
                ui->checkBoxAudioDevice->click();
            }
        }
    }
}
