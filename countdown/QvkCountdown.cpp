#include "QvkCountdown.h"
#include <QDesktopWidget>
#include <QTimer>
#include <QApplication>

QvkCountdown::QvkCountdown()
{
    QDesktopWidget *desk = QApplication::desktop();
    Width = 300;
    Height = 300;;
    x = ( desk->screenGeometry().width() / 2 ) - ( Width / 2 );
    y = ( desk->screenGeometry().height() / 2 ) -( Height / 2 );

    // Die Optionen Qt::Tool sollte nicht angewendet werden da bei Auswahl eines Fenster der Countdown nicht angezeigt wird.
    // Die Option Qt::ToolTip wäre eine möglichkeit, gefällt mir aber Optisch nicht da ein Rahmen angezeigt wird.
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    setAttribute( Qt::WA_TranslucentBackground, true );

    timer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( updateTimer() ) );

    animationTimer = new QTimer( this );
    connect( animationTimer, SIGNAL( timeout() ), this, SLOT( updateAnimationTimer() ) );

    hide();
}


void QvkCountdown::startCountdown( int value )
{
    setGeometry( x, y, Width, Height );
    show();
    countValue = value;
    gradValue = 0;

    timer->start( 1000 );
    animationTimer->start( 25 );
    emit signal_countdownBegin( true );
}


QvkCountdown::~QvkCountdown()
{
}


void QvkCountdown::updateTimer()
{
  gradValue = 0;
  countValue--;

  if ( countValue == 0 )
  {
    setGeometry( x, y, 1, 1 );
    hide();
    timer->stop();
    animationTimer->stop();
    emit signal_countDownfinish( true );
  }
}

void QvkCountdown::updateAnimationTimer()
{
  gradValue = gradValue - 20;
  update();
}


void QvkCountdown::paintEvent( QPaintEvent *event )
{
  (void)event;
  painter.begin( this );
    painter.setRenderHints( QPainter::Antialiasing, true );
    QPen pen;
    QBrush brush;
    
    brush.setColor( Qt::darkGray );
    brush.setStyle( Qt::SolidPattern );
    pen.setWidth( 0 );
    pen.setColor( Qt::darkGray );
    painter.setBrush( brush );
    painter.setPen( pen );
    painter.setOpacity( 0.3 );
    painter.drawPie( 0, 0, 300, 300, 90*16, gradValue*16 );

    painter.setOpacity( 1.0 );
    pen.setColor( Qt::darkGray );
    pen.setWidth( 6 );
    painter.setPen( pen );
    brush.setStyle( Qt::NoBrush );
    painter.setBrush( brush );
    painter.drawEllipse( QPoint( width()/2, height()/2), 125-3, 125-3 );
    painter.drawEllipse( QPoint( width()/2, height()/2), 100, 100 );
    painter.drawLine( 0, height()/2, width(), height()/2 );
    painter.drawLine( width()/2, 0, width()/2, height() );
    
    int fontSize = 110;
    QFont font;
    font.setPointSize( fontSize );
    painter.setFont( font );
    painter.setPen( Qt::red );
    QFontMetrics fontMetrics( font );
    int fontWidth = fontMetrics.width( QString::number( countValue ) );
    painter.drawText( width()/2-fontWidth/2, height()/2+fontSize/2, QString::number( countValue ) );
  painter.end();
}
