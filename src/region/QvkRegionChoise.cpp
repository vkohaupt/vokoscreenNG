/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
 * 
 * Author:
 *      Volker Kohaupt <vkohaupt@freenet.de>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * --End_License--
 */

#include "QvkRegionChoise.h"
#include "QvkRegionButtonArrow.h"
#include "global.h"

#include <QDebug>
#include <QPainter>
#include <QGuiApplication>
#include <QBitmap>
#include <QPaintEvent>
#include <QIcon>

#ifdef Q_OS_LINUX
  #include <QX11Info>
#endif

QvkRegionChoise::QvkRegionChoise( Ui_formMainWindow *ui_formMainWindow ):handlePressed(NoHandle),
                                   handleUnderMouse(NoHandle),
                                   HandleColorBackground( Qt::lightGray ),
                                   HandleColorBackgroundSize( Qt::lightGray ),
                                   HandleColorByMousePressed( Qt::lightGray ),
                                   colorSelectedArrow( Qt::green ),
                                   framePenWidth(4), // framePenWidth must be an even number
                                   framePenHalf(framePenWidth/2),
                                   radius(20),
                                   diameter(radius*2),
                                   penWidth(2),
                                   penHalf(penWidth/2),
                                   frame_X(200-framePenHalf),
                                   frame_Y(200-framePenHalf),
                                   frame_Width(320 + framePenWidth),
                                   frame_height(200 + framePenWidth),
                                   frame_min_width(320 + framePenWidth),
                                   frame_min_height(200 + framePenWidth),
                                   frameColor(Qt::lightGray)
{
#ifdef Q_OS_LINUX
    if ( QX11Info::isPlatformX11() == true )
        platform = x11;

    if ( QX11Info::isPlatformX11() == false )
        platform = wayland;
#endif
    ui = ui_formMainWindow;

    setWindowTitle( QString( tr( "Area") ) );

    QIcon icon;
    icon.addFile( QString::fromUtf8( ":/pictures/logo/logo.png" ), QSize(), QIcon::Normal, QIcon::Off );
    setWindowIcon( icon );

    // Hint: Qt::WindowStaysOnTopHint is only for X11 and Windows on WayLand not do it
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip);
    setAttribute( Qt::WA_TranslucentBackground, true);
    setMouseTracking( true );
    setFrameColor( Qt::darkGreen );
    hide();
}


void QvkRegionChoise::slot_init()
{
    if ( ui->comboBoxScreencastScreenArea->currentIndex() > -1 )
    {
        int index = ui->comboBoxScreencastScreenArea->currentIndex();
        QList<QScreen *> screenList = QGuiApplication::screens();
        screen = screenList.at( index );

        resize( screen->size().width(), screen->size().height() );
        screenWidth = screen->size().width();
        screenHeight = screen->size().height();
        move( screen->geometry().x(), screen->geometry().y() );
    }
}


QvkRegionChoise::~QvkRegionChoise()
{
}


void QvkRegionChoise::setHandleColorBackground( QColor color )
{
    HandleColorBackground = color;
}


void QvkRegionChoise::setHandleColorByMousePressed( QColor color )
{
    HandleColorByMousePressed = color;
}


void QvkRegionChoise::setHandleColorBackgroundSize( QColor color )
{
    HandleColorBackgroundSize = color;
}


/*
 * x, y is middle from blue frame
 */
void QvkRegionChoise::vk_setGeometry( int x, int y, int with, int height  )
{
  frame_X = x;
  frame_Y = y;
  frame_Width = with;
  frame_height = height;
  update();
}


void QvkRegionChoise::paintEvent( QPaintEvent *event )
{
    (void)event;

    QPixmap pixmap( screenWidth, screenHeight );

    if ( recordemode == false )
    {
        pixmap.fill( Qt::transparent );
        QPainter painterPixmap;
        painterPixmap.begin( &pixmap );
        painterPixmap.setRenderHints( QPainter::Antialiasing, true );
        HandleTopLeft( painterPixmap );
        HandleTopMiddle( painterPixmap );
        HandleTopRight( painterPixmap );
        HandleRightMiddle( painterPixmap );
        HandleBottomRight( painterPixmap );
        HandleBottomMiddle( painterPixmap );
        HandleBottomLeft( painterPixmap );
        HandleLeftMiddle( painterPixmap );
        HandleMiddle( painterPixmap );
        printSize( painterPixmap);
        drawFrame( painterPixmap );
        switch ( handlePressed )
        {
        case NoHandle    : break;
        case TopLeft     : HandleTopLeftSize( painterPixmap );     break;
        case TopMiddle   : HandleTopMiddleSize( painterPixmap );   break;
        case TopRight    : HandleTopRightSize( painterPixmap );    break;
        case RightMiddle : HandleRightMiddleSize( painterPixmap ); break;
        case BottomRight : HandleBottomRightSize( painterPixmap ); break;
        case BottomMiddle: HandleBottomMiddleSize( painterPixmap );break;
        case BottomLeft  : HandleBottomLeftSize( painterPixmap );  break;
        case LeftMiddle  : HandleLeftMiddleSize( painterPixmap);   break;
        case Middle      : HandleTopLeftSize( painterPixmap );     break;
        }
        painterPixmap.end();
    }
    else
    {
        pixmap.fill( Qt::transparent );
        QPainter painterPixmap;
        painterPixmap.begin( &pixmap );
        painterPixmap.setRenderHints( QPainter::Antialiasing, true );
        HandleRecord( painterPixmap,
                      frame_X - radius + penHalf,
                      frame_Y - radius + penHalf,
                        0 * 16,
                      270 * 16 );
        HandleRecord( painterPixmap,
                      frame_X + frame_Width/2 - radius + penHalf,
                      frame_Y - radius + penHalf,
                        0 * 16,
                      180 * 16 );
        HandleRecord( painterPixmap,
                      frame_X + frame_Width - radius + penHalf,
                      frame_Y - radius + penHalf,
                      -90 * 16,
                      270 * 16 );
        HandleRecord( painterPixmap,
                      frame_X + frame_Width - radius + penHalf,
                      frame_Y + frame_height/2 - radius + penHalf,
                      -90 * 16,
                      180 * 16 );
        HandleRecord( painterPixmap,
                      frame_X + frame_Width - radius + penHalf,
                      frame_Y + frame_height - radius + penHalf,
                      -180 * 16,
                       270 * 16 );
        HandleRecord( painterPixmap,
                      frame_X + frame_Width/2 - radius + penHalf,
                      frame_Y + frame_height - radius + penHalf,
                         0 * 16,
                      -180 * 16 );
        HandleRecord( painterPixmap,
                      frame_X - radius + penHalf,
                      frame_Y + frame_height - radius + penHalf,
                         0 * 16,
                      -270 * 16 );
        HandleRecord( painterPixmap,
                      frame_X - radius + penHalf,
                      frame_Y + frame_height/2 - radius + penHalf,
                      -90 * 16,
                      -180 * 16 );

        drawFrame( painterPixmap );

        // setMask(pixmap.mask()) is not working if enlarge the Area over the full screen.
        // Remedy: We draw a black pixel with a width=1 on the left top corner and setMask(pixmap.mask()) works.
        QPen pen;
          pen.setColor( Qt::black );
          pen.setWidth( 1 );
        painterPixmap.setPen( pen );
        painterPixmap.drawPoint( 0, 0 );
    }

    QPainter painter;
    painter.begin( this );
    painter.drawPixmap( QPoint( 0, 0 ), pixmap );
    painter.end();

    setMask( pixmap.mask() );
}


