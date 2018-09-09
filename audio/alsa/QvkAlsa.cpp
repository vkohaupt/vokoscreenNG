#include "QvkAlsa.h"

#include <QAudioDeviceInfo>
#include <QDebug>

QvkAlsa::QvkAlsa( Ui_MainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;
}


QvkAlsa::~QvkAlsa()
{
}


void QvkAlsa::slot_getAlsaDevices( bool value )
{
    Q_UNUSED(value);
    foreach (const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices( QAudio::AudioInput ) )
    {
        qDebug() << deviceInfo.deviceName();
        if ( deviceInfo.deviceName().contains("alsa") == false)
        {
            QCheckBox *checkboxAudioDevice = new QCheckBox();
            checkboxAudioDevice->setText( deviceInfo.deviceName() );
            checkboxAudioDevice->setAccessibleName( deviceInfo.deviceName() );
            checkboxAudioDevice->setObjectName( "checkboxAudioDevice" + deviceInfo.deviceName() );
            ui->verticalLayoutAudioDevices->addWidget( checkboxAudioDevice );
            checkboxAudioDevice->setAutoExclusive( true );
        }
    }
    QSpacerItem *verticalSpacerAudioDevices = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    ui->verticalLayoutAudioDevices->addSpacerItem( verticalSpacerAudioDevices );
}
