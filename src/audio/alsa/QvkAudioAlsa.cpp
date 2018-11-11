#include "QvkAudioAlsa.h"

#include <QAudioDeviceInfo>
#include <QDebug>

QvkAudioAlsa::QvkAudioAlsa( Ui_MainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;

    vkThreadAlsa = new QvkThreadAlsa();
    connect( vkThreadAlsa, SIGNAL( signal_countAudioDevices( int ) ), this, SLOT( slot_update( int ) ) );
}


QvkAudioAlsa::~QvkAudioAlsa()
{
}


void QvkAudioAlsa::slot_getAlsaDevices()
{
    foreach ( const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices( QAudio::AudioInput ) )
    {
        if ( ( deviceInfo.deviceName().contains("alsa") == false ) and ( deviceInfo.deviceName() != "" ) )
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

    // Select first device
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QCheckBox *>();
    listQCheckBox.at(0)->click();
}


void QvkAudioAlsa::slot_clearVerticalLayoutAudioDevices()
{
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QCheckBox *>();
    for ( int i = 0; i < listQCheckBox.count(); i++ )
    {
       ui->verticalLayoutAudioDevices->removeWidget( listQCheckBox.at(i) );
       delete listQCheckBox.at(i);
    }

    for ( int i = 0; i < ui->verticalLayoutAudioDevices->count(); ++i )
    {
        QLayoutItem *layoutItem = ui->verticalLayoutAudioDevices->itemAt(i);
        if ( layoutItem->spacerItem() )
        {
            ui->verticalLayoutAudioDevices->removeItem( layoutItem );
            delete layoutItem;
        }
    }
}


void QvkAudioAlsa::slot_start( bool value )
{
    if ( value == true )
    {
        counter = 0;
        slot_clearVerticalLayoutAudioDevices();
        slot_getAlsaDevices();
        vkThreadAlsa->slot_startThread( true );
    }
    else
    {
        vkThreadAlsa->slot_startThread( false );
    }
}


void QvkAudioAlsa::slot_update( int count )
{
    if ( count != counter )
    {
        counter = count;
        slot_clearVerticalLayoutAudioDevices();
        slot_getAlsaDevices();
    }
}