void QvkRegionChoise::mousePressEvent(QMouseEvent *event)
{
    if( event->button() != Qt::LeftButton)
    {
        return;
    }

    switch ( handleUnderMouse )
    {
      case NoHandle    : handlePressed = NoHandle;     break;
      case TopLeft     : { handlePressed = TopLeft;      handleKeyPressed = TopLeft;      HandleSelected = TopLeft;      break; }
      case TopMiddle   : { handlePressed = TopMiddle;    handleKeyPressed = TopMiddle;    HandleSelected = TopMiddle;    break; }
      case TopRight    : { handlePressed = TopRight;     handleKeyPressed = TopRight;     HandleSelected = TopRight;     break; }
      case RightMiddle : { handlePressed = RightMiddle;  handleKeyPressed = RightMiddle;  HandleSelected = RightMiddle;  break; }
      case BottomRight : { handlePressed = BottomRight;  handleKeyPressed = BottomRight;  HandleSelected = BottomRight;  break; }
      case BottomMiddle: { handlePressed = BottomMiddle; handleKeyPressed = BottomMiddle; HandleSelected = BottomMiddle; break; }
      case BottomLeft  : { handlePressed = BottomLeft;   handleKeyPressed = BottomLeft;   HandleSelected = BottomLeft;   break; }
      case LeftMiddle  : { handlePressed = LeftMiddle;   handleKeyPressed = LeftMiddle;   HandleSelected = LeftMiddle;   break; }
      case Middle      : { handlePressed = Middle;       handleKeyPressed = Middle;       HandleSelected = Middle;       break; }
    }

    mous_delta_X_to_blueline = event->x() - frame_X;
    mous_delta_Y_to_blueline = event->y() - frame_Y;

    old_Mouse_X = event->x();
    old_Mouse_Y = event->y();
    old_Frame_Width = frame_Width;
    old_Frame_Height = frame_height;

    old_Frame_X2 = frame_X + frame_Width;
    old_Frame_Y2 = frame_Y + frame_height;

#ifdef Q_OS_WIN
    repaint();
    update();
#endif

#ifdef Q_OS_LINUX
    if ( platform == wayland )
    {
      clearMask();
      update();
    }

    if ( platform == x11 )
    {
      repaint();
      update();
    }
#endif

}


void QvkRegionChoise::mouseReleaseEvent( QMouseEvent * event )
{
  if( event->button() != Qt::LeftButton)
  {
      return;
  }

  handlePressed = NoHandle;

  update();
}


void QvkRegionChoise::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    unsetCursor();
}


