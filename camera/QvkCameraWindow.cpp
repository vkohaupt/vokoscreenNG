#include "QvkCameraWindow.h"

QvkCameraWindow::QvkCameraWindow()
{
    setGeometry( 300, 300, 320, 240 );
    setAlignment( Qt::AlignCenter );
    setStyleSheet( "background-color:black;" );

    widgetSize = QSize( 30, 30 );
    margin = 10;

    widgetMenueBackground = new QvkWidgetMenueBackground( this );
    widgetMenueBackground->setVisible( false );

    myWidgetHelp = new QvkWidgetHelp( this );
    myWidgetHelp->resize( widgetSize );
    myWidgetHelp->setToolTip( tr( "Help" ) );
    myWidgetHelp->setVisible( false );

    myFrameWidget = new frameWidget( this );
    myFrameWidget->resize( widgetSize );
    myFrameWidget->setToolTip( tr( "Border" ) );
    myFrameWidget->setVisible( false );
    connect( myFrameWidget, SIGNAL( clicked( bool ) ), SLOT( slot_frameOnOff( bool ) ) );

    mySettingsWidget = new QvkWidgetSettings( this );
    mySettingsWidget->resize( widgetSize );
    mySettingsWidget->setToolTip( tr( "Settings" ) );
    mySettingsWidget->setVisible( false );

    myWidgetExit = new QvkWidgetExit( this );
    myWidgetExit->resize( widgetSize );
    myWidgetExit->setToolTip( tr( "Close" ) );
    myWidgetExit->setVisible( false );
    connect( myWidgetExit, SIGNAL( clicked() ), SLOT( close() ) );

}


QvkCameraWindow::~QvkCameraWindow()
{
}


void QvkCameraWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit signal_webcamwindow_close();
}


void QvkCameraWindow::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    widgetMenueBackground->setVisible( false );
    myWidgetHelp->setVisible( false );
    myFrameWidget->setVisible( false );
    mySettingsWidget->setVisible( false );
    myWidgetExit->setVisible( false );
}


void QvkCameraWindow::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    widgetMenueBackground->setVisible( true );
    myWidgetHelp->setVisible( true );
    myFrameWidget->setVisible( true );
    mySettingsWidget->setVisible( true );
    myWidgetExit->setVisible( true );
}


void QvkCameraWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    widgetMenueBackground->setGeometry( width() - margin/2 - widgetSize.width() - margin/2,
                                        margin/2,
                                        widgetSize.width() + margin/2,
                                        height() - margin );
    myWidgetHelp->move( this->width() - widgetSize.width() - margin,     1*margin );
    myFrameWidget->move( this->width() - widgetSize.width() - margin,    2*margin + 1*widgetSize.height() );
    mySettingsWidget->move( this->width() - widgetSize.width() - margin, 3*margin + 2*widgetSize.height() );
    myWidgetExit->move( this->width() - widgetSize.width() - margin,     4*margin + 3*widgetSize.height() );
}


void QvkCameraWindow::slot_frameOnOff( bool value )
{
  if ( value == true )
  {
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    show();
  }

  if ( value == false )
  {
    setWindowFlags( Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );
    show();
  }
}


void QvkCameraWindow::slot_set160x120()
{
    resize( 160, 120 );
}


void QvkCameraWindow::slot_set320x240()
{
    resize( 320, 240 );
}


void QvkCameraWindow::slot_set640x480()
{
    resize( 640, 480 );
}


void QvkCameraWindow::slot_closeWebcamWindow()
{
    close();
}
