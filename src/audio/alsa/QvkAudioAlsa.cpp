#include "QvkAudioAlsa.h"

#include <QAudioDeviceInfo>
#include <QDebug>

QvkAudioAlsa::QvkAudioAlsa( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow )
{
    connect( mainWindow, SIGNAL( destroyed( QObject* ) ), this, SLOT( slot_cleanup() ) );

    ui = ui_mainwindow;

    vkThreadAlsa = new QvkThreadAlsa();
    connect( vkThreadAlsa, SIGNAL( signal_countAudioDevices( int ) ), this, SLOT( slot_update( int ) ) );
    connect( ui->radioButtonAlsa, SIGNAL( toggled( bool ) ), this, SLOT( slot_set_counter() ) );
    connect( ui->radioButtonAlsa, SIGNAL( toggled( bool ) ), vkThreadAlsa, SLOT( slot_set_first_start( bool ) ) );
    connect( ui->radioButtonAlsa, SIGNAL( toggled( bool ) ), vkThreadAlsa, SLOT( slot_start_stop_thread_timer( bool ) ) );

    connect( ui->pushButtonStart,  SIGNAL( clicked( bool ) ), vkThreadAlsa, SLOT( slot_stop_thread_timer() ) );
    connect( ui->pushButtonStop,   SIGNAL( clicked( bool ) ), this, SLOT( slot_start_thread_timer() ) );
}


QvkAudioAlsa::~QvkAudioAlsa()
{
}


void QvkAudioAlsa::slot_start_thread_timer()
{
    if ( ui->radioButtonAlsa->isChecked() == true )
    {
        vkThreadAlsa->slot_start_thread_timer();
    }
}


void QvkAudioAlsa::slot_cleanup()
{
    vkThreadAlsa->timer->stop();
}


void QvkAudioAlsa::getAlsaDevices()
{
    foreach ( const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices( QAudio::AudioInput ) )
    {
        if ( ( deviceInfo.deviceName().contains("alsa") == false ) and ( deviceInfo.deviceName() != "" ) )
        {
            qDebug().noquote() << "[vokoscreen] Alsa device:" << deviceInfo.deviceName();
            QCheckBox *checkboxAudioDevice = new QCheckBox();
            checkboxAudioDevice->setText( deviceInfo.deviceName() );
            checkboxAudioDevice->setAccessibleName( deviceInfo.deviceName() );
            checkboxAudioDevice->setObjectName( "checkboxAudioDevice" + deviceInfo.deviceName() );
            ui->verticalLayoutAudioDevices->addWidget( checkboxAudioDevice );
            checkboxAudioDevice->setAutoExclusive( true );
        }
    }
    qDebug().noquote();
    QSpacerItem *verticalSpacerAudioDevices = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    ui->verticalLayoutAudioDevices->addSpacerItem( verticalSpacerAudioDevices );

    // Select first device
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QCheckBox *>();
    listQCheckBox.at(0)->click();
}


void QvkAudioAlsa::clearVerticalLayoutAudioDevices()
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


void QvkAudioAlsa::slot_set_counter()
{
    counter = 0;
}


void QvkAudioAlsa::slot_update( int count )
{
    if ( count != counter )
    {
        counter = count;
        clearVerticalLayoutAudioDevices();
        getAlsaDevices();
    }
}
