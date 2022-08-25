/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
 * 
 * Author:
 *      Volker Kohaupt <vkohaupt@volkoh.de>
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

#include "QvkWinInfo.h"

#include <QBitmap>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>

#ifdef Q_OS_LINUX
#include <QX11Info>
#include <X11/Xutil.h>
#endif

#ifdef Q_OS_WIN
#include <windows.h>
#endif

QvkWinInfo::QvkWinInfo()
{

}


void QvkWinInfo::slot_start()
{
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    setAttribute( Qt::WA_TranslucentBackground, true );

    resize ( 50, 50 );

    lastWinID = this->winId();

    mouseTimer = new QTimer( this );
    connect( mouseTimer, SIGNAL( timeout() ), this, SLOT( slot_mousePosition() ) );
    mouseTimer->start( 10 );

    vkGlobalMouse = new QvkGlobalMouse;
    vkGlobalMouse->slot_on( true );
    connect( vkGlobalMouse, SIGNAL( signal_mousePressed( int, int, QString ) ), this, SLOT( slot_selectWindow() ) );

    show();
    emit signal_showCursor( true );
}


QvkWinInfo::~QvkWinInfo()
{  
}


void QvkWinInfo::paintEvent( QPaintEvent *event ) 
{
    Q_UNUSED(event);

    QPixmap pixmap( 50, 50 );
    pixmap.fill( Qt::transparent );
    QPainter painter;
    painter.begin( &pixmap );
    painter.setPen( QPen( Qt::blue, 4 ) );
    painter.drawLine( 50/2, 0, 50/2, 15 );
    painter.drawLine( 50/2, 50/2+10, 50/2, 50 );
    painter.drawLine( 0, 50/2, 50/2-10, 50/2 );
    painter.drawLine( 50/2+10, 50/2, 50, 50/2 );
    painter.end();

    QPainter painter_1;
    painter_1.begin( this );
    painter_1.drawPixmap( QPoint( 0, 0 ), pixmap );
    painter_1.end();

    setMask( pixmap.mask() );
}


void QvkWinInfo::slot_mousePosition()
{
    QCursor cursor;
    move( cursor.pos().x() - 25 , cursor.pos().y() - 25 );
}


#ifdef Q_OS_LINUX
static WindowList getWindow( Atom prop )
{
    WindowList res;
    Atom type = 0;
    int format = 0;
    uchar* data = Q_NULLPTR;
    ulong count, after;

    Display *display = XOpenDisplay( NULL );
    Window window = QX11Info::appRootWindow();
    if ( XGetWindowProperty( display, window, prop, 0, 1024 * sizeof(Window) / 4, False, AnyPropertyType,
                             &type, &format, &count, &after, &data ) == Success )
    {
        Window* list = reinterpret_cast<Window*>( data ) ;
        for ( uint i = 0; i < count; ++i )
            res += list[i];
        if (data)
            XFree( data );
    }

    XCloseDisplay( display );

    return res;
}


WId QvkWinInfo::activeWindow()
{
    static Atom net_active = 0;
    if ( !net_active ) {
        Display *display = XOpenDisplay( NULL );
        net_active = XInternAtom( display, "_NET_ACTIVE_WINDOW", True );
        XCloseDisplay( display );
    }

    return getWindow( net_active ).value(0);
}


QRectF QvkWinInfo::windowGeometryWithoutFrame( WId child)
{
    int x, y;
    Window root;
    uint w, h, border, depth;

    Display *display = XOpenDisplay( NULL );
    XGetGeometry( display, child, &root, &x, &y, &w, &h, &border, &depth );

    Window parent;
    Window* children;
    unsigned int nchildren;

    if( XQueryTree( display, child, &root, &parent, &children, &nchildren ) != 0 )
    {
        if( children != nullptr )
        {
            XFree( children );
        }

        int newx, newy;
        Window dummy;

        if( XTranslateCoordinates( display, parent, QX11Info::appRootWindow(), x, y, &newx, &newy, &dummy ))
        {
            x = newx;
            y = newy;
        }
    }

    XCloseDisplay( display );

    QRectF rect( x, y, w, h );

    return rect;
}
#endif


#ifdef Q_OS_WIN
WId QvkWinInfo::activeWindow()
{
    return (WId)GetForegroundWindow();
}
#endif


void QvkWinInfo::slot_selectWindow()
{
    newWinID = activeWindow();

    if ( lastWinID != newWinID )
    {
        mouseTimer->stop();
        vkGlobalMouse->slot_on( false );

        // Cursor resize does not show in video in the first Frames
        resize( 10, 10 );

        emit signal_windowChanged( true );
        this->close();
    }
}


WId QvkWinInfo::getWinID()
{
    return newWinID;
}


