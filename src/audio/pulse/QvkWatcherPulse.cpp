#include "QvkWatcherPulse.h"
#include <QDebug>

QvkWatcherPulse::QvkWatcherPulse()
{
    timer = new QTimer( this );
    timer->setInterval( 2000 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_trigger() ) );
}


QvkWatcherPulse::~QvkWatcherPulse()
{
}


void QvkWatcherPulse::slot_trigger()
{
    QvkPulseGstr vkPulseGstr;
    QStringList list = vkPulseGstr.get_all_Audio_devices();
    emit signal_countAudioDevices( list.count() );
}


void QvkWatcherPulse::slot_set_first_start( bool value )
{
    firstStart = value;
}


/*
 * Course:
 * -vokoscreen starts
 * -In QvkAudioPulse::QvkAudioPulse becomes radioButtonPulse a click and audio devices shows immediately
 * -Then the timer starts and show to new or remove devices
 */
void QvkWatcherPulse::slot_start_stop_thread_timer( bool value )
{
  if ( firstStart == true )
  {
      firstStart = false;
      slot_trigger();
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


void QvkWatcherPulse::slot_start_thread_timer()
{
    timer->start();
}


void QvkWatcherPulse::slot_stop_thread_timer()
{
    timer->stop();
}
