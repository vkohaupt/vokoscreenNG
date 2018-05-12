#include "qxtwindowsystem.h"
/****************************************************************************
** Copyright (c) 2006 - 2011, the LibQxt project.
** See the Qxt AUTHORS file for a list of authors and copyright holders.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the LibQxt project nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** <http://libqxt.org>  <foundation@libqxt.org>
*****************************************************************************/

#include <QX11Info>
#include <QRect>
#include <X11/Xutil.h>

static WindowList qxt_getWindows(Atom prop)
{
    WindowList res;
    Atom type = 0;
    int format = 0;
    uchar* data = 0;
    ulong count, after;
    Display* display = QX11Info::display();
    Window window = QX11Info::appRootWindow();
    if (XGetWindowProperty(display, window, prop, 0, 1024 * sizeof(Window) / 4, False, AnyPropertyType,
                           &type, &format, &count, &after, &data) == Success)
    {
        Window* list = reinterpret_cast<Window*>(data);
        for (uint i = 0; i < count; ++i)
            res += list[i];
        if (data)
            XFree(data);
    }
    return res;
}

WindowList QxtWindowSystem::windows()
{
    static Atom net_clients = 0;
    if (!net_clients)
        net_clients = XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", True);

    return qxt_getWindows(net_clients);
}

WId QxtWindowSystem::activeWindow()
{
    static Atom net_active = 0;
    if (!net_active)
        net_active = XInternAtom(QX11Info::display(), "_NET_ACTIVE_WINDOW", True);

    return qxt_getWindows(net_active).value(0);
}

QRect QxtWindowSystem::windowGeometryWithoutFrame( WId child)
{
    int x, y;
    Window root;
    uint w, h, border, depth;

    XGetGeometry( QX11Info::display(), child, &root, &x, &y, &w, &h, &border, &depth );

    Window parent;
    Window* children;
    unsigned int nchildren;

    if( XQueryTree( QX11Info::display(), child, &root, &parent, &children, &nchildren ) != 0 )
    {
        if( children != NULL )
        {
            XFree( children );
        }

        int newx, newy;
        Window dummy;

        if( XTranslateCoordinates( QX11Info::display(), parent, QX11Info::appRootWindow(), x, y, &newx, &newy, &dummy ))
        {
            x = newx;
            y = newy;
        }
    }

    QRect rect( x, y, w, h );

    return rect;
}


QRect QxtWindowSystem::windowGeometryWithFrame(WId window)
{
    int x, y;
    uint width, height, border, depth;
    Window root, child;
    Display* display = QX11Info::display();
    XGetGeometry(display, window, &root, &x, &y, &width, &height, &border, &depth);
    XTranslateCoordinates(display, window, root, x, y, &x, &y, &child);

    static Atom net_frame = 0;
    if (!net_frame)
        net_frame = XInternAtom(QX11Info::display(), "_NET_FRAME_EXTENTS", True);

    QRect rect(x, y, width, height);
    Atom type = 0;
    int format = 0;
    uchar* data = 0;
    ulong count, after;
    if (XGetWindowProperty(display, window, net_frame, 0, 4, False, AnyPropertyType,
                           &type, &format, &count, &after, &data) == Success)
    {
        // _NET_FRAME_EXTENTS, left, right, top, bottom, CARDINAL[4]/32
        if (count == 4)
        {
            long* extents = reinterpret_cast<long*>(data);
            rect.adjust(-extents[0], -extents[2], extents[1], extents[3]);
        }
        if (data)
            XFree(data);
    }
    return rect;
}
