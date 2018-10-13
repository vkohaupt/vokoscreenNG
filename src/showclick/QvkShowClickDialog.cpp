#include "QvkShowClickDialog.h"

#include <QDebug>

QvkShowClickDialog::QvkShowClickDialog( QColor color, bool radiant, double opacity )
{
    vkSettings.readAll();
    
    newDialog = new QDialog;
    newDialog->setModal( true );

    myUiDialog.setupUi( newDialog );
   
    myUiDialog.pushButton_white->setStyleSheet( "background-color: white;" );
    myUiDialog.pushButton_black->setStyleSheet( "background-color: black;" );
    myUiDialog.pushButton_red->setStyleSheet( "background-color: red;" );
    myUiDialog.pushButton_darkRed->setStyleSheet( "background-color: darkRed;" );
    myUiDialog.pushButton_green->setStyleSheet( "background-color: green;" );
    myUiDialog.pushButton_darkGreen->setStyleSheet( "background-color: darkGreen;" );
    myUiDialog.pushButton_blue->setStyleSheet( "background-color: blue;" );
    myUiDialog.pushButton_darkBlue->setStyleSheet( "background-color: darkBlue;" );
    myUiDialog.pushButton_cyan->setStyleSheet( "background-color: cyan;" );
    myUiDialog.pushButton_darkCyan->setStyleSheet( "background-color: darkCyan;" );
    myUiDialog.pushButton_magenta->setStyleSheet( "background-color: magenta;" );
    myUiDialog.pushButton_darkMagenta->setStyleSheet( "background-color: darkMagenta;" );
    myUiDialog.pushButton_yellow->setStyleSheet( "background-color: yellow;" );
    myUiDialog.pushButton_darkYellow->setStyleSheet( "background-color: darkkhaki;" );
    myUiDialog.pushButton_gray->setStyleSheet( "background-color: gray;" );
    myUiDialog.pushButton_darkGray->setStyleSheet( "background-color: darkGray;" );
    
    connect( myUiDialog.pushButton_white, SIGNAL( clicked() ), this, SLOT( white() ) );
    connect( myUiDialog.pushButton_black, SIGNAL( clicked() ), this, SLOT( black() ) );
    connect( myUiDialog.pushButton_red, SIGNAL( clicked() ), this, SLOT( red() ) );
    connect( myUiDialog.pushButton_darkRed, SIGNAL( clicked() ), this, SLOT( darkRed() ) );
    connect( myUiDialog.pushButton_green, SIGNAL( clicked() ), this, SLOT( green() ) );
    connect( myUiDialog.pushButton_darkGreen, SIGNAL( clicked() ), this, SLOT( darkGreen() ) );
    connect( myUiDialog.pushButton_blue, SIGNAL( clicked() ), this, SLOT( blue() ) );
    connect( myUiDialog.pushButton_darkBlue, SIGNAL( clicked() ), this, SLOT( darkBlue() ) );
    connect( myUiDialog.pushButton_cyan, SIGNAL( clicked() ), this, SLOT( cyan() ) );
    connect( myUiDialog.pushButton_darkCyan, SIGNAL( clicked() ), this, SLOT( darkCyan() ) );
    connect( myUiDialog.pushButton_magenta, SIGNAL( clicked() ), this, SLOT( magenta() ) );
    connect( myUiDialog.pushButton_darkMagenta, SIGNAL( clicked() ), this, SLOT( darkMagenta() ) );
    connect( myUiDialog.pushButton_yellow, SIGNAL( clicked() ), this, SLOT( yellow() ) );
    connect( myUiDialog.pushButton_darkYellow, SIGNAL( clicked() ), this, SLOT( darkYellow() ) );
    connect( myUiDialog.pushButton_gray, SIGNAL( clicked() ), this, SLOT( gray() ) );
    connect( myUiDialog.pushButton_darkGray, SIGNAL( clicked() ), this, SLOT( darkGray() ) );
    
    connect( myUiDialog.buttonBox, SIGNAL( accepted() ), this, SLOT( ok() ) );
    connect( myUiDialog.buttonBox, SIGNAL( rejected() ), this, SLOT( cancel() ) );
    connect( myUiDialog.buttonBox->button( QDialogButtonBox::RestoreDefaults )   , SIGNAL( clicked() ), this, SLOT( Standard() ) );
    
    circleWidget = new QvkCircleWidget( myUiDialog.graphicsView );
    circleWidget->show();
    valueChangedSliderCircle( myUiDialog.horizontalSliderCircle->value() );
    circleWidget->setColor( color );
    circleWidget->setRadiant( radiant );
    circleWidget->setOpacity( opacity );
    
    connect( myUiDialog.horizontalSliderCircle, SIGNAL( valueChanged( int ) ), this, SLOT( valueChangedSliderCircle( int ) ) );
    myUiDialog.horizontalSliderCircle->setSliderPosition( vkSettings.getShowClickCircle() );
    
    connect( myUiDialog.checkBoxRadiant, SIGNAL( stateChanged( int ) ), this, SLOT( stateChangedRadiant( int ) ) );
    if ( radiant == true )
      myUiDialog.checkBoxRadiant->setCheckState( Qt::Checked );
    else
      myUiDialog.checkBoxRadiant->setCheckState( Qt::Unchecked );

    connect( myUiDialog.horizontalSliderOpacity, SIGNAL( valueChanged( int ) ), this, SLOT( valueChangedOpacity( int ) ) );
    myUiDialog.horizontalSliderOpacity->setSliderPosition( opacity*100 );
    
    myUiDialog.horizontalSliderShowtime->setValue( vkSettings.getShowClickTime() );
}

