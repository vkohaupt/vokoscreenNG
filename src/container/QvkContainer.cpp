#include "QvkContainer.h"
#include "global.h"
#include <gst/gst.h>

#include <QStringList>
#include <QList>
#include <QDebug>

/*!
 * Hint:
 * Constructor for any format.
 * QString muxer = "matroskamux".
 * QString suffix = "mkv"
 */
Container::Container( QString muxer, QString suffix )
{
    Container::muxer = muxer;
    Container::suffix = suffix;
}

QString Container::get_Muxer()
{
    return muxer;
}

QString Container::get_Suffix()
{
    return suffix;
}

bool Container::get_Available()
{
    return available;
}

/*!
 * Set the muxer to available or unavailable.
 */
void Container::set_MuxerAvailable( bool available )
{
    Container::available = available;
}

void Container::add_VideoCodec( QString encoder, QString name )
{
#ifdef Q_OS_WIN
    if ( encoder != "x264enc" )
#endif
    {
        VideoCodec videoCodec;
        videoCodec.encoder = encoder;
        videoCodec.name = name;
        VideoCodecs.append( videoCodec );
    }
}

void Container::add_AudioCodec( QString encoder, QString  name )
{
    AudioCodec audioCodec;
    audioCodec.encoder = encoder;
    audioCodec.name = name;
    AudioCodecs.append( audioCodec );
}

void Container::set_VideoCodecAvailable( QString encoder, bool available )
{
    for ( int i = 0; i < VideoCodecs.count(); i++ ) {
        if ( VideoCodecs.at(i).encoder == encoder ) {
            VideoCodec videoCodec = VideoCodecs.at(i);
            videoCodec.available = available;
            VideoCodecs.replace( i, videoCodec );
        }
    }
}

void Container::set_AudioCodecAvailable( QString encoder, bool available )
{
    for ( int i = 0; i < AudioCodecs.count(); i++ ) {
        if ( AudioCodecs.at(i).encoder == encoder ) {
            AudioCodec audioCodec = AudioCodecs.at(i);
            audioCodec.available = available;
            AudioCodecs.replace( i, audioCodec );
        }
    }
}

// ---------------------------------------------------------

/*
 * This is the base for supported container, audio and video codec
 */
QvkContainer::QvkContainer(QObject *parent, bool isFlatpak ) : QObject(parent)
{
#ifdef Q_OS_WIN
    Q_UNUSED(isFlatpak)
#endif

    Container *MKV = new Container( "matroskamux", "mkv" );
    MKV->add_VideoCodec( "openh264enc", "H.264" );
    MKV->add_VideoCodec( "vp8enc", "VP8");
#ifdef Q_OS_UNIX
    if ( isFlatpak == false ) { MKV->add_VideoCodec( "x264enc", "x264"); }
#endif
    MKV->add_AudioCodec( "vorbisenc", "vorbis" );
    MKV->add_AudioCodec( "flacenc", "flac" );
    MKV->add_AudioCodec( "opusenc", "opus" );
    MKV->add_AudioCodec( "lamemp3enc", "mp3" );

    Container *WEBM = new Container( "webmmux", "webm" );
    WEBM->add_VideoCodec( "vp8enc", "VP8" );
    WEBM->add_AudioCodec( "vorbisenc", "vorbis" );
    WEBM->add_AudioCodec( "opusenc", "opus" );

    Container *AVI = new Container( "avimux", "avi" );
    AVI->add_VideoCodec( "openh264enc", "H.264" );
#ifdef Q_OS_UNIX
    if ( isFlatpak == false ) { AVI->add_VideoCodec( "x264enc", "x264" ); }
#endif
    AVI->add_VideoCodec( "vp8enc", "VP8" );
    AVI->add_AudioCodec( "lamemp3enc", "mp3" );

    Container *MP4 = new Container( "mp4mux", "mp4" );
    MP4->add_VideoCodec( "openh264enc", "H.264" );
#ifdef Q_OS_UNIX
    if ( isFlatpak == false ) { MP4->add_VideoCodec( "x264enc", "x264" ); }
#endif
    MP4->add_AudioCodec( "lamemp3enc", "mp3" );
    MP4->add_AudioCodec( "opusenc", "opus" );

    Container *MOV = new Container( "qtmux", "mov" );
    MOV->add_VideoCodec( "openh264enc", "H.264" );
#ifdef Q_OS_UNIX
    if ( isFlatpak == false ) { MOV->add_VideoCodec( "x264enc", "x264" ); }
#endif
    MOV->add_VideoCodec( "vp8enc", "VP8" );
    MOV->add_AudioCodec( "lamemp3enc", "mp3" );


    // Die Abfrage ist nötig damit GIF nicht im Tab erscheint
    // Das Paket gstreamer-plugins-rs enthält den gifencoder und wurde erstmals mit Gstreamer 1.18.0 veröffentlicht
    Container *GIF = Q_NULLPTR;
    if ( QString( gst_version_string() ) >= "GStreamer 1.18.0" ) {
        GIF = new Container( "gifenc", "gif" );
        GIF->add_VideoCodec( "gifenc", "gif" );
    }

    Containers = new QList<Container*>;
    Containers->append( MKV );
    if ( QString( gst_version_string() ) >= "GStreamer 1.18.0" ) {
        Containers->append( GIF );
    }
    Containers->append( MP4 );
    Containers->append( WEBM );
    Containers->append( AVI );
    Containers->append( MOV );
}


