#include "screencast.h"

screencast::screencast()
{
  setGeometry( 300, 300, 320, 240 );
  setMouseTracking ( true );
  
  label = new QLabel( this );
  label->setAlignment( Qt::AlignCenter );
  label->setScaledContents( true );
  label->setGeometry( 0, 0, width(), height() );
  qImage = new QImage( "/vkmnt/daten/Daten/Volker/Programmieren/qt/vokoscreenplus/myWidget/vkohaupt.jpeg" );
  label->setPixmap(QPixmap::fromImage( *qImage, Qt::AutoColor) );
  label->show();

  resizeTopRight = new resizeTopRightWidget( this );
  resizeTopRight->resize( 50, 50 );
  resizeTopRight->setColor( Qt::green );
  resizeTopRight->setVisible( false );
  
  mirrorWidget = new circleWidget( this );
  mirrorWidget->resize( 30, 30);
  mirrorWidget->setToolTip( "Gespiegelt" );
  mirrorWidget->setColor( Qt::blue );
  mirrorWidget->setVisible ( false );
  connect( mirrorWidget, SIGNAL( clicked( bool ) ), SLOT( mirrored( bool ) ) );
  
  myFrameWidget = new frameWidget( this );
  myFrameWidget->resize( 30, 30 );
  myFrameWidget->setColor( Qt::darkRed );
  myFrameWidget->setVisible( false );
  connect( myFrameWidget, SIGNAL( clicked( bool ) ), SLOT( frameOnOff( bool ) ) );
  
  myNinetyWidget = new ninetyWidget( this );
  myNinetyWidget->resize(30, 30);
  myNinetyWidget->setColor( Qt::magenta );
  myNinetyWidget->setVisible( false);
  myNinetyWidget->switchBottom();
  connect( myNinetyWidget, SIGNAL( switchRadioButton( QString ) ), SLOT( switchRadioButton( QString ) ) );

  mySettingsWidget = new settingsWidget( this );
  mySettingsWidget->resize( 50, 50 );
  mySettingsWidget->setColor( Qt::blue );
  mySettingsWidget->setVisible( false );

/*  
  rotateDial = new QDial( this );
  rotateDial->setWrapping ( true );
  rotateDial->resize(30, 30 );
  rotateDial->setVisible( false );
  rotateDial->setNotchesVisible ( true );
*/  
}

screencast::~screencast()
{
  //delete myFrameWidget;
  //delete mirrorWidget;
  //delete myNinetyWidget;
}

void screencast::mirrored( bool value )
{
  QImage image;
  image = qImage->mirrored( value, false );
  label->setPixmap( QPixmap::fromImage( image, Qt::AutoColor) );
}

void screencast::enterEvent( QEvent* )
{
  if( myFrameWidget->isFrameSet() == true )
  {
   resizeTopRight->setVisible( true );
  }
   
  myFrameWidget->setVisible( true );
  myNinetyWidget->setVisible( true );
  mirrorWidget->setVisible( true );
  //rotateDial->setVisible( true );
  mySettingsWidget->setVisible( true );
}
   
void screencast::leaveEvent( QEvent* )
{
   resizeTopRight->setVisible( false );
   myFrameWidget->setVisible( false );
   myNinetyWidget->setVisible( false );
   mirrorWidget->setVisible( false );
   //rotateDial->setVisible( false );
   mySettingsWidget->setVisible( false );
}   

void screencast::resizeEvent ( QResizeEvent * )
{
  int count = 2;
  int x = 0;  
  int y = height()/(count+1);
  
  label->resize( width(), height() );

  resizeTopRight->move( width()-resizeTopRight->width()/2, 0-resizeTopRight->height()/2 );
  myFrameWidget->move( width()-40, y * ++x );
  mySettingsWidget->move( width()-40, y * ++x );
  //rotateDial->move( this->width()-50, height()/4 );
  myNinetyWidget->move( width()-40, y * ++x );
  mirrorWidget->move( width()-40, y * ++x );
}

void screencast::frameOnOff( bool value )
{
  if ( value == true )
  {
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    resizeTopRight->setVisible( true );
    show();
  }
  
  if ( value == false )
  {
    setWindowFlags( Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );
    resizeTopRight->setVisible( false );
    show();
  }
}

void screencast::switchRadioButton( QString value)
{
  qDebug() << value;
  
  int rotateValue ;
  
  if ( value == "Left" )
    rotateValue = 90 ;

  if ( value == "Top" )
    rotateValue = 180 ;

  if ( value == "Right" )
    rotateValue = 270 ;
 
  if ( value == "Bottom" )
    rotateValue = 360 ;
  
  QTransform transform;
  transform.rotate( rotateValue );
  QImage transformedImage = qImage->transformed( transform );
  label->setPixmap( QPixmap::fromImage( transformedImage, Qt::AutoColor) );
}
