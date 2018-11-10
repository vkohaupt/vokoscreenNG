#include "QvkThreadPulse.h"

#include <QDebug>
#include <QAudioDeviceInfo>

QvkThreadPulse::QvkThreadPulse()
{
}


QvkThreadPulse::~QvkThreadPulse()
{
}


void QvkThreadPulse::run()
{
    while ( running == true )
    {
        msleep( 5000 );
        emit signal_countAudioDevices( QAudioDeviceInfo::availableDevices( QAudio::AudioInput ).count() );
    }
}


void QvkThreadPulse::slot_startThread(bool value)
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
