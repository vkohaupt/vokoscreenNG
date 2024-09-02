#include "portal_wl.h"
#include "global.h"

#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusUnixFileDescriptor>
#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusObjectPath>
#include <QMap>
#include <QVariant>
#include <QVariantMap>
#include <QDebug>

Q_DECLARE_METATYPE(Portal_wl::Stream)
Q_DECLARE_METATYPE(Portal_wl::Streams)

const QDBusArgument &operator >> ( const QDBusArgument &arg, Portal_wl::Stream &stream )
{
    arg.beginStructure();
    arg >> stream.node_id;

    arg.beginMap();

    while ( !arg.atEnd() ) {
        QString key;
        QVariant value;
        arg.beginMapEntry();
        arg >> key >> value;
        arg.endMapEntry();
        stream.map.insert( key, value );
    }

    arg.endMap();
    arg.endStructure();

    return arg;
}

Portal_wl::Portal_wl( QObject* parent )
    : QObject{ parent }
{
}

Portal_wl::~Portal_wl()
{
    if ( mScreencastPortal ) {
        delete mScreencastPortal;
    }
}

void Portal_wl::slot_startScreenCast( uint sourceType, bool withCursor )
{
    mWithCursor = withCursor;
    mSourcType = sourceType;

    QDBusInterface* portal = screencastPortal();

    if ( !portal ) {
        return;
    }

    // Create ScreenCast session
    QString requestToken = createRequestToken();
    QMap<QString, QVariant> options;
    options["session_handle_token"] = createSessionToken();
    options["handle_token"] = requestToken;

    QDBusConnection::sessionBus().connect( "",
                                           mRequestPath + requestToken,
                                           "org.freedesktop.portal.Request",
                                           "Response",
                                           "ua{sv}",
                                           this,
                                           SLOT(slot_handleCreateSessionResponse(uint,QMap<QString,QVariant>)));

    const QDBusReply<QDBusObjectPath> reply = portal->call("CreateSession", options);

    if ( !reply.isValid() ) {
        qDebug().noquote() << global::nameOutput << "Couldn't get reply to ScreenCast/CreateSession";
        qDebug().noquote() << global::nameOutput << "Error: " << reply.error().message();
        emit signal_portal_aborted();
        return;
    }
}

void Portal_wl::slot_stopScreenCast()
{
    if ( mSession.isEmpty() ) {
        return;
    }

    QDBusInterface portal( "org.freedesktop.portal.Desktop", mSession, "org.freedesktop.portal.Session" );

    if ( portal.isValid() ) {
        const QDBusReply<void> reply = portal.call( "Close" );

        if ( !reply.isValid() ) {
            qDebug().noquote() << global::nameOutput << "Couldn't get reply to ScreenCast/Close";
            qDebug().noquote() << global::nameOutput << "Error: " << reply.error().message();
            mSession.clear();
            return;
        }
    }
}

void Portal_wl::slot_handleCreateSessionResponse( uint response, const QVariantMap& results )
{
    if ( response != 0 ) {
        qDebug().noquote() << global::nameOutput << "Failed to create session: " << response << results;
        emit signal_portal_aborted();
        return;
    }

    mSession = results.value( "session_handle" ).toString();

    QDBusInterface* portal = screencastPortal();

    if ( !portal ) {
        return;
    }

    // Select sources
    QString requestToken = createRequestToken();
    QMap<QString, QVariant> options;
    options["multiple"] = false;
    options["types"] = mSourcType;
    options["cursor_mode"] = mWithCursor ? EMBEDDED : HIDDEN;
    options["handle_token"] = requestToken;
    options["persist_mode"] = TRANSIENT;

    if ( !mRestoreToken.isEmpty() ) {
        options["restore_token"] = mRestoreToken;
    }

    // connect before call
    QDBusConnection::sessionBus().connect( "",
                                           mRequestPath + requestToken,
                                           "org.freedesktop.portal.Request",
                                           "Response",
                                           "ua{sv}",
                                           this,
                                           SLOT(slot_handleSelectSourcesResponse(uint,QMap<QString,QVariant>)));

    const QDBusReply<QDBusObjectPath> reply = portal->call( "SelectSources", QDBusObjectPath(mSession), options );

    if ( !reply.isValid() ) {
        qDebug().noquote() << global::nameOutput << "Couldn't get reply";
        qDebug().noquote() << global::nameOutput << "Error: " << reply.error().message();
        emit signal_portal_aborted();
        return;
    }
}