/*!
 * Return all supported container like mkv, avi, ... as QList.
 */
QList<Container*> QvkContainer::get_Containers()
{
    return *Containers;
}


/*!
 *  Return all supported video-encoder as QStringList
 */
QStringList QvkContainer::get_AllVideoEncoders()
{
    QList<Container::VideoCodec> list;
    for ( int i = 0; i < Containers->count(); i++ ) {
        list << Containers->at(i)->VideoCodecs;
    }

    QStringList stringList;
    for ( int i = 0; i < list.count(); i++ ) {
        if ( stringList.contains( list.at(i).encoder ) == false ) {
            stringList << list.at(i).encoder;
        }
    }

    return stringList;
}


/*!
 *  Return all supported audio-encoder as QStringList
 */
QStringList QvkContainer::get_AllAudioEncoders()
{
    QList<Container::AudioCodec> list;
    for ( int i = 0; i < Containers->count(); i++ ) {
        list << Containers->at(i)->AudioCodecs;
    }

    QStringList stringList;
    for ( int i = 0; i < list.count(); i++ ) {
        if ( stringList.contains( list.at(i).encoder ) == false ) {
            stringList << list.at(i).encoder;
        }
    }

    return stringList;
}

/*!
 * Input:
 * QString value = "mkv" or "webm" or ...
 *
 * Return:
 * All supported video encoder like vp8enc, avienc, ... as QList
 */
QList<Container::VideoCodec> QvkContainer::get_VideoCodecs( QString suffix )
{
    QList<Container::VideoCodec> list;
    for ( int i = 0; i < Containers->count(); i++ ) {
        if ( Containers->at(i)->get_Suffix() == suffix ) {
            list << Containers->at(i)->VideoCodecs;
        }
    }
    return list;
}


/*!
 * Input:
 * QString value = "mkv" or "webm" or ...
 *
 * Return:
 * All supported audio encoder like vorbisenc, opusenc, ... as QList
 */
QList<Container::AudioCodec> QvkContainer::get_AudioCodecs( QString suffix )
{
    QList<Container::AudioCodec> list;
    for ( int i = 0; i < Containers->count(); i++ ) {
        if ( Containers->at(i)->get_Suffix() == suffix ) {
            list << Containers->at(i)->AudioCodecs;
        }
    }
    return list;
}

/*!
 * Input:
 * Set the container to available or unavailable.
 * QString muxer = "matroskamux".
 */
void QvkContainer::set_ContainerAvailable( QString muxer, bool available  )
{
    for ( int i = 0; i < Containers->count(); i++ ) {
        if ( Containers->at(i)->get_Muxer() == muxer ) {
            Containers->at(i)->set_MuxerAvailable( available );
        }
    }
}