void QvkRegionChoise::keyPressEvent( QKeyEvent * event )
{
    if ( recordemode == true )
    {
        return;
    }

    if ( handleKeyPressed == TopLeft )
    {
        switch ( event->key() )
        {
            case Qt::Key_Up   :  {
                                    frame_Y = frame_Y - 1;
                                    frame_height = frame_height + 1;

                                    if ( frame_Y < 0 - framePenHalf )
                                    {
                                      frame_Y = frame_Y + 1;
                                      frame_height = frame_height - 1;
                                    }

                                    break;
                                 }
            case Qt::Key_Down :  {
                                    frame_Y = frame_Y + 1;
                                    frame_height = frame_height - 1;

                                    if ( frame_height < frame_min_height )
                                    {
                                        frame_Y = frame_Y - 1;
                                        frame_height = frame_height + 1;
                                    }

                                    break;
                                 }
            case Qt::Key_Left  : {
                                    frame_X = frame_X - 1;
                                    frame_Width = frame_Width + 1;

                                    if ( frame_X < 0 - framePenHalf )
                                    {
                                        frame_X = frame_X + 1;
                                        frame_Width = frame_Width - 1;
                                    }

                                    break;
                                 }
            case Qt::Key_Right : {
                                    frame_X = frame_X + 1;
                                    frame_Width = frame_Width - 1;

                                    if ( frame_Width < frame_min_width )
                                    {
                                        frame_X = frame_X - 1;
                                        frame_Width = frame_Width + 1;
                                    }

                                    break;
                                 }
        }
    }

    if ( handleKeyPressed == TopMiddle )
    {
        switch ( event->key() )
        {
            case Qt::Key_Up   : {
                                   frame_Y = frame_Y - 1;
                                   frame_height = frame_height + 1;

                                   if ( frame_Y < 0 - framePenHalf )
                                   {
                                     frame_Y = frame_Y + 1;
                                     frame_height = frame_height - 1;
                                   }

                                   break;
                                }
            case Qt::Key_Down : {
                                   frame_Y = frame_Y + 1;
                                   frame_height = frame_height - 1;

                                   if ( frame_height < frame_min_height )
                                   {
                                       frame_Y = frame_Y - 1;
                                       frame_height = frame_height + 1;
                                   }

                                   break;
                                }
        }
    }

    if ( handleKeyPressed == TopRight )
    {
        switch ( event->key() )
        {
            case Qt::Key_Up   : {
                                   frame_Y = frame_Y - 1;
                                   frame_height = frame_height + 1;

                                   if ( frame_Y < 0 - framePenHalf )
                                   {
                                     frame_Y = frame_Y + 1;
                                     frame_height = frame_height - 1;
                                   }

                                   break;
                                }
            case Qt::Key_Down : {
                                   frame_Y = frame_Y + 1;
                                   frame_height = frame_height - 1;

                                   if ( frame_height < frame_min_height )
                                   {
                                       frame_Y = frame_Y - 1;
                                       frame_height = frame_height + 1;
                                   }

                                   break;
                                }
            case Qt::Key_Left : {
                                   frame_Width = frame_Width - 1;

                                   if ( frame_Width < frame_min_width )
                                   {
                                       frame_Width = frame_Width + 1;
                                   }

                                   break;
                                }
            case Qt::Key_Right :{
                                   frame_Width = frame_Width + 1;

                                   if ( ( frame_X + frame_Width - framePenHalf ) > screenWidth )
                                   {
                                       frame_Width = frame_Width - 1;
                                   }

                                   break;
                                }
        }
    }

    if ( handleKeyPressed == RightMiddle )
    {
        switch ( event->key() )
        {
            case Qt::Key_Left : {
                                   frame_Width = frame_Width - 1;

                                   if ( frame_Width < frame_min_width )
                                   {
                                       frame_Width = frame_Width + 1;
                                   }

                                   break;
                                }
            case Qt::Key_Right :{
                                   frame_Width = frame_Width + 1;

                                   if ( ( frame_X + frame_Width - framePenHalf ) > screenWidth )
                                   {
                                       frame_Width = frame_Width - 1;
                                   }

                                   break;
                                }
        }
    }

    if ( handleKeyPressed == BottomRight )
    {
        switch ( event->key() )
        {
            case Qt::Key_Up   : {
                                   frame_height = frame_height - 1;

                                   if ( frame_height < frame_min_height )
                                   {
                                       frame_height = frame_height + 1;
                                   }

                                   break;
                                }
            case Qt::Key_Down : {
                                   frame_height = frame_height + 1;

                                   if ( frame_Y + frame_height - framePenHalf > screenHeight )
                                   {
                                       frame_height = frame_height - 1;
                                   }

                                   break;
                                }
            case Qt::Key_Left : {
                                   frame_Width = frame_Width - 1;

                                   if ( frame_Width < frame_min_width )
                                   {
                                       frame_Width = frame_Width + 1;
                                   }

                                   break;
                                }
            case Qt::Key_Right :{
                                   frame_Width = frame_Width + 1;

                                   if ( frame_X + frame_Width - framePenHalf > screenWidth )
                                   {
                                       frame_Width = frame_Width - 1;
                                   }

                                   break;
                                }
        }
    }

    if ( handleKeyPressed == BottomMiddle )
    {
        switch ( event->key() )
        {
            case Qt::Key_Up   : {
                                   frame_height = frame_height - 1;

                                   if ( frame_height < frame_min_height )
                                   {
                                       frame_height = frame_height + 1;
                                   }

                                   break;
                                }
            case Qt::Key_Down : {
                                   frame_height = frame_height + 1;

                                   if ( frame_Y + frame_height - framePenHalf > screenHeight )
                                   {
                                       frame_height = frame_height - 1;
                                   }

                                   break;
                                }
        }
     }

    if ( handleKeyPressed == BottomLeft )
    {
        switch ( event->key() )
        {
            case Qt::Key_Up   :  {
                                    frame_height = frame_height - 1;

                                    if ( frame_height < frame_min_height )
                                    {
                                        frame_height = frame_height + 1;
                                    }

                                    break;
                                 }
            case Qt::Key_Down :  {
                                    frame_height = frame_height + 1;

                                    if ( frame_Y + frame_height - framePenHalf > screenHeight )
                                    {
                                        frame_height = frame_height - 1;
                                    }

                                    break;
                                 }
            case Qt::Key_Left  : {
                                    frame_X = frame_X - 1;
                                    frame_Width = frame_Width + 1;

                                    if ( frame_X < 0 - framePenHalf )
                                    {
                                        frame_X = frame_X + 1;
                                        frame_Width = frame_Width - 1;
                                    }

                                    break;
                                 }
            case Qt::Key_Right : {
                                    frame_X = frame_X + 1;
                                    frame_Width = frame_Width - 1;

                                    if ( frame_Width < frame_min_width )
                                    {
                                        frame_X = frame_X - 1;
                                        frame_Width = frame_Width + 1;
                                    }

                                    break;
                                 }
        }
    }

    if ( handleKeyPressed == LeftMiddle )
    {
        switch ( event->key() )
        {
            case Qt::Key_Left  : {
                                    frame_X = frame_X - 1;
                                    frame_Width = frame_Width + 1;

                                    if ( frame_X < 0 - framePenHalf )
                                    {
                                        frame_X = frame_X + 1;
                                        frame_Width = frame_Width - 1;
                                    }

                                    break;
                                 }
            case Qt::Key_Right : {
                                    frame_X = frame_X + 1;
                                    frame_Width = frame_Width - 1;

                                    if ( frame_Width < frame_min_width )
                                    {
                                        frame_X = frame_X - 1;
                                        frame_Width = frame_Width + 1;
                                    }

                                    break;
                                 }
        }
    }

    if ( handleKeyPressed == Middle )
    {
        switch ( event->key() )
        {
            case Qt::Key_Left  : {
                                    frame_X = frame_X - 1;

                                    if (  frame_X + framePenHalf < 0 )
                                    {
                                        frame_X = frame_X + 1;
                                    }

                                    break;
                                 }
            case Qt::Key_Right : {
                                    frame_X = frame_X + 1;

                                    if ( frame_X + frame_Width - framePenHalf > screenWidth )
                                    {
                                        frame_X = frame_X - 1;
                                    }

                                    break;
                                 }
            case Qt::Key_Up    : {
                                    frame_Y = frame_Y - 1;

                                    if ( frame_Y + framePenHalf < 0 )
                                    {
                                        frame_Y = frame_Y + 1;
                                    }

                                    break;
                                 }
            case Qt::Key_Down  : {
                                    frame_Y = frame_Y + 1;

                                    if ( frame_Y + frame_height - framePenHalf > screenHeight )
                                    {
                                        frame_Y = frame_Y - 1;
                                    }

                                    break;
                                 }
        }
    }

    repaint();
    update();
}


