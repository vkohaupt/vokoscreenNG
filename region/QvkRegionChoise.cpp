/* vokoscreen - A desktop recorder
 * Copyright (C) 2011-2018 Volker Kohaupt
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301 USA 
 */

#include "QvkRegionChoise.h"
#include "QvkRegionButtonArrow.h"

#include <QDebug>
#include <QPainter>
#include <QGuiApplication>
#include <QBitmap>
#include <QPaintEvent>
#include <QIcon>
#include <QScreen>

#ifdef Q_OS_LINUX
  #include <QX11Info>
#endif

QvkRegionChoise::QvkRegionChoise():handlePressed(NoHandle),
                                   handleUnderMouse(NoHandle),
                                   framePenWidth(4), // framePenWidth must be an even number
                                   framePenHalf(framePenWidth/2),
                                   radius(20),
                                   diameter(radius*2),
                                   penWidth(2),
                                   penHalf(penWidth/2),
                                   frame_X(800-framePenHalf),
                                   frame_Y(400-framePenHalf),
                                   frame_Width(250 + framePenWidth),
                                   frame_height(250 + framePenWidth),
                                   frame_min_width(250 + framePenWidth),
                                   frame_min_height(250+ framePenWidth),
                                   HandleColorBackground( Qt::green ),
                                   HandleColorBackgroundSize( Qt::yellow ),
                                   HandleColorByMousePressed( Qt::yellow )
{

#ifdef Q_OS_LINUX
    if ( QX11Info::isPlatformX11() == true )
        platform = x11;

    if ( QX11Info::isPlatformX11() == false )
        platform = wayland;
#endif

    // Hint: Qt::WindowStaysOnTopHint is only for X11 on WayLand not do it
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    setAttribute( Qt::WA_TranslucentBackground, true);
    setWindowState( Qt::WindowFullScreen );

    setMouseTracking( true );

    QScreen *screen = QGuiApplication::primaryScreen();
    resize( screen->availableSize().width(), screen->availableSize().height() );
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();
/*
    areaSettingDialog = new QDialog(this);
    areaSettingDialog->setModal( true );
    myUiDialog.setupUi( areaSettingDialog );
    areaSettingDialog->show();

    QPushButton *button = new QPushButton( this );
    button->setGeometry(800, 400, 100, 100);
    button->show();
*/
    show();
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

    QPainter painter;
    painter.begin( this );
    painter.drawPixmap( QPoint( 0, 0 ), pixmap );
    painter.end();

    setMask( pixmap.mask());
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
      case TopLeft     : handlePressed = TopLeft;      break;
      case TopMiddle   : handlePressed = TopMiddle;    break;
      case TopRight    : handlePressed = TopRight;     break;
      case RightMiddle : handlePressed = RightMiddle;  break;
      case BottomRight : handlePressed = BottomRight;  break;
      case BottomMiddle: handlePressed = BottomMiddle; break;
      case BottomLeft  : handlePressed = BottomLeft;   break;
      case LeftMiddle  : handlePressed = LeftMiddle;   break;
      case Middle      : handlePressed = Middle;       break;
    }

    mous_delta_X_to_blueline = event->x() - frame_X;
    mous_delta_Y_to_blueline = event->y() - frame_Y;

    old_Mouse_X = event->x();
    old_Mouse_Y = event->y();
    old_Frame_Width = frame_Width;
    old_Frame_Height = frame_height;

    old_Frame_X2 = frame_X + frame_Width;
    old_Frame_Y2 = frame_Y + frame_height;

    repaint();

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