void Portal_wl::slot_handleSelectSourcesResponse( uint response, const QVariantMap& results )
{
    Q_UNUSED(results);

    if ( response != 0 ) {
        qDebug().noquote() << global::nameOutput << "Failed to select sources: " << response;
        emit signal_portal_aborted();
        return;
    }

    QDBusInterface* portal = screencastPortal();

    if ( !portal ) {
        return;
    }

    // Start ScreenCast
    QString requestToken = createRequestToken();
    QMap<QString, QVariant> options;
    options["handle_token"] = requestToken;

    QDBusConnection::sessionBus().connect( "",
                                           mRequestPath + requestToken,
                                           "org.freedesktop.portal.Request",
                                           "Response",
                                           "ua{sv}",
                                           this,
                                           SLOT(slot_handleStartResponse(uint,QMap<QString,QVariant>)));

    const QDBusReply<QDBusObjectPath> reply = portal->call( "Start", QDBusObjectPath(mSession), "", options );

    if ( !reply.isValid() ) {
        qDebug().noquote() << global::nameOutput << "Couldn't get reply";
        qDebug().noquote() << global::nameOutput << "Error: " << reply.error().message();
        emit signal_portal_aborted();
        return;
    }
}

void Portal_wl::slot_handleStartResponse( uint response, const QVariantMap& results )
{
    Q_UNUSED(results);

    if ( response != 0 ) {
        qDebug().noquote() << global::nameOutput << "Failed to start or cancel dialog: " << response;
        emit signal_portal_aborted();
        return;
    }

    // save restore token
    mRestoreToken = results.value( "restore_token" ).toString();

    const Streams streams = qdbus_cast<Streams>( results.value( "streams" ) );
    const Stream stream = streams.last();

    QDBusInterface* portal = screencastPortal();

    if ( !portal ) {
        return;
    }

    // Open PipeWire Remote
    QMap<QString, QVariant> options;
    const QDBusReply<QDBusUnixFileDescriptor> reply = portal->call( "OpenPipeWireRemote", QDBusObjectPath(mSession), options );

    if ( !reply.isValid() ) {
        qDebug().noquote() << global::nameOutput << "Couldn't get reply";
        qDebug().noquote() << global::nameOutput << "Error: " << reply.error().message();
        emit signal_portal_aborted();
        return;
    }

    const QString fd = QString::number( reply.value().fileDescriptor() );
    const QString path = QString::number( stream.node_id );

    emit signal_portal_fd_path( fd, path );
}

QDBusInterface* Portal_wl::screencastPortal()
{
    if ( !mScreencastPortal ) {
        mScreencastPortal = new QDBusInterface( "org.freedesktop.portal.Desktop",
                                                "/org/freedesktop/portal/desktop",
                                                "org.freedesktop.portal.ScreenCast");
        mScreencastPortal->setParent(this);

        mRequestPath = "/org/freedesktop/portal/desktop/request/" + mScreencastPortal->connection().baseService().remove(0, 1).replace('.', '_') + "/";
        qDebug().noquote() << global::nameOutput << "request path" << mRequestPath;
    }

    if ( mScreencastPortal->isValid() ) {
        return mScreencastPortal;
    }

    emit signal_portal_aborted();
    return nullptr;
}

QString Portal_wl::createSessionToken() const
{
    static int sessionTokenCounter = 0;

    sessionTokenCounter += 1;
    return QString( "vosess%1" ).arg( sessionTokenCounter );

}

QString Portal_wl::createRequestToken() const
{
    static int requestTokenCounter = 0;

    requestTokenCounter += 1;
    return QString( "voreq%1" ).arg( requestTokenCounter );
}