void QvkRegionChoise::mouseMoveEvent( QMouseEvent *event )
{
    if ( recordemode == true )
        return;

    switch ( handlePressed )
    {
      case NoHandle    : break;
      case TopLeft     : { // Move
                           frame_X = event->x() - mous_delta_X_to_blueline;
                           frame_Y = event->y() - mous_delta_Y_to_blueline;
                           frame_Width = old_Mouse_X - event->x() + old_Frame_Width;
                           frame_height = old_Mouse_Y - event->y() + old_Frame_Height;

                           // Limit min
                           if ( frame_Width < frame_min_width )
                           {
                             frame_X = old_Frame_X2 - frame_min_width;
                             frame_Width = frame_min_width;
                           }

                           if ( frame_height < frame_min_height )
                           {
                             frame_Y = old_Frame_Y2 - frame_min_height;
                             frame_height = frame_min_height;
                           }

                           // Limit max
                           if ( frame_Y <= 0 - framePenHalf )
                           {
                             frame_Y = 0 - framePenHalf;
                             frame_height = old_Frame_Y2 + framePenHalf;
                           }

                           if ( frame_X <= 0 - framePenHalf )
                           {
                              frame_X = 0 - framePenHalf;
                              frame_Width = old_Frame_X2 + framePenHalf;
                           }

                           break;
                         }
      case TopMiddle   : { // Move
                           frame_Y = event->y() - mous_delta_Y_to_blueline;
                           frame_height = old_Mouse_Y - event->y() + old_Frame_Height;

                           // Limit min
                           if ( frame_height < frame_min_height )
                           {
                             frame_Y = old_Frame_Y2 - frame_min_height;
                             frame_height = frame_min_height;
                           }

                           // Limit max
                           if ( frame_Y <= 0 - framePenHalf )
                           {
                             frame_Y = 0 - framePenHalf;
                             frame_height = old_Frame_Y2 + framePenHalf;
                           }

                           break;
                         }
      case TopRight    : { // Move
                           frame_Y = event->y() - mous_delta_Y_to_blueline;
                           frame_Width = event->x() - old_Mouse_X + old_Frame_Width;
                           frame_height = old_Mouse_Y - event->y() + old_Frame_Height;;

                           // Limit min
                           if ( frame_Width < frame_min_width )
                           {
                             frame_Width = frame_min_width;
                           }

                           if ( frame_height < frame_min_height )
                           {
                             frame_Y = old_Frame_Y2 - frame_min_height;
                             frame_height = frame_min_height;
                           }

                           // Limit max
                           if ( frame_Y <= 0 - framePenHalf )
                           {
                             frame_Y = 0 - framePenHalf;
                             frame_height = old_Frame_Y2 + framePenHalf;
                           }

                           if( ( frame_X + frame_Width - framePenHalf ) > screenWidth )
                           {
                             frame_Width = screenWidth + framePenHalf - frame_X;
                           }

                           break;
                         }
      case RightMiddle : { // Move
                           frame_Width = event->x() - old_Mouse_X + old_Frame_Width;

                           // Limit min
                           if ( frame_Width < frame_min_width )
                           {
                             frame_Width = frame_min_width;
                           }

                           // Limit max
                           if( ( frame_X + frame_Width - framePenHalf ) > screenWidth )
                           {
                             frame_Width = screenWidth + framePenHalf - frame_X;
                           }

                           break;
                         }
      case BottomRight : { // Move
                           frame_Width = event->x() - old_Mouse_X + old_Frame_Width;
                           frame_height = event->y() - old_Mouse_Y + old_Frame_Height;

                           // Limit min
                           if ( frame_Width < frame_min_width )
                           {
                             frame_Width = frame_min_width;
                           }

                           if ( frame_height < frame_min_height )
                           {
                             frame_height = frame_min_height;
                           }

                           //Limit max
                           if( ( frame_X + frame_Width - framePenHalf ) > screenWidth )
                           {
                             frame_Width = screenWidth + framePenHalf - frame_X;
                           }

                           if( ( frame_Y + frame_height - framePenHalf ) > screenHeight )
                           {
                             frame_height = screenHeight + framePenHalf - frame_Y;
                           }

                           break;
                         }
      case BottomMiddle: { // Move
                           frame_height = event->y() - old_Mouse_Y + old_Frame_Height;

                           // Limit min
                           if ( frame_height < frame_min_height )
                           {
                             frame_height = frame_min_height;
                           }

                           //Limit max
                           if( ( frame_Y + frame_height - framePenHalf ) > screenHeight )
                           {
                             frame_height = screenHeight + framePenHalf - frame_Y;
                           }

                           break;
                         }
    case BottomLeft    : {
                           // Move
                           frame_X = event->x() - mous_delta_X_to_blueline;
                           frame_height = event->y() - old_Mouse_Y + old_Frame_Height;
                           frame_Width = old_Mouse_X - event->x() + old_Frame_Width;

                           // Limit min
                           if ( frame_Width < frame_min_width )
                           {
                             frame_X = old_Frame_X2 - frame_min_width;
                             frame_Width = frame_min_width;
                           }

                           if ( frame_height < frame_min_height )
                           {
                             frame_height = frame_min_height;
                           }

                           // Limit max
                           if ( frame_X <= 0 - framePenHalf )
                           {
                              frame_X = 0 - framePenHalf;
                              frame_Width = old_Frame_X2 + framePenHalf;
                           }

                           if( ( frame_Y + frame_height - framePenHalf ) > screenHeight )
                           {
                             frame_height = screenHeight + framePenHalf - frame_Y;
                           }

                           break;
                         }
      case LeftMiddle  : { // Move
                           frame_X = event->x() - mous_delta_X_to_blueline;
                           frame_Width = old_Mouse_X - event->x() + old_Frame_Width;

                           // Limit min
                           if ( frame_Width < frame_min_width )
                           {
                             frame_X = old_Frame_X2 - frame_min_width;
                             frame_Width = frame_min_width;
                           }

                           // Limit max
                           if ( frame_X <= 0 - framePenHalf )
                           {
                              frame_X = 0 - framePenHalf;
                              frame_Width = old_Frame_X2 + framePenHalf;
                           }

                           break;
                         }
      case Middle      : { // Move
                           int deltaX = ( old_Frame_X2 - framePenHalf - frame_Width/2 ) - old_Mouse_X;
                           int deltaY = ( old_Frame_Y2 - framePenHalf - frame_height/2 ) - old_Mouse_Y;
                           frame_X = event->x() - frame_Width/2 + framePenHalf + deltaX;
                           frame_Y = event->y() - frame_height/2 + framePenHalf + deltaY;

                           // Limit Top
                           if ( frame_Y <= 0 - framePenHalf )
                           {
                             frame_Y = 0 - framePenHalf;
                           }

                           // Limit Left
                           if ( frame_X <= 0 - framePenHalf )
                           {
                             frame_X = 0 - framePenHalf;
                           }

                           // Limit Right
                           if( ( frame_X + frame_Width - framePenHalf ) > screenWidth )
                           {
                               frame_X = screenWidth - frame_Width + framePenHalf;
                           }

                           // Limit Bottom
                           if( ( frame_Y + frame_height - framePenHalf ) > screenHeight )
                           {
                               frame_Y = screenHeight - frame_height + framePenHalf;
                           }

                           break;
                         }
    } // end switch

#ifdef Q_OS_LINUX
    if ( handlePressed != NoHandle )
    {
        if ( platform == wayland )
        {
            clearMask();
            update();
        }

        if ( platform == x11 )
        {
            if ( QX11Info::isCompositingManagerRunning() == true )
            {
                repaint();
                update();
            }
            else
            {
                repaint();
                update();
            }
        }
    }
#endif

#ifdef Q_OS_WIN
    repaint();
    update();
#endif

    if ( handlePressed != NoHandle )
        return;

    QRect regionTopLeft( frame_X - radius - 1, frame_Y - radius - 1, diameter + 2, diameter + 2 );
    if ( regionTopLeft.contains( event->pos() ) )
    {
        QPixmap pixmap( ":/pictures/cursor/size_fdiag.png" );
        QCursor cursor( pixmap );
        setCursor( cursor );
        handleUnderMouse = TopLeft;
        return;
    }

    QRect regionTopMiddle( frame_X + frame_Width/2 - radius - 1, frame_Y - radius - 1, diameter + 2, diameter + 2 );
    if ( regionTopMiddle.contains( event->pos() )  )
    {
        QPixmap pixmap( ":/pictures/cursor/size_ver.png" );
        QCursor cursor( pixmap );
        setCursor( cursor );
        handleUnderMouse = TopMiddle;
        return;
    }

    QRect regionTopRight( frame_X + frame_Width - radius - 1, frame_Y - radius - 1, diameter + 2, diameter + 2 );
    if ( regionTopRight.contains( event->pos() )  )
    {
        QPixmap pixmap( ":/pictures/cursor/size_bdiag.png" );
        QCursor cursor( pixmap );
        setCursor( cursor );
        handleUnderMouse = TopRight;
        return;
    }

    QRect regionRightMiddle( frame_X + frame_Width - radius - 1, frame_Y + frame_height/2 - radius - 1, diameter + 2, diameter + 2 );
    if ( regionRightMiddle.contains( event->pos() )  )
    {
        QPixmap pixmap( ":/pictures/cursor/size_hor.png" );
        QCursor cursor( pixmap );
        setCursor( cursor );
        handleUnderMouse = RightMiddle;
        return;
    }

    QRect regionMiddle( frame_X + frame_Width/2 - radius - penHalf, frame_Y + frame_height/2 - radius - penHalf, 2 * radius + penWidth, 2 * radius + penWidth);
    if ( regionMiddle.contains( event->pos() )  )
    {
        QPixmap pixmap( ":/pictures/cursor/size_all.png" );
        QCursor cursor( pixmap );
        setCursor( cursor );
        handleUnderMouse = Middle;
        return;
    }

    QvkRegionButtonArrow buttonArrow;
    QRect regionBottomRight( frame_X + frame_Width - buttonArrow.getWithHalf(),
                               frame_Y + frame_height - buttonArrow.getWithHalf(),
                               buttonArrow.getWithHalf()*2,
                               buttonArrow.getWithHalf()*2
                              );
    if( regionBottomRight.contains( event->pos()) )
    {
        QPixmap pixmap( ":/pictures/cursor/size_fdiag.png" );
        QCursor cursor( pixmap );
        setCursor( cursor );
        handleUnderMouse = BottomRight;
        return;
    }

    QRect regionBottomMiddle( frame_X + frame_Width/2 - buttonArrow.getWithHalf(),
                                frame_Y + frame_height - buttonArrow.getWithHalf(),
                                buttonArrow.getWithHalf()*2,
                                buttonArrow.getWithHalf()*2
                               );
    if( regionBottomMiddle.contains( event->pos()) )
    {
        QPixmap pixmap( ":/pictures/cursor/size_ver.png" );
        QCursor cursor( pixmap );
        setCursor( cursor );
        handleUnderMouse = BottomMiddle;
        return;
    }

    QRect regionBottomLeft( frame_X - buttonArrow.getWithHalf(),
                              frame_Y + frame_height - buttonArrow.getWithHalf(),
                              buttonArrow.getWithHalf()*2,
                              buttonArrow.getWithHalf()*2
                            );
    if( regionBottomLeft.contains( event->pos()) )
    {
        QPixmap pixmap( ":/pictures/cursor/size_bdiag.png" );
        QCursor cursor( pixmap );
        setCursor( cursor );
        handleUnderMouse = BottomLeft;
        return;
    }

    QRect regionLeftMiddle( frame_X - buttonArrow.getWithHalf(),
                              frame_Y + frame_height/2 - buttonArrow.getWithHalf(),
                              buttonArrow.getWithHalf()*2,
                              buttonArrow.getWithHalf()*2
                            );
    if( regionLeftMiddle.contains( event->pos()) )
    {
        QPixmap pixmap( ":/pictures/cursor/size_hor.png" );
        QCursor cursor( pixmap );
        setCursor( cursor );
        handleUnderMouse = LeftMiddle;
        return;
    }

    unsetCursor();
    handleUnderMouse = NoHandle;
}


