
#include "QvkSpezialCheckbox.h"

#include <QDebug>

QvkSpezialCheckbox::QvkSpezialCheckbox( QWidget *parent ) : QWidget( parent )
{
    setObjectName( "spezialCheckBox" );
    setMinimumWidth( 100 );
    setMinimumHeight( 34 );
    setMaximumWidth( 100 );

    connect( this, SIGNAL( signal_clicked( bool ) ), this, SLOT( slot_setChecked( bool ) ) );
}


bool QvkSpezialCheckbox::isChecked()
{
    return checked;
}


void QvkSpezialCheckbox::slot_setChecked( bool value )
{
     checked = value;
     repaint();
}


void QvkSpezialCheckbox::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    if ( checked == true )
    {
        checked = false;
    }
    else
    {
        checked = true;
    }

    repaint();

    emit signal_clicked( checked );
}


void QvkSpezialCheckbox::paintEvent( QPaintEvent *event )
{
    Q_UNUSED( event );
    QPainter painter( this );
    painter.setRenderHints( QPainter::Antialiasing, true );

    button_x = 2;
    button_y = 2;
    button_width = width() - 2 * button_x;
    button_height = height() - 2 * button_y;
    penWidth = 1;
    radius = 11;
    margin = 3;

    if ( isChecked() == false )
    {
        paintUnChecked( painter );
    }

    if ( isChecked() == true )
    {
        paintChecked( painter );
    }
}

void QvkSpezialCheckbox::paintChecked( QPainter &painter )
{
    QPen pen;
    pen.setColor( Qt::black );
    pen.setWidthF( penWidth );
    painter.setPen( pen );
    QBrush brushButton( Qt::darkGreen );
    painter.setBrush( brushButton );
    QRectF rectButton( button_x, button_y, button_width, button_height );
    painter.setBrush( brushButton );
    QPainterPath painterPathButton;
    painterPathButton.addRoundedRect( rectButton, radius, radius, Qt::AbsoluteSize );
    painter.drawPath( painterPathButton );

    QPen penSlider;
    penSlider.setColor( Qt::black );
    painter.setPen( penSlider );
    qreal slider_x = ( button_width / 3 ) + margin;
    qreal slider_y = button_y + margin;
    qreal slider_width = ( button_width / 3 * 2 ) - ( 2 * margin );
    qreal slider_height = button_height - ( 2 * margin );
    QRectF rectSlider( slider_x, slider_y, slider_width, slider_height );
    QBrush brushSlider( Qt::lightGray );
    painter.setBrush( brushSlider );
    QPainterPath painterPathSlider;
    painterPathSlider.addRoundedRect( rectSlider, radius - margin, radius - margin, Qt::AbsoluteSize );
    painter.drawPath( painterPathSlider );

    painter.setPen( Qt::NoPen );
    QBrush brushRibs( Qt::darkGray );
    painter.setBrush( brushRibs );
    int step = 5;
    for ( int x = 0; x < 10; x++ )
    {
       int value = step * x;

       painter.drawRect( slider_x + radius/2 + value,
                         slider_y + margin,
                         3,
                         slider_height - 2 * margin);
    }

    QPixmap pixmap( ":/spezialCheckBox/on.png" );
    pixmap.setDevicePixelRatio( devicePixelRatioF() );
    pixmap = pixmap.scaled( slider_height / 5 * 3 * devicePixelRatioF(), slider_height / 5 * 3 * devicePixelRatioF(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    painter.drawPixmap( button_x + margin + ( ( slider_x - button_x - margin ) / 2 ) - ( pixmap.width() / 2 / devicePixelRatioF() ),
                        button_y + margin + ( slider_height / 5 ),
                        pixmap );
}


void QvkSpezialCheckbox::paintUnChecked( QPainter &painter )
{
    QPen pen;
    pen.setWidthF( penWidth );
    pen.setColor( Qt::black );
    painter.setPen( pen );
    QBrush brushButton( Qt::red );
    painter.setBrush( brushButton );
    QRectF rectButton( button_x, button_y, button_width, button_height );
    painter.setBrush( brushButton );
    QPainterPath painterPathButton;
    painterPathButton.addRoundedRect( rectButton, radius, radius, Qt::AbsoluteSize );
    painter.drawPath( painterPathButton );

    QPen penSlider;
    penSlider.setColor( Qt::black );
    painter.setPen( penSlider );
    qreal slider_x = button_x + margin;
    qreal slider_y = button_y + margin;
    qreal slider_width = ( button_width / 3 * 2 )  - ( 2 * margin );
    qreal slider_height = button_height - ( 2 * margin );
    QRectF rectSlider( slider_x, slider_y, slider_width, slider_height );
    QBrush brushSlider( Qt::lightGray );
    painter.setBrush( brushSlider );
    QPainterPath painterPathSlider;
    painterPathSlider.addRoundedRect( rectSlider, radius - margin, radius - margin, Qt::AbsoluteSize );
    painter.drawPath( painterPathSlider );

    painter.setPen( Qt::NoPen );
    QBrush brushRibs( Qt::darkGray );
    painter.setBrush( brushRibs );
    int step = 5;
    for ( int x = 0; x < 10; x++ )
    {
       int value = step * x;

       painter.drawRect( slider_x + radius/2 + value,
                         slider_y + margin,
                         3,
                         slider_height - 2 * margin);
    }

    QPixmap pixmap( ":/spezialCheckBox/off.png" );
    pixmap.setDevicePixelRatio(  devicePixelRatioF() );
    pixmap = pixmap.scaled( slider_height / 5 * 3 * devicePixelRatioF(),
                            slider_height / 5 * 3 * devicePixelRatioF(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    painter.drawPixmap( slider_x+slider_width + ( button_width - slider_x - slider_width) / 2 - pixmap.width() / 2 / devicePixelRatioF(),
                        button_y + margin + ( slider_height / 5 ),
                        pixmap );
}
