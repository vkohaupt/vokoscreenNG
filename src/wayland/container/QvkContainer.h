#ifndef QVKCONTAINER_H
#define QVKCONTAINER_H

#include <QObject>

class Container
{
public:
    class VideoCodec
    {
    public:
        QString encoder = "";
        QString name = "";
        bool available = false;
    };

    class AudioCodec
    {
    public:
        QString encoder = "";
        QString name = "";
        bool available = false;
    };

    Container( QString muxer, QString suffix );
    QString get_Muxer();
    QString get_Suffix();
    bool get_Available();
    void set_MuxerAvailable( bool available );

    void add_VideoCodec( QString encoder, QString  name );
    void add_AudioCodec( QString encoder, QString  name );
    void set_VideoCodecAvailable( QString encoder, bool available );
    void set_AudioCodecAvailable( QString encoder, bool available );

    QList<VideoCodec> VideoCodecs;
    QList<AudioCodec> AudioCodecs;

private:
    QString muxer = "";
    QString suffix = "";
    bool available = false;

};


class QvkContainer : public QObject
{
    Q_OBJECT
public:
    explicit QvkContainer(QObject *parent = nullptr);
    QList<Container*> get_Containers();
    QStringList get_AllVideoEncoders();
    QStringList get_AllAudioEncoders();
    QList<Container::VideoCodec> get_VideoCodecs( QString suffix );
    QList<Container::AudioCodec> get_AudioCodecs( QString suffix );
    void set_ContainerAvailable( QString muxer, bool available );

private:
    QList<Container*> *Containers;

signals:

};

#endif // QVKCONTAINER_H
