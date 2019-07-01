#include "QvkSpezialSlider.h"

#include <QPaintEvent>
#include <QDebug>
#include <QPainter>
#include <QGuiApplication>
#include <QSlider>

QvkSpezialSlider::QvkSpezialSlider( Qt::Orientation orientation)
{
    setOrientation( orientation );
    setMinimumHeight( 26 ); // Sliderhöhe
    radius = 4.0; // Radius from begin and end off line
    lineHight = 6.0;
    show();
}

QvkSpezialSlider::~QvkSpezialSlider()
{
}


QColor QvkSpezialSlider::vk_get_color_highlight()
{
    QPalette palette = QGuiApplication::palette();
    QColor color( palette.highlight().color().red(),
                  palette.highlight().color().green(),
                  palette.highlight().color().blue() );
    return color;
}


QColor QvkSpezialSlider::vk_get_color_windowtext()
{
    QPalette palette = QGuiApplication::palette();
    QColor color( palette.windowText().color().red(),
                  palette.windowText().color().green(),
                  palette.windowText().color().blue() );
    return color;
}


QColor QvkSpezialSlider::vk_get_color_button()
{
    QPalette palette = QGuiApplication::palette();
    QColor color( palette.button().color().red(),
                  palette.button().color().green(),
                  palette.button().color().blue() );
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

    pen.setStyle( Qt::NoPen );
    painter.setPen( pen );

    QBrush brush;
    brush.setStyle( Qt::SolidPattern );

    // Background from line
    brush.setColor( Qt::lightGray );
    painter.setBrush( brush );
    painter.drawRoundedRect( 0,
                             distance,
                             width(),
                             height() - 2*distance,
                             radius,
                             radius,
                             Qt::AbsoluteSize );

    // Foreground from line
    brush.setColor( vk_get_color_highlight() );
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
    pen.setColor( vk_get_color_highlight() );
    pen.setWidthF( 2.0 );
    painter.setPen( pen );

    qreal onePixel = 0;
    if ( value() == minimum() )
    {
        onePixel = 1;
    }
    if ( value() == maximum() )
    {
        onePixel = -1;
    }
    brush.setColor( vk_get_color_button() );
    painter.setBrush( brush );
    handleRadius = handleRadius - pen.widthF()/2;
    painter.drawEllipse( QRectF( (qreal)(width() - 2*handleRadius) / (qreal)( maximum() - minimum() ) * (qreal)( value() - minimum() ) + onePixel,
                                 pen.widthF()/2,
                                 2*handleRadius,
                                 2*handleRadius ) );

    // Handletext
    QFont font;
    font.setPointSizeF( handleRadius - pen.widthF() );
    QFontMetrics fontMetrics( font );
    qreal pixelWidth = fontMetrics.width( QString::number( value() ) );
    painter.setFont( font );
    painter.setPen( vk_get_color_windowtext() );
    painter.drawText( (qreal)(width() - 2*handleRadius) / ( (qreal)maximum() - (qreal)minimum() ) * ( (qreal)value() - minimum() ) + ( handleRadius - pixelWidth/2 ) + onePixel,
                      handleRadius * 1.5,
                      QString::number( value() ) );

    painter.end();
}

void QvkSpezialSlider::mousePressEvent( QMouseEvent *event )
{
    if( event->button() != Qt::LeftButton)
    {
        return;
    }

    // Press on Handle
    QRect rectHandle( (qreal)(width() - 2*handleRadius) / ( (qreal)maximum() - (qreal)minimum() ) * ( (qreal)value() - minimum() ) + (qreal)( pen.widthF() / 2.0 ),
                      pen.widthF()/2.0,
                      2*handleRadius,
                      2*handleRadius );

    if ( rectHandle.contains( event->pos() ) )
    {
        mousePressed = true;
        return;
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
            int couldValue = ( event->localPos().x() / stepSizeInPixels ) + minimum() + 1;
            setValue( couldValue );
            return;
        }

        if ( event->localPos().x() < ( stepCurrent * stepSizeInPixels ) - (qreal)( stepSizeInPixels / 2 ) )
        {
            int couldValue = ( event->localPos().x() / stepSizeInPixels ) + minimum();
            setValue( couldValue );
            return;
        }
    }
}


void QvkSpezialSlider::mouseReleaseEvent( QMouseEvent *event )
{
    Q_UNUSED(event);
    mousePressed = false;
}
