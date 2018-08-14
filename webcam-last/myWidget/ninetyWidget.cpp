#include "ninetyWidget.h"

ninetyWidget::ninetyWidget( QWidget *parent ) : QWidget( parent )
{
  ninetyWidgetTop = new circleWidget( this );
  connect( ninetyWidgetTop, SIGNAL( clicked( bool ) ), SLOT( switchTop() ) );
  
  ninetyWidgetRight = new circleWidget( this );
  connect( ninetyWidgetRight, SIGNAL( clicked( bool ) ), SLOT( switchRight() ) );
  
  ninetyWidgetBottom = new circleWidget( this );
  connect( ninetyWidgetBottom, SIGNAL( clicked( bool ) ), SLOT( switchBottom() ) );
  
  ninetyWidgetLeft = new circleWidget( this );
  connect( ninetyWidgetLeft, SIGNAL( clicked( bool ) ), SLOT( switchLeft() ) );
}

ninetyWidget::~ninetyWidget()
{
}

void ninetyWidget::resizeEvent ( QResizeEvent * event )
{
  (void) event;
  radioSize = width()/2;
  
  ninetyWidgetTop->resize(radioSize, radioSize);
  ninetyWidgetTop->move( width()/2- radioSize/2, 0 );
  
  ninetyWidgetRight->resize(radioSize, radioSize);
  ninetyWidgetRight->move( width()-radioSize, height()/2-radioSize/2 );

  ninetyWidgetBottom->resize( radioSize, radioSize );
  ninetyWidgetBottom->move( width()/2- radioSize/2, height()-radioSize );
  
  ninetyWidgetLeft->resize(radioSize, radioSize);
  ninetyWidgetLeft->move( 0, height()/2-radioSize/2 );
}

void ninetyWidget::setColor( const QColor color)
{
  ninetyWidgetTop->setColor( color );
  ninetyWidgetRight->setColor( color );
  ninetyWidgetBottom->setColor( color );
  ninetyWidgetLeft->setColor( color );
}

void ninetyWidget::switchTop()
{
  ninetyWidgetTop->setPoint();
  ninetyWidgetRight->unsetPoint();
  ninetyWidgetBottom->unsetPoint();
  ninetyWidgetLeft->unsetPoint();
  emit switchRadioButton( "Top" );
}

void ninetyWidget::switchRight()
{
  ninetyWidgetTop->unsetPoint();
  ninetyWidgetRight->setPoint();
  ninetyWidgetBottom->unsetPoint();
  ninetyWidgetLeft->unsetPoint();
  emit switchRadioButton( "Right" );
}

void ninetyWidget::switchBottom()
{
  ninetyWidgetTop->unsetPoint();
  ninetyWidgetRight->unsetPoint();
  ninetyWidgetBottom->setPoint();
  ninetyWidgetLeft->unsetPoint();
  emit switchRadioButton( "Bottom" );
}

void ninetyWidget::switchLeft()
{
  ninetyWidgetTop->unsetPoint();
  ninetyWidgetRight->unsetPoint();
  ninetyWidgetBottom->unsetPoint();
  ninetyWidgetLeft->setPoint();
  emit switchRadioButton( "Left" );
}
