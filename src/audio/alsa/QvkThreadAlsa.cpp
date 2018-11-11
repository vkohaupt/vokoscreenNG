#include "QvkThreadAlsa.h"

#include <QDebug>
#include <QAudioDeviceInfo>

QvkThreadAlsa::QvkThreadAlsa()
{
}


QvkThreadAlsa::~QvkThreadAlsa()
{
}


void QvkThreadAlsa::run()
{
    while ( running == true )
    {
        msleep( 5000 );
        emit signal_countAudioDevices( QAudioDeviceInfo::availableDevices( QAudio::AudioInput ).count() );
    }
}


void QvkThreadAlsa::slot_startThread(bool value)
{
    if ( value == true )
    {
        running = true;
        start();
    }
    else
    {
        running = false;
    }
}
