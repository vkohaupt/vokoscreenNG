#include "QvkAudioPulse.h"

#include <QDebug>
#include <QAudioDeviceInfo>

QvkAudioPulse::QvkAudioPulse()
{}


QvkAudioPulse::QvkAudioPulse( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow )
{
    connect( mainWindow, SIGNAL( destroyed( QObject* ) ), this, SLOT( slot_cleanup() ) );
    ui = ui_mainwindow;

    vkWatcherPulse = new QvkWatcherPulse();
    connect( vkWatcherPulse, SIGNAL( signal_countAudioDevices( int ) ), this, SLOT( slot_update( int ) ) );
    connect( ui->radioButtonPulse, SIGNAL( toggled( bool ) ), this, SLOT( slot_set_counter_back() ) );
    connect( ui->radioButtonPulse, SIGNAL( toggled( bool ) ), vkWatcherPulse, SLOT( slot_set_first_start( bool ) ) );
    connect( ui->radioButtonPulse, SIGNAL( toggled( bool ) ), vkWatcherPulse, SLOT( slot_start_stop_thread_timer( bool ) ) );

    connect( ui->pushButtonStart,  SIGNAL( clicked( bool ) ), vkWatcherPulse, SLOT( slot_stop_thread_timer() ) );
    connect( ui->pushButtonStop,   SIGNAL( clicked( bool ) ), this, SLOT( slot_start_thread_timer() ) );

    ui->radioButtonPulse->click();
    ui->radioButtonPulse->hide();
}


QvkAudioPulse::~QvkAudioPulse()
{
}


void QvkAudioPulse::slot_start_thread_timer()
{
    if ( ui->radioButtonPulse->isChecked() == true  )
    {
       vkWatcherPulse->slot_start_thread_timer();
    }
}


void QvkAudioPulse::slot_cleanup()
{
   vkWatcherPulse->stop();
}


void QvkAudioPulse::getPulseDevices()
{
    QvkPulseGstr vkPulseGstr;
    QStringList list;
    list << vkPulseGstr.get_all_Audio_devices();
    if ( !list.empty() == true )
    {
        QString string = tr( "Special device(See help)" );
        list.prepend( "pulsesrc:::" + string );
    }

    for ( int i = 0; i < list.count(); i++ )
    {
        qDebug().noquote() << "[vokoscreen] PulseAudio device:" << list.at(i);
    }
    qDebug().noquote();

    if ( !list.empty() )
    {
        for ( int i = 0; i < list.count(); i++ )
        {
            QCheckBox *checkboxAudioDevice = new QCheckBox();
            checkboxAudioDevice->setText( QString( list.at(i) ).section( ":::", 1, 1 ) );
            checkboxAudioDevice->setAccessibleName( QString( list.at(i) ).section( ":::", 0, 0 ) );
            ui->verticalLayoutAudioDevices->addWidget( checkboxAudioDevice );
            checkboxAudioDevice->setAutoExclusive( true );

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
        ui->radioButtonPulse->setEnabled( false );
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
