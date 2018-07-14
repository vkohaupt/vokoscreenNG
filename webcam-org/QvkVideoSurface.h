#ifndef QvkVideoSurface_H
#define QvkVideoSurface_H

#include <QAbstractVideoSurface>

class QvkVideoSurface: public QAbstractVideoSurface
{
  Q_OBJECT
  public:
    QvkVideoSurface(QObject * parent=NULL) : QAbstractVideoSurface(parent)
    {}

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const
    {
       (void)type;
       return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB32;
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
                emit ( newPicture( image ) );
                return true;
        }
        return false;
    }

signals:
    void newPicture( QImage );


};

#endif
