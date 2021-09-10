#include "QvkSpezialSlider.h"

#include <QPaintEvent>
#include <QDebug>
#include <QPainter>
#include <QGuiApplication>
#include <QSlider>

QvkSpezialSlider::QvkSpezialSlider( Qt::Orientation orientation)
{
    setOrientation( orientation );
    radius = 4.0; // Radius from begin and end off line
    lineHight = 6.0;
}

QvkSpezialSlider::~QvkSpezialSlider()
{
}


QColor QvkSpezialSlider::vk_get_color( enum QPalette::ColorRole colorRole )
{
    QColor color;
    if ( isEnabled() == true )
    {
       QPalette palette = QGuiApplication::palette();
       color = palette.color( QPalette::Active, colorRole );
    }

    if ( isEnabled() == false )
    {
       QPalette palette = QGuiApplication::palette();
       color = palette.color( QPalette::Disabled, colorRole );
    }

    return color;
}


void QvkSpezialSlider::paintEvent(QPaintEvent *event)
{
    (void)event;
    distance = ( height() - lineHight ) / 2.0; // Distance from top or bottom to line
    handleRadius = height() / 2.0;

    QPainter painter;
    painter.begin( this );
    painter.setRenderHints( QPainter::Antialiasing, true );

    // Background from line
    pen.setStyle( Qt::NoPen );
    painter.setPen( pen );

    QBrush brush;
    brush.setStyle( Qt::SolidPattern );
    brush.setColor( vk_get_color( QPalette::Mid ) );
    painter.setBrush( brush );

    painter.drawRoundedRect( 0,
                             distance,
                             width(),
                             height() - 2*distance,
                             radius,
                             radius,
                             Qt::AbsoluteSize );

    // Foreground from line
    brush.setColor( vk_get_color( QPalette::Highlight) );
    painter.setBrush( brush );
    painter.drawRoundedRect( 0,
                             distance,
                             (qreal)(width() - handleRadius) / ( (qreal)maximum() - (qreal)minimum() ) * ( (qreal)value() - minimum() ) + (qreal)( pen.widthF() / 2.0 ),
                             height() - 2*distance,
                             radius,
                             radius,
                             Qt::AbsoluteSize );

    // Handle
    pen.setStyle( Qt::SolidLine );
    pen.setColor( vk_get_color( QPalette::Highlight ) );
    pen.setWidthF( 2.0 );
    painter.setPen( pen );

    brush.setColor( vk_get_color( QPalette::Button ) );
    painter.setBrush( brush );

    qreal onePixel = 0;
    if ( value() == minimum() )
    {
        onePixel = 1;
    }
    if ( value() == maximum() )
    {
        onePixel = -1;
    }

    handleRadius = handleRadius - pen.widthF()/2;
    QRectF qRectF( (qreal)(width() - 2*handleRadius) / (qreal)( maximum() - minimum() ) * (qreal)( value() - minimum() ) + onePixel,
                   pen.widthF()/2,
                   2*handleRadius,
                   2*handleRadius );

    painter.drawEllipse( qRectF );

    // Handletext
    if ( showValue == true )
    {
        QFont font = qApp->font();
        painter.setFont( font );
        QFontMetrics fontMetrics( font );
        setMinimumHeight( fontMetrics.width( "5555" ) );
        painter.setPen( vk_get_color( QPalette::ButtonText ) );
        painter.drawText( qRectF, Qt::AlignCenter, QString::number( value() ) );
    }

    painter.end();
}


void QvkSpezialSlider::mousePressEvent( QMouseEvent *event )
{
    if( event->button() != Qt::LeftButton)
    {
        return;
    }

    // Press on Handle
    QRectF rectHandle( (qreal)(width() - 2*handleRadius) / ( (qreal)maximum() - (qreal)minimum() ) * ( (qreal)value() - minimum() ) + (qreal)( pen.widthF() / 2.0 ),
                       pen.widthF()/2.0,
                       2*handleRadius,
                       2*handleRadius );

    if ( rectHandle.contains( event->pos() ) )
    {
        mousePressed = true;
        return;
    }

    // Click on line
    if ( rectHandle.contains( event->pos() ) == false )
    {
        qreal myValue = 0;
        qreal myX = event->x();
        qreal myWidth = width();
        qreal myMinimum = minimum();
        qreal myMaximum = maximum();
        qreal distancePixel = myWidth / ( myMaximum - myMinimum );
        if ( event->button() == Qt::LeftButton )
        {
            if ( myX <= distancePixel / 2 )
            {
                myValue = myMinimum;
            }

            if ( myX >= ( distancePixel * ( myMaximum - myMinimum ) - distancePixel / 2 ) )
            {
                myValue = myMaximum;
            }

            if ( ( myX >= distancePixel / 2 ) and ( myX <= ( distancePixel * ( myMaximum - myMinimum ) - distancePixel / 2 ) ) )
            {
                myValue = qRound( myMinimum + ( myX / distancePixel ) );
            }

            setValue( myValue );
            emit sliderMoved( myValue );
        }
        event->accept();
    }
}


void QvkSpezialSlider::mouseMoveEvent( QMouseEvent *event )
{
    if ( mousePressed == true )
    {
        qreal stepSizeInPixels = (qreal)( (qreal)width() / (qreal)( maximum() - minimum() ) );
        qreal stepCurrent = value() - minimum();

        if ( event->localPos().x() > ( stepCurrent * stepSizeInPixels ) + (qreal)( stepSizeInPixels / 2 ) )
        {
            qreal couldValue = ( event->localPos().x() / stepSizeInPixels ) + minimum() + 1;
            setValue( static_cast<int>(couldValue) );
            emit sliderMoved( static_cast<int>(couldValue) );
            return;
        }

        if ( event->localPos().x() < ( stepCurrent * stepSizeInPixels ) - (qreal)( stepSizeInPixels / 2 ) )
        {
            qreal couldValue = ( event->localPos().x() / stepSizeInPixels ) + minimum();
            setValue( static_cast<int>(couldValue) );
            emit sliderMoved( static_cast<int>(couldValue) );
            return;
        }
    }
}


void QvkSpezialSlider::mouseReleaseEvent( QMouseEvent *event )
{
    Q_UNUSED(event);
    mousePressed = false;
}

/*
 * Show value on handle.
 * The value default is true.
 */
void QvkSpezialSlider::setShowValue( bool value )
{
    showValue = value;
}
