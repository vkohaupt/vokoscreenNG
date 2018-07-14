#include "QvkWebcamWindow.h" 

QvkWebcamWindow::QvkWebcamWindow():border(true),
                                   overFullScreen(false)
{
  // Es werden die Abmaße des Bildes in der conf abgespeichert, also ohne Rahmen
  
  //vkSettings.readAll();
  setToolTip( tr( "Right click for menu" ) );

  //setWindowTitle( vkSettings.getProgName() + " " + "webcam" + " " + vkSettings.getVersion() );

  setWindowFlags( Qt::Window | Qt::WindowStaysOnTopHint);
  setStyleSheet( "background-color: black;" );

  QIcon icon;
  icon.addFile( QString::fromUtf8( ":/pictures/vokoscreen.png" ), QSize(), QIcon::Normal, QIcon::Off );
  QvkWebcamWindow::setWindowIcon( icon );

  webcamLabel = new QLabel( this );
  webcamLabel->setAlignment( Qt::AlignCenter );
  webcamLabel->show();
 
  actionLabelWindowSize = new QAction( tr( "Windowsize" ) );
  actionLabelWindowSize->setEnabled( false );

  action160x120 = new QAction( "160 x 120", this );
  action160x120->setCheckable( true );
  connect( action160x120, SIGNAL( triggered() ), this, SLOT( set160x120() ) );
   
  action320x240 = new QAction( "320 x 240", this );
  action320x240->setCheckable( true );
  connect( action320x240, SIGNAL( triggered() ), this, SLOT( set320x240() ) );

  action640x480 = new QAction( "640 x 480", this );
  action640x480->setCheckable( true );
  connect( action640x480, SIGNAL( triggered() ), this, SLOT( set640x480() ) );

  actionUserDefined = new QAction( tr( "User-defined" ), this );
  actionUserDefined->setCheckable( true );
  connect( actionUserDefined, SIGNAL( triggered() ), this, SLOT( setActionUserDefined() ) );

  actionLabelOptions = new QAction( tr( "Options" ) );
  actionLabelOptions->setEnabled( false );

  actionBorder = new QAction( tr( "Border" ), this );
  actionBorder->setCheckable( true );
  connect( actionBorder, SIGNAL( triggered( bool ) ), this, SLOT( setBorder( bool ) ) );

#ifndef Q_OS_WIN
  actionVisibleOverFullscreen = new QAction( tr ( "Show over fullscreen" ), this );
  actionVisibleOverFullscreen->setCheckable( true );
  connect( actionVisibleOverFullscreen, SIGNAL( triggered() ), this, SLOT( visibleOverFullscreen() ) );
#endif

  actionClose = new QAction( tr ( "Close" ), this );
  connect( actionClose, SIGNAL( triggered() ), this, SLOT( closeMenue() ) );
/*
  setGeometry( vkSettings.getWebcamX(),
               vkSettings.getWebcamY(),
               vkSettings.getWebcamWidth(),
               vkSettings.getWebcamHeight() );

  if ( vkSettings.getWebcamBorder() == true  )
  {
      setWindowFlags( Qt::Window | Qt::WindowStaysOnTopHint);
      actionBorder->setChecked( true );
      setValueBorder( true );
  }
  else
  {
      setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
      actionBorder->setChecked( false );
      setValueBorder( false );
  }

  if ( ( vkSettings.getWebcamWidth() == 160 ) and ( vkSettings.getWebcamHeight() == 120 ) )
  {
    set160x120();
  }
  else if ( ( vkSettings.getWebcamWidth() == 320 ) and ( vkSettings.getWebcamHeight() == 240 ) )
       {
         set320x240();
       }else if ( ( vkSettings.getWebcamWidth() == 640 ) and ( vkSettings.getWebcamHeight() == 480 ) )
             {
               set640x480();
             }else
              {
                setActionUserDefined();
              }
*/
}


QvkWebcamWindow::~QvkWebcamWindow()
{
}

int QvkWebcamWindow::getValueX()
{
  return geometry().x();
}

int QvkWebcamWindow::getValueY()
{
  return geometry().y();
}

int QvkWebcamWindow::getValueWidth()
{
  return width();
}

int QvkWebcamWindow::getValueHeight()
{
  return height();
}


#ifndef Q_OS_WIN
void QvkWebcamWindow::setOverFullScreen( bool value )
{
  overFullScreen = value;
}
#endif


#ifndef Q_OS_WIN
bool QvkWebcamWindow::getOverFullScreen()
{
  return overFullScreen;
}
#endif


/**
 *  closeEvent wird ausgelößt wenn das webcamfenster geschloßen wird
 */
