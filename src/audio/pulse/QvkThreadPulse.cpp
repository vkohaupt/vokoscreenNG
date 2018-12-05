#include "QvkThreadPulse.h"

#include <QDebug>
#include <QAudioDeviceInfo>

QvkThreadPulse::QvkThreadPulse()
{
    timer = new QTimer( this );
    timer->setInterval( 5000 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_trigger_Thread() ) );
}


QvkThreadPulse::~QvkThreadPulse()
{
}


void QvkThreadPulse::run()
{
    int count = 0;
    foreach ( const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices( QAudio::AudioInput ) )
    {
        if ( ( deviceInfo.deviceName().contains("alsa") == true ) and ( deviceInfo.deviceName() != "" ) )
        {
            count++;
        }
    }
    emit signal_countAudioDevices( count );
}


void QvkThreadPulse::slot_trigger_Thread()
{
    start();
}


void QvkThreadPulse::slot_set_first_start( bool value )
{
    firstStart = value;
}


/*
 * Course:
 * -vokoscreen starts
 * -In QvkAudioPulse::QvkAudioPulse becomes radioButtonPulse a click and audio devices shows immediately
 * -Then the timer starts and show to new or remove devices
 */
void QvkThreadPulse::slot_start_stop_thread_timer( bool value )
{

  if ( firstStart == true )
  {
      firstStart = false;
      start();
      timer->start();
  }
  else
  {
    if ( value == true )
        timer->start();
    else
        timer->stop();
  }
}

/*
 *
 */
void QvkThreadPulse::slot_start_thread_timer()
{
    timer->start();
}


void QvkThreadPulse::slot_stop_thread_timer()
{
    timer->stop();
}
