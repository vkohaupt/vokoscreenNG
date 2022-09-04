/* vokoscreenNG - A desktop recorder
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

#ifndef QVKPLAYERVIDEOSURFACE_H
#define QVKPLAYERVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QDebug>

class QvkPlayerVideoSurface: public QAbstractVideoSurface
{
  Q_OBJECT
  public:
    QvkPlayerVideoSurface(QObject * parent = Q_NULLPTR ) : QAbstractVideoSurface(parent)
    {}

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const
    {
       (void)type;
#ifdef Q_OS_LINUX
       return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB24;
#endif

#ifdef Q_OS_WIN
       return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB32;
#endif
    }

    bool present(const QVideoFrame &frame)
    {
        if (frame.isValid() )
        {
                QVideoFrame cloneFrame(frame);

                cloneFrame.map(QAbstractVideoBuffer::ReadOnly);

                const QImage image( cloneFrame.bits(),
                                    cloneFrame.width(),
                                    cloneFrame.height(),
                                    QVideoFrame::imageFormatFromPixelFormat( cloneFrame.pixelFormat() )
                                  );

                cloneFrame.unmap();
                emit ( signal_newPicture( image ) );
                return true;
        }
        return false;
    }

signals:
    void signal_newPicture( QImage );


};

#endif
