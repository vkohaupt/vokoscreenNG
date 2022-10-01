#include "QvkSpezialSlider.h"

#include <QDebug>
#include <QFont>
#include <QApplication>
#include <QSlider>
#include <QStyle>

QvkSpezialSlider::QvkSpezialSlider( Qt::Orientation orientation )
{
    QFont font = qApp->font();
    QFontMetrics fontMetrics( font );
    fontWidth = fontMetrics.horizontalAdvance( QString::number( 123 ) );

    grooveHight = 6;
    grooveBorderRadius = 2;
    grooveBackgroundColor = QColor( Qt::gray ).name();
    grooveForgroundColor = "#3DAEE9";

    handleBorderColor = "#3DAEE9";
    handleBorderWith = 2;

    sliderHight = fontWidth + 2*handleBorderWith + 6;

    setOrientation( orientation );
    setFixedHeight( sliderHight );

    label = new QLabel( this );
    label->resize( sliderHight, sliderHight );
    label->setAlignment( Qt::AlignCenter );
    label->setStyleSheet( "QLabel{ color: black; background-color: none;} QLabel::disabled { color: lightGray; }" );

    connect( this, SIGNAL( valueChanged( int ) ), this, SLOT( slot_valueChanged( int ) ) );

    css_slider();
}


QvkSpezialSlider::~QvkSpezialSlider()
{
}


void QvkSpezialSlider::css_slider()
{
    QString slider;

    // The Widget
    slider.append( "QSlider {" );
    slider.append( "background-color: none;" );
    slider.append( "}" );

    // Line background
    slider.append( "QSlider::groove:horizontal {" );
    slider.append( "background-color: " + grooveBackgroundColor + ";" );
    slider.append( "height: " + QString::number( grooveHight ) + "px;" );
    slider.append( "border-radius: " + QString::number( grooveBorderRadius ) + "px;" );
    slider.append( "}" );

    // Line forground
    slider.append( "QSlider::sub-page:horizontal {" );
    slider.append( "background-color: " + grooveForgroundColor + ";" );
    slider.append( "border-radius: " + QString::number( grooveBorderRadius ) + "px;" );
    slider.append( "}" );

    // Handle big unpressed
    if ( bigHandel == true ) {
        slider.append( "QSlider::handle:horizontal {" );
        slider.append( "background-color: white;" );
        slider.append( "border-width: " + QString::number( handleBorderWith ) + "px;" );
        slider.append( "border-color: " + handleBorderColor + ";" );
        slider.append( "border-style: solid;" );
        slider.append( "width: " +  QString::number( fontWidth + 2*handleBorderWith ) + "px;" );
        slider.append( "height: " + QString::number( fontWidth + 2*handleBorderWith ) + "px;" );
        slider.append( "margin-top: " +    QString::number( -( fontWidth/2 + handleBorderWith-1 ) ) + "px;" );
        slider.append( "margin-bottom: " + QString::number( -( fontWidth/2 + handleBorderWith-1 ) ) + "px;" ); // minus ist gleich aus der Vertikalen Mitte
        slider.append( "border-radius: " + QString::number( fontWidth/2 + 2*handleBorderWith ) + "px;" );
        slider.append( "}" );
    }

    // Handle big pressed
    if ( bigHandel == true ) {
        slider.append( "QSlider::handle:horizontal:pressed {" );
        slider.append( "background-color: whitesmoke;" );
        slider.append( "border-width: " + QString::number( handleBorderWith ) + "px;" );
        slider.append( "border-color: " + handleBorderColor + ";" );
        slider.append( "border-style: solid;" );
        slider.append( "width: " +  QString::number( fontWidth + 2*handleBorderWith ) + "px;" );
        slider.append( "height: " + QString::number( fontWidth + 2*handleBorderWith ) + "px;" );
        slider.append( "margin-top: " +    QString::number( -( fontWidth/2 + handleBorderWith-1 ) ) + "px;" );
        slider.append( "margin-bottom: " + QString::number( -( fontWidth/2 + handleBorderWith-1 ) ) + "px;" ); // minus ist gleich aus der Vertikalen Mitte
        slider.append( "border-radius: " + QString::number( fontWidth/2 + 2*handleBorderWith ) + "px;" );
        slider.append( "}" );
    }


    // Handle mini
    if ( bigHandel == false ) {
        slider.append( "QSlider::handle:horizontal {" );
        slider.append( "background-color: white;" );
        slider.append( "border-width: " + QString::number( handleBorderWith ) + "px;" );
        slider.append( "border-color: " + handleBorderColor + ";" );
        slider.append( "border-style: solid;" );
        slider.append( "width: " + QString::number( 10 ) + "px;" ); // content
        slider.append( "height: " + QString::number( 10 ) + "px;" ); // content
        slider.append( "margin-top: " + QString::number( -( 5  ) ) + "px;" ); // width/2
        slider.append( "margin-bottom: " + QString::number( -( 5 ) ) + "px;" ); // width/2
        slider.append( "border-radius: " + QString::number( 7 ) + "px;" ); // width/2+border
        slider.append( "}" );
    }

    // Line disabled
    slider.append( "QSlider::sub-page:horizontal:disabled {" );
    slider.append( "background-color: lightgray;" );
    slider.append( "}" );

    // Handle disabled
    slider.append( "QSlider::handle:horizontal:disabled {" );
    slider.append( "border-color: lightgray;" );
    slider.append( "}" );

    setStyleSheet( slider );
}


