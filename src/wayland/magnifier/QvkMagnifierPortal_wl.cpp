#include "QvkMagnifierPortal_wl.h"
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

Q_DECLARE_METATYPE(QvkMagnifierPortal_wl::Stream)
Q_DECLARE_METATYPE(QvkMagnifierPortal_wl::Streams)

const QDBusArgument &operator >> ( const QDBusArgument &arg, QvkMagnifierPortal_wl::Stream &stream )
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

QvkMagnifierPortal_wl::QvkMagnifierPortal_wl( QObject* parent )
    : QObject{ parent }
{
}

QvkMagnifierPortal_wl::~QvkMagnifierPortal_wl()
{
    if ( mScreencastPortal ) {
        delete mScreencastPortal;
    }
}

void QvkMagnifierPortal_wl::slot_startScreenCast(uint m_sourceType, uint m_cursorModes )
{
    mCursorModes = m_cursorModes;
    mSourcType = m_sourceType;

    QDBusInterface* portal = screencastPortal();
    if ( !portal ) {
        qDebug().noquote() << global::nameOutput << "Aborted in function: Portal_wl::slot_startScreenCast";
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

void QvkMagnifierPortal_wl::slot_stopScreenCast()
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

void QvkMagnifierPortal_wl::slot_handleCreateSessionResponse( uint response, const QVariantMap& results )
{
    if ( response != 0 ) {
        qDebug().noquote() << global::nameOutput << "Failed to create session: " << response << results;
        emit signal_portal_aborted();
        return;
    }

    mSession = results.value( "session_handle" ).toString();

    // Auf dem Desktop wurde z.b im KDE auf den roten Knopf gelickt so das die Aufnahme gestoppt wird
    QDBusConnection::sessionBus().connect("org.freedesktop.portal.Desktop",
                                          mSession,
                                          "org.freedesktop.portal.Session",
                                          "Closed",
                                          this,
                                          SLOT(slot_handleSessionClosed()));

    QDBusInterface* portal = screencastPortal();
    if ( !portal ) {
        qDebug().noquote() << global::nameOutput << "Aborted in function: Portal_wl::slot_handleCreateSessionResponse";
        return;
    }

    // Select sources
    QString requestToken = createRequestToken();
    QMap<QString, QVariant> options;
    options["multiple"] = false;
    options["types"] = mSourcType;
    options["cursor_mode"] = mCursorModes;
    options["handle_token"] = requestToken;
    options["persist_mode"] = PERSISTENT;

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
        qDebug().noquote() << global::nameOutput << "Couldn't get reply in Portal_wl::slot_handleCreateSessionResponse";
        qDebug().noquote() << global::nameOutput << "Error: " << reply.error().message();
        emit signal_portal_aborted();
        return;
    }
}


void QvkMagnifierPortal_wl::slot_handleSessionClosed()
{
    qDebug().noquote() << global::nameOutput << "[Magnifier][Portal] Die Screencast-Sitzung wurde vom Desktop aus gestoppt";
    mSession.clear();
    emit signal_portal_stopped();
}


void QvkMagnifierPortal_wl::slot_handleSelectSourcesResponse( uint response, const QVariantMap& results )
{
    Q_UNUSED(results);

    if ( response != 0 ) {
        qDebug().noquote() << global::nameOutput << "Failed to select sources: " << response;
        emit signal_portal_aborted();
        return;
    }

    QDBusInterface* portal = screencastPortal();
    if ( !portal ) {
        qDebug().noquote() << global::nameOutput << "Aborted in function: Portal_wl::slot_handleSelectSourcesResponse";
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
        qDebug().noquote() << global::nameOutput << "Couldn't get reply in Portal_wl::slot_handleSelectSourcesResponse";
        qDebug().noquote() << global::nameOutput << "Error: " << reply.error().message();
        emit signal_portal_aborted();
        return;
    }
}

void QvkMagnifierPortal_wl::slot_handleStartResponse( uint response, const QVariantMap& results )
{
    Q_UNUSED(results);

    if ( response != 0 ) {
        qDebug().noquote() << global::nameOutput << "[Magnifier]Failed to start or cancel dialog: " << response;
        emit signal_portal_aborted();
        return;
    }

    // save restore token
    mRestoreToken = results.value( "restore_token" ).toString();

    const Streams streams = qdbus_cast<Streams>( results.value( "streams" ) );
    const Stream stream = streams.last();

    QDBusInterface* portal = screencastPortal();
    if ( !portal ) {
        qDebug().noquote() << global::nameOutput << "Aborted in function: Portal_wl::slot_handleStartResponse";
        return;
    }

    // Open PipeWire Remote
    QMap<QString, QVariant> options;
    const QDBusReply<QDBusUnixFileDescriptor> reply = portal->call( "OpenPipeWireRemote", QDBusObjectPath(mSession), options );
    if ( !reply.isValid() ) {
        qDebug().noquote() << global::nameOutput << "Couldn't get reply in Portal_wl::slot_handleStartResponse";
        qDebug().noquote() << global::nameOutput << "Error: " << reply.error().message();
        emit signal_portal_aborted();
        return;
    }

    const QString fd = QString::number( reply.value().fileDescriptor() );
    const QString path = QString::number( stream.node_id );

    emit signal_portal_fd_path( fd, path );
}

QDBusInterface* QvkMagnifierPortal_wl::screencastPortal()
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

QString QvkMagnifierPortal_wl::createSessionToken() const
{
    static int sessionTokenCounter = 0;

    sessionTokenCounter += 1;
    return QString( "vokoMagniSes%1" ).arg( sessionTokenCounter );
}

QString QvkMagnifierPortal_wl::createRequestToken() const
{
    static int requestTokenCounter = 0;

    requestTokenCounter += 1;
    return QString( "vokoMagniReq%1" ).arg( requestTokenCounter );
}


/*
void QvkMagnifierPortal_wl::slot_handleCreateSessionResponse( uint response, const QVariantMap& results )
{
    // ... [Dein bestehender Code bis zur Instanziierung der Optionen] ...

    // Select sources
    QString requestToken = createRequestToken();
    QMap<QString, QVariant> options;
    options["multiple"] = false;
    options["types"] = mSourcType;
    options["cursor_mode"] = mCursorModes;
    options["handle_token"] = requestToken;

    // WICHTIG: Ändere 'TRANSIENT' auf den Wert 2 (Persistent)
    // Wenn 'PERSISTENT' eine Enum-Konstante bei dir ist, stelle sicher, dass sie den Wert 2 hat.
    options["persist_mode"] = 2;

    // Wenn bereits ein Token existiert (aus QSettings geladen), übergeben wir es hier
    if ( !mRestoreToken.isEmpty() ) {
        options["restore_token"] = mRestoreToken;
    }

    // ... [Rest der Funktion bleibt absolut identisch] ...
}

void QvkMagnifierPortal_wl::slot_handleStartResponse( uint response, const QVariantMap& results )
{
    Q_UNUSED(results);

    if ( response != 0 ) {
        qDebug().noquote() << global::nameOutput << "[Magnifier]Failed to start or cancel dialog: " << response;
        emit signal_portal_aborted();
        return;
    }

    // Das Portal liefert uns hier das NEUE oder ERNEUERTE Token zurück
    if ( results.contains( "restore_token" ) ) {
        mRestoreToken = results.value( "restore_token" ).toString();

        // WICHTIG: Das Token muss hier sofort dauerhaft gespeichert werden!
        #include <QSettings>
        QSettings settings;
        settings.setValue( "Wayland/ScreenCastRestoreToken", mRestoreToken );
        qDebug().noquote() << global::nameOutput << "Neues Restore-Token gespeichert:" << mRestoreToken;
    }

    // ... [Rest der Funktion für PipeWire-Remote bleibt identisch] ...
}

#include <QSettings>

QvkMagnifierPortal_wl::QvkMagnifierPortal_wl( QObject* parent )
    : QObject{ parent }
{
    // Lade das gespeicherte Token beim Instanziieren der Klasse
    QSettings settings;
    mRestoreToken = settings.value( "Wayland/ScreenCastRestoreToken", "" ).toString();
}

 */