QvkShowClickDialog::~QvkShowClickDialog()
{
}

QColor QvkShowClickDialog::getColor()
{
  return circleWidget->getColor();
}

void QvkShowClickDialog::show()
{
   newDialog->show();
}

void QvkShowClickDialog::cancel()
{
   newDialog->close();
}

void QvkShowClickDialog::Standard()
{
  myUiDialog.horizontalSliderShowtime->setValue( 5 );
  myUiDialog.pushButton_red->click();
  myUiDialog.horizontalSliderCircle->setValue( 70 );
  myUiDialog.horizontalSliderOpacity->setValue( 50 );
  myUiDialog.checkBoxRadiant->setCheckState( Qt::Unchecked );
}

void QvkShowClickDialog::ok()
{
   emit newCircleWidgetValue( circleWidget->getDiameter(), circleWidget->getColor() );
   emit newShowtime( (double) myUiDialog.horizontalSliderShowtime->value()/10 );
   emit newOpacity( circleWidget->getOpacity() );
   emit newRadiant( circleWidget->getRadiant() );
   newDialog->close();
}

void QvkShowClickDialog::valueChangedSliderCircle( int value )
{
  circleWidget->setDiameter( value );
}

void QvkShowClickDialog::valueChangedOpacity( int value )
{
  double value_1 = value;
  circleWidget->setOpacity( value_1 / 100 );
}

void QvkShowClickDialog::stateChangedRadiant( int value )
{
  if ( value == Qt::Unchecked )
    circleWidget->setRadiant( false );
  else
    circleWidget->setRadiant( true );
}

void QvkShowClickDialog::white()
{
  circleWidget->setColor( Qt::white );
}

void QvkShowClickDialog::black()
{
  circleWidget->setColor( Qt::black );
}

void QvkShowClickDialog::red()
{
  circleWidget->setColor( Qt::red );
}

void QvkShowClickDialog::darkRed()
{
  circleWidget->setColor( Qt::darkRed );
}

void QvkShowClickDialog::green()
{
  circleWidget->setColor( Qt::green );
}

void QvkShowClickDialog::darkGreen()
{
  circleWidget->setColor( Qt::darkGreen );
}

void QvkShowClickDialog::blue()
{
  circleWidget->setColor( Qt::blue );
}

void QvkShowClickDialog::darkBlue()
{
  circleWidget->setColor( Qt::darkBlue );
}

void QvkShowClickDialog::cyan()
{
  circleWidget->setColor( Qt::cyan );
}

void QvkShowClickDialog::darkCyan()
{
  circleWidget->setColor( Qt::darkCyan );
}

void QvkShowClickDialog::magenta()
{
  circleWidget->setColor( Qt::magenta );
}

void QvkShowClickDialog::darkMagenta()
{
  circleWidget->setColor( Qt::darkMagenta );
}

void QvkShowClickDialog::yellow()
{
  circleWidget->setColor( Qt::yellow );
}

void QvkShowClickDialog::darkYellow()
{
  circleWidget->setColor( Qt::darkYellow );
}

void QvkShowClickDialog::gray()
{
  circleWidget->setColor( Qt::gray );
}

void QvkShowClickDialog::darkGray()
{
  circleWidget->setColor( Qt::darkGray );
}
