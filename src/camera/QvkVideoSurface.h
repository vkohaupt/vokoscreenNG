#ifndef QvkVideoSurface_H
#define QvkVideoSurface_H

#include <QAbstractVideoSurface>
#include <QDebug>

class QvkVideoSurface: public QAbstractVideoSurface
{
  Q_OBJECT
  public:
    QvkVideoSurface(QObject * parent=NULL) : QAbstractVideoSurface(parent)
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
