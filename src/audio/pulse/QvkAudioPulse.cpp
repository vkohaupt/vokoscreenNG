#include "QvkAudioPulse.h"

#include <QDebug>
#include <QAudioDeviceInfo>

QvkAudioPulse::QvkAudioPulse()
{}

QvkAudioPulse::QvkAudioPulse( Ui_MainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;

    vkThreadPulse = new QvkThreadPulse();
    connect( vkThreadPulse, SIGNAL( signal_countAudioDevices( int ) ), this, SLOT( slot_update( int ) ) );
    connect( ui->radioButtonPulse, SIGNAL( toggled( bool ) ), this, SLOT( slot_set_counter_back() ) );
    connect( ui->radioButtonPulse, SIGNAL( toggled( bool ) ), vkThreadPulse, SLOT( slot_set_first_start( bool ) ) );
    connect( ui->radioButtonPulse, SIGNAL( toggled( bool ) ), vkThreadPulse, SLOT( slot_start_stop_thread_timer( bool ) ) );
    // Pulse is Standard. If no pulsedevice found, change to alsa see QvkAudioPulse::getPulsesDevices()
    ui->radioButtonPulse->click();
}


QvkAudioPulse::~QvkAudioPulse()
{
}


void QvkAudioPulse::getPulseDevices()
{
    qDebug() << "QvkAudioPulse::getPulseDevices()";
    QvkPulseGstr vkPulseGstr;
    QStringList list = vkPulseGstr.get_all_Audio_devices();
    //qDebug() << "pulse StringList:" << list;
    if ( !list.empty() )
    {
        for ( int i = 0; i < list.count(); i++ )
        {
            QCheckBox *checkboxAudioDevice = new QCheckBox();
            checkboxAudioDevice->setText( QString( list.at(i) ).section( ":::", 1, 1 ) );
            checkboxAudioDevice->setAccessibleName( QString( list.at(i) ).section( ":::", 0, 0 ) );
            ui->verticalLayoutAudioDevices->addWidget( checkboxAudioDevice );
            //checkboxAudioDevice->setAutoExclusive( true );

            // Select default AudioDevice
            if ( QString( list.at(i) ).section( ":::", 0, 0 ) == QAudioDeviceInfo::defaultInputDevice().deviceName() )
            {
                checkboxAudioDevice->click();
            }
        }
        QSpacerItem *verticalSpacerAudioDevices = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
        ui->verticalLayoutAudioDevices->addSpacerItem( verticalSpacerAudioDevices );
    }
    else
    {
        qDebug() << "QvkAudioPulse::getPulseDevices() --> else";
        ui->radioButtonPulse->setEnabled( false );
        ui->radioButtonAlsa->click();
    }
}


void QvkAudioPulse::clearVerticalLayoutAudioDevices()
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


void QvkAudioPulse::slot_set_counter_back()
{
    counter = -1;
}


void QvkAudioPulse::slot_update( int count )
{

    if ( count != counter )
    {
        counter = count;
        clearVerticalLayoutAudioDevices();
        getPulseDevices();
    }
}
