#include "QvkThreadAlsa.h"

#include <QDebug>
#include <QAudioDeviceInfo>

QvkThreadAlsa::QvkThreadAlsa()
{
    timer = new QTimer( this );
    timer->setInterval( 2000 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_trigger_Thread() ) );
}


QvkThreadAlsa::~QvkThreadAlsa()
{
}


void QvkThreadAlsa::run()
{
    emit signal_countAudioDevices( QAudioDeviceInfo::availableDevices( QAudio::AudioInput ).count() );
}


void QvkThreadAlsa::slot_trigger_Thread()
{
    start();
}


void QvkThreadAlsa::slot_set_first_start( bool value )
{
    firstStart = value;
}


void QvkThreadAlsa::slot_start_stop_thread_timer( bool value )
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
void QvkThreadAlsa::slot_start_thread_timer()
{
    timer->start();
}


void QvkThreadAlsa::slot_stop_thread_timer()
{
    timer->stop();
}