void QvkRegionChoise::mouseMoveEvent( QMouseEvent *event )
{
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

    //QCoreApplication::processEvents( QEventLoop::AllEvents );

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

    if ( handlePressed != NoHandle )
        return;

    QRegion regionTopLeft( frame_X - radius - 1, frame_Y - radius - 1, diameter + 2, diameter + 2, QRegion::Ellipse );
    if ( regionTopLeft.contains( event->pos() ) )
    {
        setCursor( Qt::SizeFDiagCursor );
        handleUnderMouse = TopLeft;
        return;
    }

    QRegion regionTopMiddle( frame_X + frame_Width/2 - radius - 1, frame_Y - radius - 1, diameter + 2, diameter + 2, QRegion::Ellipse );
    if ( regionTopMiddle.contains( event->pos() )  )
    {
        setCursor( Qt::SizeVerCursor );
        handleUnderMouse = TopMiddle;
        return;
    }

    QRegion regionTopRight( frame_X + frame_Width - radius - 1, frame_Y - radius - 1, diameter + 2, diameter + 2, QRegion::Ellipse );
    if ( regionTopRight.contains( event->pos() )  )
    {
        setCursor( Qt::SizeBDiagCursor );
        handleUnderMouse = TopRight;
        return;
    }

    QRegion regionRightMiddle( frame_X + frame_Width - radius - 1, frame_Y + frame_height/2 - radius - 1, diameter + 2, diameter + 2, QRegion::Ellipse );
    if ( regionRightMiddle.contains( event->pos() )  )
    {
        handleUnderMouse = RightMiddle;
        setCursor( Qt::SizeHorCursor );
        return;
    }

    QRegion regionMiddle( frame_X + frame_Width/2 - radius - penHalf, frame_Y + frame_height/2 - radius - penHalf, 2 * radius + penWidth, 2 * radius + penWidth);
    if ( regionMiddle.contains( event->pos() )  )
    {
        setCursor( Qt::SizeAllCursor );
        handleUnderMouse = Middle;
        return;
    }

    QvkRegionButtonArrow buttonArrow;
    QRegion regionBottomRight( frame_X + frame_Width - buttonArrow.getWithHalf(),
                               frame_Y + frame_height - buttonArrow.getWithHalf(),
                               buttonArrow.getWithHalf()*2,
                               buttonArrow.getWithHalf()*2
                              );
    if( regionBottomRight.contains( event->pos()) )
    {
        setCursor(  Qt::SizeFDiagCursor);
        handleUnderMouse = BottomRight;
        return;
    }

    QRegion regionBottomMiddle( frame_X + frame_Width/2 - buttonArrow.getWithHalf(),
                                frame_Y + frame_height - buttonArrow.getWithHalf(),
                                buttonArrow.getWithHalf()*2,
                                buttonArrow.getWithHalf()*2
                               );
    if( regionBottomMiddle.contains( event->pos()) )
    {
        setCursor( Qt::SizeVerCursor );
        handleUnderMouse = BottomMiddle;
        return;
    }

    QRegion regionBottomLeft( frame_X - buttonArrow.getWithHalf(),
                              frame_Y + frame_height - buttonArrow.getWithHalf(),
                              buttonArrow.getWithHalf()*2,
                              buttonArrow.getWithHalf()*2
                            );
    if( regionBottomLeft.contains( event->pos()) )
    {
        setCursor( Qt::SizeBDiagCursor );
        handleUnderMouse = BottomLeft;
        return;
    }

    QRegion regionLeftMiddle( frame_X - buttonArrow.getWithHalf(),
                              frame_Y + frame_height/2 - buttonArrow.getWithHalf(),
                              buttonArrow.getWithHalf()*2,
                              buttonArrow.getWithHalf()*2
                            );
    if( regionLeftMiddle.contains( event->pos()) )
    {
        setCursor( Qt::SizeHorCursor );
        handleUnderMouse = LeftMiddle;
        return;
    }

    unsetCursor();
    handleUnderMouse = NoHandle;
}


void QvkRegionChoise::printSize( QPainter &painter )
{
  QString widthHeigtSize = QString::number( getWidth() ) + " x " + QString::number( getHeight() );

  QFont font;
  font.setPointSize( 14 );
  painter.setFont( font );

  QFontMetrics fontMetrics( font );
  pixelWidth = fontMetrics.width( widthHeigtSize );
  pixelHeight = fontMetrics.height();
  QRect rect( frame_X + frame_Width/2 - pixelWidth/2 - 5,
              frame_Y + frame_height/2 - pixelHeight/2 - 50,
              pixelWidth + 10,
              pixelHeight );

  painter.setBrush( QBrush( Qt::yellow, Qt::SolidPattern ) );
  painter.setPen( QPen( Qt::black, 2 ) );

  painter.drawRoundedRect( rect, 7, 7 );

  painter.drawText( rect, Qt::AlignCenter, widthHeigtSize );
}