void QvkWebcamWindow::closeEvent( QCloseEvent * event )
{
  (void)event;
  emit closeWebcamWindow();
}

/**
 * Wird aufgerufen sobald die Kontexttaste betätigt wird
 */
void QvkWebcamWindow::contextMenuEvent( QContextMenuEvent *event )
{
     menu.addAction( actionLabelWindowSize );
     menu.addAction( action160x120 );
     menu.addAction( action320x240 );
     menu.addAction( action640x480 );
     menu.addAction( actionUserDefined );
     menu.addSeparator();
     menu.addAction( actionLabelOptions );
     menu.addAction( actionBorder );
#ifndef Q_OS_WIN
     menu.addAction( actionVisibleOverFullscreen );
#endif
     menu.addSeparator();
     menu.addAction( actionClose );
     menu.exec( event->globalPos() );
 }

/**
 * Wird vom Popupmenue aufgerufen
 */
void QvkWebcamWindow::closeMenue()
{
  close();
}
 
 
void QvkWebcamWindow::set160x120()
{
  if ( isMaximized() )
    showNormal();

  setGeometry( geometry().x(), geometry().y(), 160, 120 );
  action160x120->setChecked( true );
  action320x240->setChecked( false );
  action640x480->setChecked( false );
  actionUserDefined->setChecked( false );
  actionUserDefined->setVisible ( false );
  emit webcamWindoResize( QSize( 160, 120 ) );
}


void QvkWebcamWindow::set320x240()
{
  if ( isMaximized() )
    showNormal();

  setGeometry( geometry().x(), geometry().y(), 320, 240 );
  action160x120->setChecked( false );
  action320x240->setChecked( true );
  action640x480->setChecked( false );
  actionUserDefined->setChecked( false );
  actionUserDefined->setVisible ( false );
  emit webcamWindoResize( QSize( 320, 240 ) );
}


void QvkWebcamWindow::set640x480()
{
  if ( isMaximized() )
    showNormal();
  
  setGeometry( geometry().x(), geometry().y(), 639, 479 ); // Bei 640x480 stützt das Fenster ab evtl. wegen Viewfinder der auf 640x480 gesetzt ist
  action160x120->setChecked( false );
  action320x240->setChecked( false );
  action640x480->setChecked( true );
  actionUserDefined->setChecked( false );
  actionUserDefined->setVisible ( false );
  emit webcamWindoResize( QSize( 640, 480 ) );
}


void QvkWebcamWindow::setActionUserDefined()
{
  action160x120->setChecked( false );
  action320x240->setChecked( false );
  action640x480->setChecked( false );
  actionUserDefined->setChecked( true );
  actionUserDefined->setVisible ( true );
}


void QvkWebcamWindow::setBorder( bool value )
{
  if ( value == false )
  {
    hide();
      setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
#ifndef Q_OS_WIN
      actionVisibleOverFullscreen->setChecked( false );
      setOverFullScreen( false );
#endif
      setValueBorder( false );
    show();
  }
  else
  {
    hide();
      setWindowFlags( Qt::Window | Qt::WindowStaysOnTopHint);
#ifndef Q_OS_WIN
      actionVisibleOverFullscreen->setChecked( false );
      setOverFullScreen( false );
#endif
      setValueBorder( true );
    show();
  }
}

#ifndef Q_OS_WIN
void QvkWebcamWindow::visibleOverFullscreen()
{
    actionBorder->setChecked( false );
    emit setOverScreen();
}
#endif


#ifndef Q_OS_WIN
// Wird von QvkWebcamController::overFullScreenCheckBox_OnOff() aufgerufen
void QvkWebcamWindow::overFullScreenSetWindowFlags()
{
    if ( actionVisibleOverFullscreen->isChecked() )
    {
      setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint );
      actionBorder->setChecked( false );
      setOverFullScreen( true );
      setValueBorder( false );
    }
    else
    {
      setWindowFlags( Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );
      actionBorder->setChecked( true );
      setOverFullScreen( false );
      setValueBorder( true );
    }
}
#endif


void QvkWebcamWindow::resizeEvent ( QResizeEvent *event )
{
  webcamLabel->setGeometry( 0, 0, width(), height() );

  if ( ( width() == 160 ) and ( height() == 120 ) )
  { 
    return;
  } 
  else if ( ( width() == 320 ) and ( height() == 240 ) )
       { 
         return;
       }
       else if ( ( width() == 640 ) and ( height() == 480 ) )
            { 
              return;
            }
            else
            {
              setActionUserDefined();
            }
  emit webcamWindoResize( event->size() );
}


void QvkWebcamWindow::setValueBorder( bool value )
{
  border = value;
}


bool QvkWebcamWindow::getValueBorder()
{
  return border;
}