void QvkRegionChoise::printSize( QPainter &painter )
{
  QString widthHeigtSize = QString::number( static_cast<int>( getWidth()/screen->devicePixelRatio() ) ) +
                           " x " +
                           QString::number( static_cast<int>( getHeight()/screen->devicePixelRatio() ) );

  QFont font;
  font.setPointSize( 14 );
  painter.setFont( font );

  QFontMetrics fontMetrics( font );
  pixelWidth = fontMetrics.horizontalAdvance( widthHeigtSize );
  pixelHeight = fontMetrics.height();
  QRect rect( frame_X + frame_Width/2 - pixelWidth/2 - 5,
              frame_Y + frame_height/2 - pixelHeight/2 - 50,
              pixelWidth + 10,
              pixelHeight );

  painter.setBrush( QBrush( Qt::lightGray, Qt::SolidPattern ) );
  painter.setPen( QPen( Qt::black, 2 ) );

  painter.drawRoundedRect( rect, 7, 7 );

  painter.drawText( rect, Qt::AlignCenter, widthHeigtSize );
}


void QvkRegionChoise::drawFrame(QPainter &painter)
{
    QPen pen( getFrameColor(), framePenWidth );
    pen.setJoinStyle( Qt::MiterJoin );
    painter.setPen( pen );
    QBrush brush( Qt::transparent, Qt::SolidPattern);
    painter.setBrush( brush );
    painter.drawRect( frame_X,
                      frame_Y,
                      frame_Width,
                      frame_height);
}


void QvkRegionChoise::setFrameColor( Qt::GlobalColor color )
{
    frameColor = color;
}


Qt::GlobalColor QvkRegionChoise::getFrameColor()
{
    return frameColor;
}


void QvkRegionChoise::HandleRecord( QPainter &painter, int x, int y, int startAngle, int spanAngle )
{
    QBrush brush;
      brush.setColor( Qt::darkGray );
      brush.setStyle( Qt::SolidPattern );
    painter.setBrush( brush );
    QPen pen;
      pen.setColor( Qt::black );
      pen.setWidth( penWidth );
    painter.setPen( pen );
    QRectF rectangle = QRectF( x,
                               y,
                               diameter,
                               diameter);

    painter.drawPie( rectangle, startAngle, spanAngle );
}


