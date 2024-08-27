/*
 * Copyright (C) 2015-2024 Département de l'Instruction Publique (DIP-SEM)
 *
 * Copyright (C) 2013 Open Education Foundation
 *
 * Copyright (C) 2010-2013 Groupement d'Intérêt Public pour
 * l'Education Numérique en Afrique (GIP ENA)
 *
 * This file is part of OpenBoard.
 *
 * OpenBoard is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License,
 * with a specific linking exception for the OpenSSL project's
 * "OpenSSL" library (or with modified versions of it that use the
 * same license as the "OpenSSL" library).
 *
 * OpenBoard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenBoard. If not, see <http://www.gnu.org/licenses/>.
 */


// #include "UBDesktopPortal.h"
#include "portal_wl.h"


#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusUnixFileDescriptor>
#include <QDebug>
#include <QUrl>
#include <QFile>
#include <QPixmap>

//#include "board/UBBoardView.h"
//#include "core/UBApplication.h"
//#include "core/UBApplicationController.h"
//#include "desktop/UBDesktopAnnotationController.h"


enum : uint { MONITOR = 1, WINDOW = 2, VIRTUAL = 4 } SourceType;
enum : uint { HIDDEN = 1, EMBEDDED = 2, METADATA = 4 } CursorMode;
enum : uint { TRANSIENT = 0, APPLICATION = 1, PERSISTENT = 2 } PersistMode;

Q_DECLARE_METATYPE(Portal_wl::Stream)
Q_DECLARE_METATYPE(Portal_wl::Streams)

const QDBusArgument &operator >> (const QDBusArgument &arg, Portal_wl::Stream &stream)
{
    arg.beginStructure();
    arg >> stream.node_id;

    arg.beginMap();

    while (!arg.atEnd())
    {
        QString key;
        QVariant value;
        arg.beginMapEntry();
        arg >> key >> value;
        arg.endMapEntry();
        stream.map.insert(key, value);
    }

    arg.endMap();
    arg.endStructure();

    return arg;
}

Portal_wl::Portal_wl(QObject* parent)
    : QObject{parent}
{
}

Portal_wl::~Portal_wl()
{
    if (mScreencastPortal)
    {
        delete mScreencastPortal;
    }
}
/*
void Portal_wl::grabScreen(QScreen* screen, const QRect& rect)
{
    mScreenRect = screen->geometry();

    if (!rect.isNull())
    {
        mScreenRect = rect.translated(mScreenRect.topLeft());
    }

    QDBusInterface screenshotPortal("org.freedesktop.portal.Desktop", "/org/freedesktop/portal/desktop", "org.freedesktop.portal.Screenshot");

    if (screenshotPortal.isValid())
    {
        QMap<QString, QVariant> options;
        options["handle_token"] = createRequestToken();
        QDBusReply<QDBusObjectPath> reply = screenshotPortal.call("Screenshot", "", options);
        QDBusObjectPath objectPath = reply.value();
        QString path = objectPath.path();
        QDBusConnection::sessionBus().connect(
                            "",
                            path,
                            "org.freedesktop.portal.Request",
                            "Response",
                            "ua{sv}",
                            this,
                            SLOT(handleScreenshotResponse(uint,QMap<QString,QVariant>)));
    }
    else
    {
        qDebug() << "No valid screenshot portal";
        emit screenGrabbed(QPixmap{});
    }
}
*/
void Portal_wl::startScreenCast(bool withCursor)
{
    mWithCursor = withCursor;

    QDBusInterface* portal = screencastPortal();

    if (!portal)
    {
        return;
    }

    // Create ScreenCast session
    QString requestToken = createRequestToken();
    QMap<QString, QVariant> options;
    options["session_handle_token"] = createSessionToken();
    options["handle_token"] = requestToken;

    QDBusConnection::sessionBus().connect("", mRequestPath + requestToken, "org.freedesktop.portal.Request", "Response", "ua{sv}", this,
                                          SLOT(handleCreateSessionResponse(uint,QMap<QString,QVariant>)));

    const QDBusReply<QDBusObjectPath> reply = portal->call("CreateSession", options);

    if (!reply.isValid())
    {
        qWarning() << "Couldn't get reply to ScreenCast/CreateSession";
        qWarning() << "Error: " << reply.error().message();
        emit screenCastAborted();
        return;
    }
}

