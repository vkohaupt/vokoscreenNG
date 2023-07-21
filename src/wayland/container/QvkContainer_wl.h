#ifndef QVKCONTAINER_WL_H
#define QVKCONTAINER_WL_H

#include <QObject>

class Container_wl
{
public:
    class VideoCodec_wl
    {
    public:
        QString encoder = "";
        QString name = "";
        bool available = false;
    };

    class AudioCodec_wl
    {
    public:
        QString encoder = "";
        QString name = "";
        bool available = false;
    };

    Container_wl( QString muxer, QString suffix );
    QString get_Muxer();
    QString get_Suffix();
    bool get_Available();
    void set_MuxerAvailable( bool available );

    void add_VideoCodec( QString encoder, QString  name );
    void add_AudioCodec( QString encoder, QString  name );
    void set_VideoCodecAvailable( QString encoder, bool available );
    void set_AudioCodecAvailable( QString encoder, bool available );

    QList<VideoCodec_wl> VideoCodecs;
    QList<AudioCodec_wl> AudioCodecs;

private:
    QString muxer = "";
    QString suffix = "";
    bool available = false;

};


class QvkContainer_wl : public QObject
{
    Q_OBJECT
public:
    explicit QvkContainer_wl(QObject *parent = nullptr);
    QList<Container_wl*> get_Containers();
    QStringList get_AllVideoEncoders();
    QStringList get_AllAudioEncoders();
    QList<Container_wl::VideoCodec_wl> get_VideoCodecs( QString suffix );
    QList<Container_wl::AudioCodec_wl> get_AudioCodecs( QString suffix );
    void set_ContainerAvailable( QString muxer, bool available );


private:
    QList<Container_wl*> *Containers;


signals:


};

#endif // QVKCONTAINER_WL_H