void QvkRegionChoise::HandleTopLeft( QPainter &painter )
{
    QvkRegionButtonArrow buttonArrow;
    QColor color;

    if ( handlePressed == TopLeft )
    {
        color = HandleColorByMousePressed;
    }
    else
    {
        color = HandleColorBackground;
    }

    QColor colorSelected;
    if ( HandleSelected == TopLeft )
    {
        colorSelected = colorSelectedArrow;
    }
    else
    {
        colorSelected = Qt::black;
    }

    painter.drawPixmap( frame_X - radius,
                        frame_Y - radius,
                        buttonArrow.getPixmapHandle( color, colorSelected, buttonArrow.topLeft ) );
}


void QvkRegionChoise::HandleTopLeftSize( QPainter &painter)
{
    QFont font;
    font.setPointSize( 10 );
    painter.setFont( font );

    QString string;
    string.append( "  " );
    string.append( "x:" + QString::number( frame_X ) );
    string.append( "  ");
    QFontMetrics fontMetrics( font );
    pixelWidth = fontMetrics.horizontalAdvance( string );
    pixelHeight = fontMetrics.height() * 2;
    QRect rect( frame_X + radius,
                frame_Y + radius,
                pixelWidth,
                pixelHeight );

    painter.setBrush( QBrush( HandleColorBackgroundSize, Qt::SolidPattern ) );
    painter.setPen( QPen( Qt::black, 2 ) );
    painter.drawRoundedRect( rect, 5, 5 );
    painter.drawText( rect, Qt::AlignCenter, "x:" + QString::number( frame_X + framePenHalf + 1 ) +
                                             "\n" +
                                             "y:" + QString::number( frame_Y + framePenHalf + 1 ) );
}


void QvkRegionChoise::HandleTopMiddle( QPainter &painter)
{
    QvkRegionButtonArrow buttonArrow;
    QColor color;

    if ( handlePressed == TopMiddle )
    {
        color = HandleColorByMousePressed;
    }
    else
    {
        color = HandleColorBackground;
    }

    QColor colorSelected;
    if ( HandleSelected == TopMiddle )
    {
        colorSelected = colorSelectedArrow;
    }
    else
    {
        colorSelected = Qt::black;
    }

    painter.drawPixmap( frame_X + frame_Width/2 - buttonArrow.getWithHalf(),
                        frame_Y - buttonArrow.getWithHalf(),
                        buttonArrow.getPixmapHandle( color, colorSelected, buttonArrow.topMiddle ) );
}


void QvkRegionChoise::HandleTopMiddleSize( QPainter &painter )
{
    QFont font;
    font.setPointSize( 10 );
    painter.setFont( font );

    QFontMetrics fontMetrics( font );
    QString string;
    string.append( "  " );
    string.append( "y:" + QString::number( frame_Y ) );
    string.append( "  " );
    pixelWidth = fontMetrics.horizontalAdvance( string);
    pixelHeight = fontMetrics.height();
    QRect rect( frame_X + frame_Width/2 - pixelWidth/2,
                frame_Y + radius + 5,
                pixelWidth,
                pixelHeight );

    painter.setBrush( QBrush( HandleColorBackgroundSize, Qt::SolidPattern ) );
    painter.setPen( QPen( Qt::black, 2 ) );
    painter.drawRoundedRect( rect, 5, 5 );
    painter.drawText( rect, Qt::AlignCenter, "y:" + QString::number( frame_Y + framePenHalf + 1 ) );
}


void QvkRegionChoise::HandleTopRight(QPainter &painter)
{
    QvkRegionButtonArrow buttonArrow;
    QColor color;

    if ( handlePressed == TopRight )
    {
        color = HandleColorByMousePressed;
    }
    else
    {
        color = HandleColorBackground;
    }

    QColor colorSelected;
    if ( HandleSelected == TopRight )
    {
        colorSelected = colorSelectedArrow;
    }
    else
    {
        colorSelected = Qt::black;
    }

    painter.drawPixmap( frame_X + frame_Width - buttonArrow.getWithHalf(),
                        frame_Y - buttonArrow.getWithHalf(),
                        buttonArrow.getPixmapHandle( color, colorSelected, buttonArrow.topRight ) );
}


void QvkRegionChoise::HandleTopRightSize( QPainter &painter )
{
    QFont font;
    font.setPointSize( 10 );
    painter.setFont( font );

    QString string;
    string.append( "  " );
    string.append( "x:" + QString::number( frame_X + frame_Width - framePenHalf ) );
    string.append( "  ");
    QFontMetrics fontMetrics( font );
    pixelWidth = fontMetrics.horizontalAdvance( string );
    pixelHeight = fontMetrics.height() * 2;
    QRect rect( frame_X + frame_Width - pixelWidth - radius,
                frame_Y + radius,
                pixelWidth,
                pixelHeight );

    painter.setBrush( QBrush( HandleColorBackgroundSize, Qt::SolidPattern ) );
    painter.setPen( QPen( Qt::black, 2 ) );
    painter.drawRoundedRect( rect, 5, 5 );
    painter.drawText( rect, Qt::AlignCenter, "x:" + QString::number( frame_X + frame_Width - framePenHalf ) +
                                             "\n" +
                                             "y:" + QString::number( frame_Y + framePenHalf + 1 ) );
}


void QvkRegionChoise::HandleRightMiddle( QPainter &painter )
{
    QvkRegionButtonArrow buttonArrow;
    QColor color;

    if ( handlePressed == RightMiddle )
    {
        color = HandleColorByMousePressed;
    }
    else
    {
        color = HandleColorBackground;
    }

    QColor colorSelected;
    if ( HandleSelected == RightMiddle )
    {
        colorSelected = colorSelectedArrow;
    }
    else
    {
        colorSelected = Qt::black;
    }

    painter.drawPixmap( frame_X + frame_Width - buttonArrow.getWithHalf(),
                        frame_Y + frame_height/2 - buttonArrow.getWithHalf(),
                        buttonArrow.getPixmapHandle( color, colorSelected, buttonArrow.rightMiddle ) );
}


void QvkRegionChoise::HandleRightMiddleSize( QPainter &painter )
{
    QFont font;
    font.setPointSize( 10 );
    painter.setFont( font );

    QString string;
    string.append( "  " );
    string.append( "x:" + QString::number( frame_X + frame_Width - framePenHalf ) );
    string.append( "  ");
    QFontMetrics fontMetrics( font );
    pixelWidth = fontMetrics.horizontalAdvance( string );
    pixelHeight = fontMetrics.height();
    QRect rect( frame_X + frame_Width - pixelWidth - radius - 5,
                frame_Y + frame_height/2 - pixelHeight/2,
                pixelWidth,
                pixelHeight );

    painter.setBrush( QBrush( HandleColorBackgroundSize, Qt::SolidPattern ) );
    painter.setPen( QPen( Qt::black, 2 ) );
    painter.drawRoundedRect( rect, 5, 5 );
    painter.drawText( rect, Qt::AlignCenter, "x:" + QString::number( frame_X + frame_Width - framePenHalf) );
}


