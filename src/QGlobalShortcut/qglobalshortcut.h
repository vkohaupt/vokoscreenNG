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

#ifndef QGLOBALSHORTCUTPRIVATE_H
#define QGLOBALSHORTCUTPRIVATE_H

#include <QObject>
#include <QAbstractNativeEventFilter>

class QKeySequence;
class QGlobalShortcutPrivate;

class QGlobalShortcut : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit QGlobalShortcut(QObject *parent = nullptr);
    ~QGlobalShortcut();

    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;

    QKeySequence shortcut();
    bool isEmpty();
    bool isEnabled();

signals:
    void activated();

public slots:
    bool setShortcut(const QKeySequence &keySequence);
    bool unsetShortcut();
    void setEnabled(bool enable);

private:
    QGlobalShortcutPrivate *sPrivate;
};

#endif // QGLOBALSHORTCUTPRIVATE_H
