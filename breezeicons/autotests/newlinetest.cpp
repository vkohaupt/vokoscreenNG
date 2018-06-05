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

#include <QDirIterator>
#include <QObject>
#include <QtTest>

#include "testhelpers.h"

class NewlineTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    // Files that contain a newline in their name
    void test_whitespace()
    {
        QList<QString> brokenFiles;
        QDirIterator it(PROJECT_SOURCE_DIR,
                        QDir::Files	| QDir::System,
                        QDirIterator::Subdirectories);
        while (it.hasNext()) {
            it.next();
            if (it.fileName().simplified() != it.fileName()) {
                brokenFiles << it.filePath();
            }
        }
        failListContent(brokenFiles,
                        QStringLiteral("Found file with bad characters (http://doc.qt.io/qt-5/qstring.html#simplified):\n"));
    }

};

QTEST_GUILESS_MAIN(NewlineTest)

#include "newlinetest.moc"