void Portal_wl::stopScreenCast()
{
    if (mSession.isEmpty())
    {
        return;
    }

    QDBusInterface portal("org.freedesktop.portal.Desktop", mSession, "org.freedesktop.portal.Session");

    if (portal.isValid())
    {
        const QDBusReply<void> reply = portal.call("Close");

        if (!reply.isValid())
        {
            qWarning() << "Couldn't get reply to ScreenCast/Close";
            qWarning() << "Error: " << reply.error().message();
            mSession.clear();
            return;
        }
    }
}
/*
void Portal_wl::handleScreenshotResponse(uint code, const QMap<QString, QVariant>& results)
{
    QUrl uri(results["uri"].toUrl());
    QFile file(uri.toLocalFile());

    if (!file.exists())
    {
        qDebug() << "Screenshot image file does not exist";
        emit screenGrabbed(QPixmap{});
        return;
    }

    QPixmap pixmap{file.fileName()};
    file.remove();

    // cut requested screen
    QPixmap screenshot = pixmap.copy(mScreenRect);

    emit screenGrabbed(screenshot);
}
*/
void Portal_wl::handleCreateSessionResponse(uint response, const QVariantMap& results)
{
    if (response != 0)
    {
        qWarning() << "Failed to create session: " << response << results;
        emit screenCastAborted();
        return;
    }

    mSession = results.value("session_handle").toString();

    QDBusInterface* portal = screencastPortal();

    if (!portal)
    {
        return;
    }

    // Select sources
    QString requestToken = createRequestToken();
    QMap<QString, QVariant> options;
    options["multiple"] = false;
    options["types"] = MONITOR;
    options["cursor_mode"] = mWithCursor ? EMBEDDED : HIDDEN;
    options["handle_token"] = requestToken;
    options["persist_mode"] = PERSISTENT;

    if (!mRestoreToken.isEmpty())
    {
        options["restore_token"] = mRestoreToken;
    }

    // connect before call
    QDBusConnection::sessionBus().connect("", mRequestPath + requestToken, "org.freedesktop.portal.Request", "Response", "ua{sv}", this,
                                          SLOT(handleSelectSourcesResponse(uint,QMap<QString,QVariant>)));

    const QDBusReply<QDBusObjectPath> reply = portal->call("SelectSources", QDBusObjectPath(mSession), options);

    if (!reply.isValid())
    {
        qWarning() << "Couldn't get reply";
        qWarning() << "Error: " << reply.error().message();
        emit screenCastAborted();
        return;
    }
}

void Portal_wl::handleSelectSourcesResponse(uint response, const QVariantMap& results)
{
    Q_UNUSED(results);

    if (response != 0)
    {
        qWarning() << "Failed to select sources: " << response;
        emit screenCastAborted();
        return;
    }

    QDBusInterface* portal = screencastPortal();

    if (!portal)
    {
        return;
    }

    // Start ScreenCast
    QString requestToken = createRequestToken();
    QMap<QString, QVariant> options;
    options["handle_token"] = requestToken;

    QDBusConnection::sessionBus().connect("", mRequestPath + requestToken, "org.freedesktop.portal.Request", "Response", "ua{sv}", this,
                                          SLOT(handleStartResponse(uint,QMap<QString,QVariant>)));

    const QDBusReply<QDBusObjectPath> reply = portal->call("Start", QDBusObjectPath(mSession), "", options);

    if (!reply.isValid())
    {
        qWarning() << "Couldn't get reply";
        qWarning() << "Error: " << reply.error().message();
        emit screenCastAborted();
        return;
    }

    // Hide annotation drawing view in desktop mode so that portal dialog is topmost
//    showGlassPane(false);
}

void Portal_wl::handleStartResponse(uint response, const QVariantMap& results)
{
    Q_UNUSED(results);

    // Show annotation drawing view in desktop mode after portal dialog was closed
//    showGlassPane(true);

    if (response != 0)
    {
        // The system Desktop dialog was canceled
        qDebug() << "Failed to start or cancel dialog: " << response;
        emit screenCastAborted();
        return;
    }

    // save restore token
    mRestoreToken = results.value("restore_token").toString();

    const Streams streams = qdbus_cast<Streams>(results.value("streams"));
    const Stream stream = streams.last();

    QDBusInterface* portal = screencastPortal();

    if (!portal)
    {
        return;
    }

    // Open PipeWire Remote
    QMap<QString, QVariant> options;
    const QDBusReply<QDBusUnixFileDescriptor> reply = portal->call("OpenPipeWireRemote", QDBusObjectPath(mSession), options);

    if (!reply.isValid())
    {
        qWarning() << "Couldn't get reply";
        qWarning() << "Error: " << reply.error().message();
        emit screenCastAborted();
        return;
    }

    const quint64 fd = reply.value().fileDescriptor();
    const QString path = QString::number(stream.node_id);

    emit streamStarted(fd, path);
}

QDBusInterface* Portal_wl::screencastPortal()
{
    if (!mScreencastPortal)
    {
        mScreencastPortal = new QDBusInterface("org.freedesktop.portal.Desktop", "/org/freedesktop/portal/desktop",
                                               "org.freedesktop.portal.ScreenCast");
        mScreencastPortal->setParent(this);

        mRequestPath = "/org/freedesktop/portal/desktop/request/" + mScreencastPortal->connection().baseService().remove(0, 1).replace('.', '_') + "/";
        qDebug() << "request path" << mRequestPath;
    }

    if (mScreencastPortal->isValid())
    {
        return mScreencastPortal;
    }

    emit screenCastAborted();
    return nullptr;
}

QString Portal_wl::createSessionToken() const
{
    static int sessionTokenCounter = 0;

    sessionTokenCounter += 1;
    return QString("obsess%1").arg(sessionTokenCounter);
}

QString Portal_wl::createRequestToken() const
{
    static int requestTokenCounter = 0;

    requestTokenCounter += 1;
    return QString("obreq%1").arg(requestTokenCounter);
}
