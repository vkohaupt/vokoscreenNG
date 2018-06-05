/*
    Copyright 2015 Harald Sitter <sitter@kde.org>

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

class SymlinkTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    // Invalid symlinks shouldn't happen.
    void test_broken()
    {
        // By default broken symlinks are not listed unless the System filter
        // is used. System may however also include pipes and the like, so we
        // still manually need to filter for symlinks afterwards.
        QDirIterator it(PROJECT_SOURCE_DIR,
                        QDir::System,
                        QDirIterator::Subdirectories);
        QList<QFileInfo> brokenSymLinks;
        while (it.hasNext()) {
            it.next();
            auto info = it.fileInfo();
            if (!info.isSymLink() || info.exists()) {
                continue;
            }
            brokenSymLinks << info;
        }
        failSymlinkList(brokenSymLinks,
                        QStringLiteral("Found broken symlinks:\n"));
    }

    // Symlinks should never point to something outside the tree, even if valid!
    void test_outOfTree()
    {
        QDirIterator it(PROJECT_SOURCE_DIR,
                        QDir::AllEntries,
                        QDirIterator::Subdirectories);
        QList<QFileInfo> OOTSymLinks;
        while (it.hasNext()) {
            it.next();
            auto info = it.fileInfo();
            if (!info.isSymLink() ||
                    info.symLinkTarget().startsWith(PROJECT_SOURCE_DIR)) {
                continue;
            }
            OOTSymLinks << info;
        }
        failSymlinkList(OOTSymLinks,
                        QStringLiteral("Found out-of-tree symlinks:\n"));
    }
};

QTEST_GUILESS_MAIN(SymlinkTest)

#include "symlinktest.moc"
