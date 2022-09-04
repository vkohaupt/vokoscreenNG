/* vokoscreenNG
 * Copyright (C) 2017-2022 Volker Kohaupt
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

#include "QvkGlobalMouse.h"

#include <QDebug>

#ifdef Q_OS_WIN
#include <QCursor>
#include <Windows.h>
#endif

#ifdef Q_OS_LINUX
#include<X11/Xlib.h>
#include<stdio.h>
#endif

QvkGlobalMouse::QvkGlobalMouse()
{
    timer = new QTimer;
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( 10 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_mousePressed() ) );
}


QvkGlobalMouse::~QvkGlobalMouse()
{
}


void QvkGlobalMouse::slot_on( bool value )
{
    if ( value == true )
    {
        timer->start();
    }

    if ( value == false )
    {
        timer->stop();
    }
}


#ifdef Q_OS_LINUX
void QvkGlobalMouse::slot_mousePressed()
{
    Display* display;
    Window root;
    display = XOpenDisplay( NULL );
    root = DefaultRootWindow( display);
    Window root_return, child_return;
    int x, y;
    int win_x, win_y;
    unsigned int mask;

    XQueryPointer( display, root, &root_return, &child_return, &x, &y, &win_x, &win_y, &mask );
    if ( ( mask & Button1Mask) | ( mask & Button2Mask ) | ( mask & Button3Mask ) )
    {
        if ( pressed == 0 )
        {
            QString mouseButton;
            if (  mask & Button1Mask  )
            {
                mouseButton = "LeftButton";
            }

            if (  mask & Button2Mask  )
            {
                mouseButton = "MiddleButton";
            }

            if ( mask & Button3Mask )
            {
                mouseButton = "RightButton";
            }

            pressed = 1;
            emit signal_mousePressed( win_x, win_y, mouseButton );
        }
    }
    else
    {
        pressed = 0;
    }

    fflush(stdout);

    XCloseDisplay( display );
}
#endif


#ifdef Q_OS_WIN
HHOOK hHook = NULL;
QString Mouse_Button;
int Mouse_X;
int Mouse_Y;
int pressed = 0;

LRESULT CALLBACK MouseProc( int nCode, WPARAM wParam, LPARAM lParam )
{
    (void)lParam;
    (void)nCode;

    QPoint point = QCursor::pos();

    switch( wParam )
    {
    case WM_LBUTTONDOWN:
    {
        Mouse_Button = "LeftButton";
        Mouse_X = point.x();
        Mouse_Y = point.y();
        pressed = 1;
        break;
    }
    case WM_RBUTTONDOWN:
    {
        Mouse_Button = "RightButton";
        Mouse_X = point.x();
        Mouse_Y = point.y();
        pressed = 1;
        break;
    }
    case WM_MBUTTONUP:
    {
        Mouse_Button = "MiddleButton";
        Mouse_X = point.x();
        Mouse_Y = point.y();
        pressed = 1;
        break; }
    }

    return CallNextHookEx( hHook, nCode, wParam, lParam );
}

void QvkGlobalMouse::slot_mousePressed()
{
    if ( once == false )
    {
        hHook = SetWindowsHookEx( WH_MOUSE_LL, MouseProc, NULL, 0 );
        once = true;
    }

    if ( hHook == NULL )
    {
        qDebug() << "Hook failed";
    }

    if ( pressed == 1 )
    {
        //bool bol = UnhookWindowsHookEx( hHook );

        pressed = 0;
        emit signal_mousePressed( Mouse_X, Mouse_Y, Mouse_Button );
    }
}
#endif