void QvkRegionChoise::HandleBottomRight( QPainter &painter )
{
    QvkRegionButtonArrow buttonArrow;
    QColor color;

    if ( handlePressed == BottomRight )
    {
        color = HandleColorByMousePressed;
    }
    else
    {
        color = HandleColorBackground;
    }

    QColor colorSelected;
    if ( HandleSelected == BottomRight )
    {
        colorSelected = colorSelectedArrow;
    }
    else
    {
        colorSelected = Qt::black;
    }

    painter.drawPixmap( frame_X + frame_Width - buttonArrow.getWithHalf(),
                        frame_Y + frame_height - buttonArrow.getWithHalf(),
                        buttonArrow.getPixmapHandle( color, colorSelected, buttonArrow.bottomRight ) );
}


void QvkRegionChoise::HandleBottomRightSize( QPainter &painter )
{
    QvkRegionButtonArrow buttonArrow;
    QFont font;
    font.setPointSize( 10 );
    painter.setFont( font );

    QString string;
    string.append( "  " );
    string.append( "x:" + QString::number( frame_X + frame_Width - framePenHalf) );
    string.append( "  ");
    QFontMetrics fontMetrics( font );
    pixelWidth = fontMetrics.horizontalAdvance( string );
    pixelHeight = fontMetrics.height() * 2;
    QRect rect( frame_X + frame_Width - pixelWidth - radius,
                frame_Y + frame_height - pixelHeight - radius,
                pixelWidth,
                pixelHeight );

    painter.setBrush( QBrush( HandleColorBackgroundSize, Qt::SolidPattern ) );
    painter.setPen( QPen( Qt::black, 2 ) );
    painter.drawRoundedRect( rect, 5, 5 );
    painter.drawText( rect, Qt::AlignCenter, "x:" + QString::number( frame_X + frame_Width - framePenHalf ) +
                                             "\n" +
                                             "y:" + QString::number( frame_Y + frame_height - framePenHalf ) );
}



void QvkRegionChoise::HandleBottomMiddle( QPainter &painter )
{
    QvkRegionButtonArrow buttonArrow;
    QColor color;

    if ( handlePressed == BottomMiddle )
    {
        color = HandleColorByMousePressed;
    }
    else
    {
        color = HandleColorBackground;
    }

    QColor colorSelected;
    if ( HandleSelected == BottomMiddle )
    {
        colorSelected = colorSelectedArrow;
    }
    else
    {
        colorSelected = Qt::black;
    }

    painter.drawPixmap( frame_X + frame_Width/2 - buttonArrow.getWithHalf(),
                        frame_Y + frame_height - buttonArrow.getWithHalf(),
                        buttonArrow.getPixmapHandle( color, colorSelected, buttonArrow.bottomMiddel ) );
}


void QvkRegionChoise::HandleBottomMiddleSize(QPainter &painter )
{
    QFont font;
    font.setPointSize( 10 );
    painter.setFont( font );

    QFontMetrics fontMetrics( font );
    QString string;
    string.append( "  " );
    string.append( "y:" + QString::number( frame_Y ) );
    string.append( "  " );
    pixelWidth = fontMetrics.horizontalAdvance( string);
    pixelHeight = fontMetrics.height();
    QRect rect( frame_X + frame_Width/2 - pixelWidth/2,
                frame_Y + frame_height - pixelHeight - radius - 5,
                pixelWidth,
                pixelHeight );

    painter.setBrush( QBrush( HandleColorBackgroundSize, Qt::SolidPattern ) );
    painter.setPen( QPen( Qt::black, 2 ) );
    painter.drawRoundedRect( rect, 5, 5 );
    painter.drawText( rect, Qt::AlignCenter, "y:" + QString::number( frame_Y + frame_height - framePenHalf ) );
}


void QvkRegionChoise::HandleBottomLeft( QPainter &painter )
{
    QvkRegionButtonArrow buttonArrow;
    QColor color;

    if ( handlePressed == BottomLeft )
    {
        color = HandleColorByMousePressed;
    }
    else
    {
        color = HandleColorBackground;
    }

    QColor colorSelected;
    if ( HandleSelected == BottomLeft )
    {
        colorSelected = colorSelectedArrow;
    }
    else
    {
        colorSelected = Qt::black;
    }

    painter.drawPixmap( frame_X - buttonArrow.getWithHalf(),
                        frame_Y + frame_height - buttonArrow.getWithHalf(),
                        buttonArrow.getPixmapHandle( color, colorSelected, buttonArrow.bottomLeft ) );
}


void QvkRegionChoise::HandleBottomLeftSize( QPainter &painter )
{
    QFont font;
    font.setPointSize( 10 );
    painter.setFont( font );

    QString string;
    string.append( "  " );
    string.append( "x:" + QString::number( frame_X ) );
    string.append( "  ");
    QFontMetrics fontMetrics( font );
    pixelWidth = fontMetrics.horizontalAdvance( string );
    pixelHeight = fontMetrics.height() * 2;
    QRect rect( frame_X + radius,
                frame_Y + frame_height - pixelHeight - radius - 5,
                pixelWidth,
                pixelHeight );

    painter.setBrush( QBrush( HandleColorBackgroundSize, Qt::SolidPattern ) );
    painter.setPen( QPen( Qt::black, 2 ) );
    painter.drawRoundedRect( rect, 5, 5 );
    painter.drawText( rect, Qt::AlignCenter, "x:" + QString::number( frame_X + framePenHalf + 1 ) +
                                             "\n" +
                                             "y:" + QString::number( frame_Y + frame_height - framePenHalf ) );
}


void QvkRegionChoise::HandleLeftMiddle( QPainter &painter )
{
    QvkRegionButtonArrow buttonArrow;
    QColor color;

    if ( handlePressed == LeftMiddle )
    {
        color = HandleColorByMousePressed;
    }
    else
    {
        color = HandleColorBackground;
    }

    QColor colorSelected;
    if ( HandleSelected == LeftMiddle )
    {
        colorSelected = colorSelectedArrow;
    }
    else
    {
        colorSelected = Qt::black;
    }

    painter.drawPixmap( frame_X - buttonArrow.getWithHalf(),
                        frame_Y + frame_height/2 - buttonArrow.getWithHalf(),
                        buttonArrow.getPixmapHandle( color, colorSelected, buttonArrow.leftMiddel ) );
}


