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
    painter.begin( &pixmap );
    painter.setRenderHints( QPainter::Antialiasing, true );

    QPainter painter_1;
    painter_1.begin( this );
    painter_1.drawPixmap( 0, 0, pixmap );
    painter_1.end();
}


void QvkComposite::slot_shot()
{
    QImage image = screen()->grabWindow(0).toImage();
    image.save( "/home/vk/test.png" );

    QColor colorBlack = image.pixelColor( 3, 3 );
    if ( colorBlack == Qt::black ) {
        isCompositeEnabled = false;
    }

    emit signal_compositeEnabled( isCompositeEnabled );
    qDebug().noquote() << global::nameOutput << "[Composite]" << isCompositeEnabled;
    close();
}
