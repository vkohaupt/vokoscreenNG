/*
 * Copyright © 2016 Red Hat, Inc
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *       Jan Grulich <jgrulich@redhat.com>
 */

#ifndef PORTAL_WL_H
#define PORTAL_WL_H

#include <QFlags>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QVariantMap>

class Portal_wl : public QObject
{
    Q_OBJECT
public:
    typedef struct {
        uint node_id;
        QVariantMap map;
    } Stream;
    typedef QList<Stream> Streams;

    Portal_wl();
    ~Portal_wl();
    void requestScreenSharing( int value, int mouseOnOff );


public Q_SLOTS:
    void slot_gotCreateSessionResponse(uint response, const QVariantMap &results);
    void slot_gotSelectSourcesResponse(uint response, const QVariantMap &results);
    void slot_gotStartResponse(uint response, const QVariantMap &results);


private slots:


private:
    QString getSessionToken();
    QString getRequestToken();

    QString m_session;
    uint m_sessionTokenCounter;
    uint m_requestTokenCounter;

    int Selection_Screen_Window_Area = 1;
    int record_mouse_onOff;

protected:


signals:
    void signal_portal_fd_path( QString, QString );
    void signal_portal_cancel( uint );


};

#endif // PORTAL_WL_H