void QvkRegionChoise::drawFrame(QPainter &painter)
{
    QPen pen( Qt::blue, framePenWidth );
    pen.setJoinStyle( Qt::MiterJoin );
    painter.setPen( pen );
    QBrush brush( Qt::transparent, Qt::SolidPattern);
    painter.setBrush( brush );
    painter.drawRect( frame_X,
                      frame_Y,
                      frame_Width,
                      frame_height);
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

    painter.drawPixmap( frame_X - radius,
                        frame_Y - radius,
                        buttonArrow.getPixmapHandle( color, buttonArrow.topLeft ) );
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
    pixelWidth = fontMetrics.width( string );
    pixelHeight = fontMetrics.height() * 2;
    QRect rect( frame_X + radius,
                frame_Y + radius,
                pixelWidth,
                pixelHeight );

    painter.setBrush( QBrush( HandleColorBackgroundSize, Qt::SolidPattern ) );
    painter.setPen( QPen( Qt::black, 2 ) );
    painter.drawRoundedRect( rect, 5, 5 );
    painter.drawText( rect, Qt::AlignCenter, "x:" + QString::number( frame_X + framePenHalf ) +
                                             "\n" +
                                             "y:" + QString::number( frame_Y + framePenHalf ) );
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

    painter.drawPixmap( frame_X + frame_Width/2 - buttonArrow.getWithHalf(),
                        frame_Y - buttonArrow.getWithHalf(),
                        buttonArrow.getPixmapHandle( color, buttonArrow.topMiddle ) );
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
    pixelWidth = fontMetrics.width( string);
    pixelHeight = fontMetrics.height();
    QRect rect( frame_X + frame_Width/2 - pixelWidth/2,
                frame_Y + radius + 5,
                pixelWidth,
                pixelHeight );

    painter.setBrush( QBrush( HandleColorBackgroundSize, Qt::SolidPattern ) );
    painter.setPen( QPen( Qt::black, 2 ) );
    painter.drawRoundedRect( rect, 5, 5 );
    painter.drawText( rect, Qt::AlignCenter, "y:" + QString::number( frame_Y + framePenHalf ) );
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

    painter.drawPixmap( frame_X + frame_Width - buttonArrow.getWithHalf(),
                        frame_Y - buttonArrow.getWithHalf(),
                        buttonArrow.getPixmapHandle( color, buttonArrow.topRight ) );
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
    pixelWidth = fontMetrics.width( string );
    pixelHeight = fontMetrics.height() * 2;
    QRect rect( frame_X + frame_Width - pixelWidth - radius,
                frame_Y + radius,
                pixelWidth,
                pixelHeight );

    painter.setBrush( QBrush( HandleColorBackgroundSize, Qt::SolidPattern ) );
    painter.setPen( QPen( Qt::black, 2 ) );
    painter.drawRoundedRect( rect, 5, 5 );
    painter.drawText( rect, Qt::AlignCenter, "x:" + QString::number( frame_X + frame_Width - framePenHalf) +
                                             "\n" +
                                             "y:" + QString::number( frame_Y + framePenHalf ) );
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

    painter.drawPixmap( frame_X + frame_Width - buttonArrow.getWithHalf(),
                        frame_Y + frame_height/2 - buttonArrow.getWithHalf(),
                        buttonArrow.getPixmapHandle( color, buttonArrow.rightMiddle ) );
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
    pixelWidth = fontMetrics.width( string );
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

    painter.drawPixmap( frame_X + frame_Width - buttonArrow.getWithHalf(),
                        frame_Y + frame_height - buttonArrow.getWithHalf(),
                        buttonArrow.getPixmapHandle( color, buttonArrow.bottomRight ) );
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
    pixelWidth = fontMetrics.width( string );
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

    painter.drawPixmap( frame_X + frame_Width/2 - buttonArrow.getWithHalf(),
                        frame_Y + frame_height - buttonArrow.getWithHalf(),
                        buttonArrow.getPixmapHandle( color, buttonArrow.bottomMiddel ) );
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
    pixelWidth = fontMetrics.width( string);
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

    painter.drawPixmap( frame_X - buttonArrow.getWithHalf(),
                        frame_Y + frame_height - buttonArrow.getWithHalf(),
                        buttonArrow.getPixmapHandle( color, buttonArrow.bottomLeft ) );
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
    pixelWidth = fontMetrics.width( string );
    pixelHeight = fontMetrics.height() * 2;
    QRect rect( frame_X + radius,
                frame_Y + frame_height - pixelHeight - radius - 5,
                pixelWidth,
                pixelHeight );

    painter.setBrush( QBrush( HandleColorBackgroundSize, Qt::SolidPattern ) );
    painter.setPen( QPen( Qt::black, 2 ) );
    painter.drawRoundedRect( rect, 5, 5 );
    painter.drawText( rect, Qt::AlignCenter, "x:" + QString::number( frame_X + framePenHalf) +
                                             "\n" +
                                             "y:" + QString::number( frame_Y + framePenHalf ) );
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

    painter.drawPixmap( frame_X - buttonArrow.getWithHalf(),
                        frame_Y + frame_height/2 - buttonArrow.getWithHalf(),
                        buttonArrow.getPixmapHandle( color, buttonArrow.leftMiddel ) );
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
    pixelWidth = fontMetrics.width( string );
    pixelHeight = fontMetrics.height();
    QRect rect( frame_X + radius + 5,
                frame_Y + frame_height/2 - pixelHeight/2,
                pixelWidth,
                pixelHeight );

    painter.setBrush( QBrush( HandleColorBackgroundSize, Qt::SolidPattern ) );
    painter.setPen( QPen( Qt::black, 2 ) );
    painter.drawRoundedRect( rect, 5, 5 );
    painter.drawText( rect, Qt::AlignCenter, "x:" + QString::number( frame_X + framePenHalf ) );

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

    painter.drawPixmap( frame_X + frame_Width/2 - buttonArrow.getWithHalf(),
                        frame_Y + frame_height/2 - buttonArrow.getWithHalf(),
                        buttonArrow.getButton( color) );

    painter.drawPixmap( frame_X + frame_Width/2 - buttonArrow.getWithHalf(),
                        frame_Y + frame_height/2 - buttonArrow.getWithHalf(),
                        buttonArrow.getArrow( buttonArrow.degreeArrow::topMiddle ) );

    painter.drawPixmap( frame_X + frame_Width/2 - buttonArrow.getWithHalf(),
                        frame_Y + frame_height/2 - buttonArrow.getWithHalf(),
                        buttonArrow.getArrow( buttonArrow.degreeArrow::rightMiddle ) );

    painter.drawPixmap( frame_X + frame_Width/2 - buttonArrow.getWithHalf(),
                        frame_Y + frame_height/2 - buttonArrow.getWithHalf(),
                        buttonArrow.getArrow( buttonArrow.degreeArrow::bottomMiddel ) );

    painter.drawPixmap( frame_X + frame_Width/2 - buttonArrow.getWithHalf(),
                        frame_Y + frame_height/2 - buttonArrow.getWithHalf(),
                        buttonArrow.getArrow( buttonArrow.degreeArrow::leftMiddel ) );

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
int QvkRegionChoise::getXRecordArea()
{
  return frame_X + framePenWidth/2;
}


/**
 * Return y from record area
 */
int QvkRegionChoise::getYRecordArea()
{
  return  frame_Y + framePenWidth/2;
}


/**
 * Return height from record area
 */
int QvkRegionChoise::getHeight()
{
  return frame_height - framePenWidth;
}


/**
 * Return width from record Area
 */
int QvkRegionChoise::getWidth()
{
  return frame_Width - framePenWidth;
}

void QvkRegionChoise::areaReset()
{
  vk_setGeometry( 200,
                  200,
                  250 + framePenWidth,
                  250 + framePenWidth
                );
}
