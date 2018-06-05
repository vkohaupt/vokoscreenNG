/*
    Copyright 2015-2016 Harald Sitter <sitter@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TESTHELPERS_H
#define TESTHELPERS_H

#include <QFileInfo>
#include <QList>
#include <QString>

#include "testdata.h"

#define _T_LIST_INDENT QStringLiteral("  ")
#define _T_LIST_INDENT2 QStringLiteral("    ")

void failListContent(const QList<QString> &list, const QString &header)
{
    if (list.empty()) {
        return;
    }
    QString message = ("\n" + _T_LIST_INDENT + header);
    for (const auto path : list) {
        message += (_T_LIST_INDENT2 + "- " + path + "\n");
    }
    QFAIL(message.toLatin1());
}

void failSymlinkList(const QList<QFileInfo> &list, const QString &header)
{
    if (list.empty()) {
        return;
    }
    QString message = ("\n" + _T_LIST_INDENT + header);
    for (const auto info : list) {
        message += (_T_LIST_INDENT2 + info.filePath() + " => " + info.symLinkTarget() + "\n");
    }
    QFAIL(message.toLatin1());
}

#endif // TESTHELPERS_H
