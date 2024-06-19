
#include "QvkSpezialProgressBarAudio.h"

#include <QDebug>


QvkSpezialProgressBarAudio::QvkSpezialProgressBarAudio( QProgressBar *parent ) : QProgressBar( parent )
{
    setObjectName( "spezialProgressBarAudio" );
}


void QvkSpezialProgressBarAudio::paintEvent( QPaintEvent *event )
{
    Q_UNUSED( event );
    QPainter painter( this );
    painter.setRenderHint( QPainter::Antialiasing, true );
    painter.setRenderHint( QPainter::SmoothPixmapTransform, true );
}

