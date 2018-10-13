#include "QvkGlobalMouse.h"

#include <QDebug>
#include <QCoreApplication>

#ifdef Q_OS_LINUX
   #include<X11/Xlib.h>
   #include<stdio.h>
#endif

#ifdef Q_OS_WIN
   #include <windows.h>
#endif

QvkGlobalMouse::QvkGlobalMouse()
{
  onOff = false;
}

QvkGlobalMouse::~QvkGlobalMouse()
{
}

void QvkGlobalMouse::setCursorOn()
{
  onOff = true;
}

void QvkGlobalMouse::setCursorOff()
{
  onOff = false;
}

/*
int keyPressed(int key){
    return (GetAsyncKeyState(key) & 0x8000 != 0);
}

int main(){
    while(1){
        if(keyPressed(VK_LBUTTON)){
            printf("%s\n","Click izquierdo");
        }
        if(keyPressed(VK_RBUTTON)){
            printf("%s\n","Click Derecho");
        }
    }
    return 0;
}
*/

#ifdef Q_OS_WIN
int keyPressed(int key)
{
    return (GetAsyncKeyState(key) & 0x8000 != 0);
}

void QvkGlobalMouse::mousePressed()
{
    while( onOff )
    {
        if(keyPressed(VK_LBUTTON))
        {
            printf("%s\n","Click left button");
        }
        if(keyPressed(VK_RBUTTON))
        {
            printf("%s\n","Click right button");
        }
    }
}
#endif


#ifdef Q_OS_LINUX
void QvkGlobalMouse::mousePressed()
{
  Display* display;
  Window root;
  display = XOpenDisplay( NULL );
  root = DefaultRootWindow( display);
  Window root_return, child_return;
  int x, y;
  int win_x, win_y;
  unsigned int mask;
  int pressed = 0;

  while( onOff )
  {
    QCoreApplication::processEvents( QEventLoop::AllEvents );
    XQueryPointer(display, root, &root_return, &child_return, &x, &y, &win_x, &win_y, &mask);
    //QTest::qSleep( 10 );

    if ( ( mask & Button1Mask) | ( mask & Button2Mask ) | ( mask & Button3Mask ) )
    {
      if ( pressed == 0 )
      {
        pressed = 1;
	emit mousePressed( win_x, win_y );
      }
    }
    else
      pressed = 0;

    fflush(stdout);
  }
  XCloseDisplay( display );
}
#endif
