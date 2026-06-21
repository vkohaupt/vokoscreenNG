#include "QvkAudioPipewireSingle_wl.h"
#include "ui_QvkAudioPipewireSingle_wl.h"
#include "QvkSettings_wl.h"

#include <QString>
#include <QFrame>
#include <QCheckBox>
#include <QSize>
#include <QIcon>
#include <QMouseEvent>
#include <QLineEdit>

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
    // Hier wird das device mit "--" benötigt wegen Settings
    ui->checkBoxAudioDevice->setObjectName("checkBoxAudioDevice--" + device);
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

    ui->toolButtonAudioDevice->setObjectName(ui->toolButtonAudioDevice->objectName() + "_" + deviceID);
    ui->toolButtonAudioDevice->setToolTip(ui->toolButtonAudioDevice->objectName());
    connect( ui->toolButtonAudioDevice,
             &QToolButton::clicked,
             this,
             [=](bool value){
        if ( value == true ){
            vkAudioPipewireLevelMeter_wl->stop();
            ui->progressBarAudioDevice->setValue(0);
        }
        if ( value == false ){
            vkAudioPipewireLevelMeter_wl->start(deviceID, "Levelmeter", deviceID);
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

    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setObjectName( "lineEditLevelMeter_" + deviceID );
    global::listChildren->append( lineEdit );

    connect( lineEdit,
             &QLineEdit::textChanged,
             this,
             [=](QString value){
        ui->progressBarAudioDevice->setValue( value.toDouble() * 1000 );}
    );

    vkAudioPipewireLevelMeter_wl = new QvkAudioPipewireLevelMeter_wl;
    // Für den dritten Parameter wird ebenfalls die deviceID genommen da diese eindeutig ist.
//    vkAudioPipewireLevelMeter_wl->start(deviceID, "Levelmeter", deviceID);

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