void QvkRegionChoise::HandleLeftMiddleSize( QPainter &painter )
{
    QFont font;
    font.setPointSize( 10 );
    painter.setFont( font );

    QString string;
    string.append( "  " );
    string.append( "x:" + QString::number( frame_X ) );
    string.append( "  ");
    QFontMetrics fontMetrics( font );
    pixelWidth = fontMetrics.horizontalAdvance( string );
    pixelHeight = fontMetrics.height();
    QRect rect( frame_X + radius + 5,
                frame_Y + frame_height/2 - pixelHeight/2,
                pixelWidth,
                pixelHeight );

    painter.setBrush( QBrush( HandleColorBackgroundSize, Qt::SolidPattern ) );
    painter.setPen( QPen( Qt::black, 2 ) );
    painter.drawRoundedRect( rect, 5, 5 );
    painter.drawText( rect, Qt::AlignCenter, "x:" + QString::number( frame_X + framePenHalf + 1 ) );
}



void QvkRegionChoise::HandleMiddle( QPainter &painter )
{
    QvkRegionButtonArrow buttonArrow;
    QColor color;

    if ( handlePressed == Middle )
    {
        color = HandleColorByMousePressed;
    }
    else
    {
        color = HandleColorBackground;
    }

    QColor colorSelected;
    if ( HandleSelected == Middle )
    {
        colorSelected = colorSelectedArrow;
    }
    else
    {
        colorSelected = Qt::black;
    }

    painter.drawPixmap( frame_X + frame_Width/2 - buttonArrow.getWithHalf(),
                        frame_Y + frame_height/2 - buttonArrow.getWithHalf(),
                        buttonArrow.getButton( color) );

    painter.drawPixmap( frame_X + frame_Width/2 - buttonArrow.getWithHalf(),
                        frame_Y + frame_height/2 - buttonArrow.getWithHalf(),
                        buttonArrow.getArrow( buttonArrow.degreeArrow::topMiddle, colorSelected ) );

    painter.drawPixmap( frame_X + frame_Width/2 - buttonArrow.getWithHalf(),
                        frame_Y + frame_height/2 - buttonArrow.getWithHalf(),
                        buttonArrow.getArrow( buttonArrow.degreeArrow::rightMiddle, colorSelected ) );

    painter.drawPixmap( frame_X + frame_Width/2 - buttonArrow.getWithHalf(),
                        frame_Y + frame_height/2 - buttonArrow.getWithHalf(),
                        buttonArrow.getArrow( buttonArrow.degreeArrow::bottomMiddel, colorSelected ) );

    painter.drawPixmap( frame_X + frame_Width/2 - buttonArrow.getWithHalf(),
                        frame_Y + frame_height/2 - buttonArrow.getWithHalf(),
                        buttonArrow.getArrow( buttonArrow.degreeArrow::leftMiddel, colorSelected ) );
}


void QvkRegionChoise::recordMode( bool value )
{
    if ( value == true )
    {
        recordemode = true;
        repaint();
        update();
    }
    else
    {
        recordemode = false;
        repaint();
        update();
    }
}

/**
 * Return x from window
 */
int QvkRegionChoise::getX()
{
  return frame_X;
}


/**
 * Return y from window
 */
int QvkRegionChoise::getY()
{
  return  frame_Y;
}


/**
 * Return x from record area
 */
qreal QvkRegionChoise::getXRecordArea()
{
    qreal xReal = ( frame_X + framePenWidth/2 ) * screen->devicePixelRatio();
    int xInt = static_cast<int>( ( frame_X + framePenWidth/2 ) * screen->devicePixelRatio() );

    if ( xReal > xInt )
    {
        xReal = xInt + 1;
    }

    return xReal;
}


/**
 * Return y from record area
 */
qreal QvkRegionChoise::getYRecordArea()
{
   qreal xReal = ( frame_Y + framePenWidth/2 ) * screen->devicePixelRatio();
   int xInt = static_cast<int>( ( frame_Y + framePenWidth/2 ) * screen->devicePixelRatio() );

   if ( xReal > xInt )
   {
       xReal = xInt + 1;
   }

   return xReal;
}


/**
 * Return height from record area
 */
qreal QvkRegionChoise::getHeight()
{
    qreal xReal =   ( ( frame_Y + framePenWidth/2 ) + ( frame_height - framePenWidth ) ) * screen->devicePixelRatio();
    int xInt = static_cast<int>( ( ( frame_Y + framePenWidth/2 ) + ( frame_height - framePenWidth ) ) * screen->devicePixelRatio() );

    if ( xReal > xInt )
    {
        xReal = static_cast<int>( ( frame_height - framePenWidth ) * screen->devicePixelRatio() ) - 1;
    }
    else
    {
        xReal = static_cast<int>( ( frame_height - framePenWidth ) * screen->devicePixelRatio() );
    }

#ifdef Q_OS_WIN
    if ( ( xReal + getYRecordArea() ) == screen->size().height() )
    {
      xReal-=2;
    }
#endif

    return xReal;
}


/**
 * Return width from record Area
 */
qreal QvkRegionChoise::getWidth()
{
    qreal xReal = ( ( frame_X + framePenWidth/2 ) + ( frame_Width - framePenWidth ) ) * screen->devicePixelRatio();
    int xInt = static_cast<int>( ( ( frame_X + framePenWidth/2 ) + ( frame_Width - framePenWidth ) ) * screen->devicePixelRatio() );

    if ( xReal > xInt )
    {
        xReal = static_cast<int>( ( frame_Width - framePenWidth ) * screen->devicePixelRatio() ) - 1;
    }
    else
    {
        xReal = static_cast<int>( ( frame_Width - framePenWidth ) * screen->devicePixelRatio() );
    }

#ifdef Q_OS_WIN
    if ( ( xReal + getXRecordArea() ) == screen->size().width() )
    {
      xReal-=2;
    }
#endif

    return xReal;
}


void QvkRegionChoise::slot_areaReset()
{
    frame_X = 200 - framePenHalf;
    frame_Y = 200 - framePenHalf;
    frame_Width = frame_min_width;
    frame_height = frame_min_height;
    repaint();
    update();
}

void QvkRegionChoise::setX( int x )
{
    frame_X = x - framePenHalf;
    repaint();
    update();
}

void QvkRegionChoise::setY( int y )
{
    frame_Y = y - framePenHalf;
    repaint();
    update();
}


void QvkRegionChoise::setWidth( int width )
{
    frame_Width = width + framePenWidth;
    repaint();
    update();
}

void QvkRegionChoise::setHeight( int height )
{
    frame_height = height + framePenWidth;
    repaint();
    update();
}

