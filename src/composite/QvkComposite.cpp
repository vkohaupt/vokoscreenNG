#include "QvkComposite.h"
#include "global.h"

#include <QPainter>
#include <QDebug>
#include <QScreen>
#include <QTimer>
#include <QGuiApplication>

QvkComposite::QvkComposite( QMainWindow *parent )
{
    Q_UNUSED( parent );
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip );
    setAttribute( Qt::WA_TranslucentBackground, true );

    resize( 5, 5 );
    move( 0, 0 );
    show();

    QTimer::singleShot( 1000, Qt::PreciseTimer, this, SLOT( slot_shot() ) );
}


void QvkComposite::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event)

    QPixmap pixmap( width(), height() );
    pixmap.fill( Qt::transparent );

    QPainter painter;
    painter.begin( this );
    painter.drawPixmap( 0, 0, pixmap );
    painter.end();
}


void QvkComposite::slot_shot()
{
    QImage image = screen()->grabWindow(0).toImage();

    QColor colorBlack = image.pixelColor( 3, 3 );
    if ( colorBlack == Qt::black ) {
        isCompositeEnabled = false;
    }

    emit signal_compositeEnabled( isCompositeEnabled );
    qDebug();
    qDebug().noquote() << global::nameOutput << "[Composite]" << isCompositeEnabled;
    close();
}