void QvkSpezialSlider::slot_valueChanged( int value )
{
    if ( showValue == false ) {
        label->setText( "" );
    }

    if ( showValue == true ) {
        if ( decimalPoint == false ) {
            label->setText( QString::number( value ) );
        } else {
            qreal real = (qreal)value / 10;
            label->setText( QString::number( real, 'f', 1 ) );
        }
    }

    qreal distancePixel = QStyle::sliderPositionFromValue( minimum(), maximum(), value, width() - label->width() );
    int adjustmentPixel = 0;
    if ( value == minimum() ) {
        adjustmentPixel = -1;
    }
    label->move( distancePixel + adjustmentPixel, 0 );
}


void QvkSpezialSlider::resizeEvent( QResizeEvent *event )
{
    Q_UNUSED(event)
    slot_valueChanged( value() );
}


void QvkSpezialSlider::setShowValue( bool value )
{
    showValue = value;
    css_slider();
}


void QvkSpezialSlider::setBigHandel( bool value )
{
    bigHandel = value;
    css_slider();
}


void QvkSpezialSlider::setDecimalPoint( bool value )
{
    decimalPoint = value;
    css_slider();
}



/*
QvkSpezialSlider::QvkSpezialSlider( Qt::Orientation orientation )
{
    QFont font = qApp->font();
    fontSize = font.pointSize();

    sliderHight = 24 + fontSize;
    lineHight = 6;
    lineBorderRadius = 3;
    lineBackgroundColor = QColor( Qt::gray ).name();
    lineForgroundColor = "#3DAEE9";

    HandleBorderColor = "#3DAEE9";

    setOrientation( orientation );
    setFixedHeight( sliderHight );

    label = new QLabel( this );
    label->resize( 17 + fontSize, 17 + fontSize );
    label->setAlignment( Qt::AlignCenter );
    label->setStyleSheet( "QLabel{ color : black; } QLabel::disabled { color : lightGray; }" );

    connect( this, SIGNAL( valueChanged( int ) ), this, SLOT( slot_valueChanged( int ) ) );

    css_slider();
}


QvkSpezialSlider::~QvkSpezialSlider()
{
}


void QvkSpezialSlider::css_slider()
{
    QString slider;

    // The Widget
    slider.append( "QSlider {" );
    slider.append( "background-color: none;" );
    slider.append( "}" );

    // Line background
    slider.append( "QSlider::groove:horizontal {" );
    slider.append( "background-color: " + lineBackgroundColor + ";" );
    slider.append( "height: " + QString::number( lineHight ) + "px;" );
    slider.append( "border-radius: " + QString::number( lineBorderRadius ) + "px;" );
    slider.append( "}" );

    // Line forground
    slider.append( "QSlider::sub-page:horizontal {" );
    slider.append( "background-color: " + lineForgroundColor + ";" );
    slider.append( "border-radius: " + QString::number( lineBorderRadius ) + "px;" );
    slider.append( "}" );

    // Handle big
    if ( bigHandel == true ) {
        slider.append( "QSlider::handle:horizontal {" );
        slider.append( "background-color: white;" );
        slider.append( "border-width: 2px;" );
        slider.append( "border-color: " + HandleBorderColor + ";" );
        slider.append( "border-style: solid;" );
        slider.append( "width: " + QString::number( 17 + fontSize ) + "px;" );
        slider.append( "height: " + QString::number( 17 + fontSize ) + "px;" );
        slider.append( "margin-top: " + QString::number( -( lineHight + fontSize/2 + 2  ) ) + "px;" );
        slider.append( "margin-bottom: " + QString::number( -( lineHight + fontSize/2 + 2 ) ) + "px;" );
        slider.append( "border-radius: " + QString::number( sliderHight/2 - 2 ) + "px;" );
        slider.append( "}" );
    }

    // Handle mini
    if ( bigHandel == false ) {
        slider.append( "QSlider::handle:horizontal {" );
        slider.append( "background-color: white;" );
        slider.append( "border-width: 2px;" );
        slider.append( "border-color: " + HandleBorderColor + ";" );
        slider.append( "border-style: solid;" );
        slider.append( "width: " + QString::number( 10 ) + "px;" ); // content
        slider.append( "height: " + QString::number( 10 ) + "px;" ); // content
        slider.append( "margin-top: " + QString::number( -( 5  ) ) + "px;" ); // width/2
        slider.append( "margin-bottom: " + QString::number( -( 5 ) ) + "px;" ); // width/2
        slider.append( "border-radius: " + QString::number( 7 ) + "px;" ); // width/2+border
        slider.append( "}" );
    }

    // Line disabled
    slider.append( "QSlider::sub-page:horizontal:disabled {" );
    slider.append( "background-color: lightgray;" );
    slider.append( "}" );

    // Handle disabled
    slider.append( "QSlider::handle:horizontal:disabled {" );
    slider.append( "border-color: lightgray;" );
    slider.append( "}" );

    setStyleSheet( slider );
}


void QvkSpezialSlider::slot_valueChanged( int value )
{
    if ( showValue == false ) {
        label->setText( "" );
    }

    if ( showValue == true ) {
        if ( decimalPoint == false ) {
            label->setText( QString::number( value ) );
        } else {
            qreal real = (qreal)value / 10;
            label->setText( QString::number( real, 'f', 1 ) );
        }
    }

    qreal distancePixel = (qreal)( width() - (qreal)32 ) / ( (qreal)maximum() - (qreal)minimum() );
    label->move( ( value - minimum() ) * distancePixel + 2, 2 );
}


void QvkSpezialSlider::resizeEvent( QResizeEvent *event )
{
    Q_UNUSED(event)
    slot_valueChanged( value() );
}


void QvkSpezialSlider::setShowValue( bool value )
{
    showValue = value;
    css_slider();
}


void QvkSpezialSlider::setBigHandel( bool value )
{
    bigHandel = value;
    css_slider();
}


void QvkSpezialSlider::setDecimalPoint( bool value )
{
    decimalPoint = value;
    css_slider();
}
*/
