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

#ifndef QVKVIDEOSURFACE_H
#define QVKVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QDebug>

class QvkVideoSurface: public QAbstractVideoSurface
{
  Q_OBJECT
  public:
    QvkVideoSurface(QObject * parent=Q_NULLPTR) : QAbstractVideoSurface(parent)
    {}

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const
    {
        (void)type;
        return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB32
                                                 << QVideoFrame::Format_ARGB32
                                                 << QVideoFrame::Format_ARGB32_Premultiplied
                                                 << QVideoFrame::Format_ARGB8565_Premultiplied
                                                 << QVideoFrame::Format_RGB555;
    }

    bool present(const QVideoFrame &frame)
    {
        if ( frame.isValid() )
        {
                QVideoFrame cloneFrame(frame);
                cloneFrame.map( QAbstractVideoBuffer::ReadOnly );
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
