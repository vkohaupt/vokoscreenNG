/*
    Copyright 2017 Harald Sitter <sitter@kde.org>
    Copyright 2017 Sune Vuorela <sune@kde.org>

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

#include <QObject>
#include <QtTest>

#include <QSettings> // parsing the ini files as desktop files
#include "testhelpers.h"

// lift a bit of code from KIconLoader to get the unit test running without tier 3 libraries
class KIconLoaderDummy : public QObject
{
Q_OBJECT
public:
    enum Context {
        Any,
        Action,
        Application,
        Device,
        FileSystem,
        MimeType,
        Animation,
        Category,
        Emblem,
        Emote,
        International,
        Place,
        StatusIcon
    };
    Q_ENUM(Context)
    enum Type {
        Fixed,
        Scalable,
        Threshold
    };
    Q_ENUM(Type)
};


/**
 * Represents icon directory to conduct simple icon lookup within.
 */
class Dir
{
public:
    Dir(const QSettings &cg, const QString &themeDir_)
        :
          themeDir(themeDir_)
        , path(cg.group())
        , size(cg.value("Size", 0).toInt())
        , contextString(cg.value("Context", QString()).toString())
        , context(parseContext(contextString))
        , type(parseType(cg.value("Type", QString("Threshold")).toString()))
    {
        QVERIFY2(!contextString.isEmpty(),
                 QString("Missing 'Context' key in file %1, config group '[%2]'").arg(cg.fileName(), cg.group()).toLatin1());
        QVERIFY2(context != -1,
                 QString("Don't know how to handle 'Context=%1' in file %2, config group '[%3]'").arg(contextString, cg.fileName(), cg.group()).toLatin1());
    }

    static QMetaEnum findEnum(const char *name)
    {
        auto mo = KIconLoaderDummy::staticMetaObject;
        for (int i = 0; i < mo.enumeratorCount(); ++i) {
            auto enumerator = mo.enumerator(i);
            if (strcmp(enumerator.name(), name) == 0) {
                return KIconLoaderDummy::staticMetaObject.enumerator(i);
            }
        }
        Q_ASSERT(false); // failed to resolve enum
        return QMetaEnum();
    }

    static QMetaEnum typeEnum()
    {
        static auto e = findEnum("Type");
        return e;
    }

    static KIconLoaderDummy::Context parseContext(const QString &string)
    {
        // Can't use QMetaEnum as the enum names are singular, the entry values are plural though.
        static QHash<QString, int> hash {
            { QStringLiteral("Actions"), KIconLoaderDummy::Action },
            { QStringLiteral("Animations"), KIconLoaderDummy::Animation },
            { QStringLiteral("Applications"), KIconLoaderDummy::Application },
            { QStringLiteral("Categories"), KIconLoaderDummy::Category },
            { QStringLiteral("Devices"), KIconLoaderDummy::Device },
            { QStringLiteral("Emblems"), KIconLoaderDummy::Emblem },
            { QStringLiteral("Emotes"), KIconLoaderDummy::Emote },
            { QStringLiteral("MimeTypes"), KIconLoaderDummy::MimeType },
            { QStringLiteral("Places"), KIconLoaderDummy::Place },
            { QStringLiteral("Status"), KIconLoaderDummy::StatusIcon },
        };
        const auto value = hash.value(string, -1);
        return static_cast<KIconLoaderDummy::Context>(value); // the caller will check that it wasn't -1
    }

    static KIconLoaderDummy::Type parseType(const QString &string)
    {
        bool ok;
        auto v = (KIconLoaderDummy::Type)typeEnum().keyToValue(string.toLatin1(), &ok);
        Q_ASSERT(ok);
        return v;
    }

    /**
     * @returns list of all icon's fileinfo (first level only, selected types
     *          only)
     */
    QList<QFileInfo> allIcons()
    {
        QList<QFileInfo> icons;
        auto iconDir = QString("%1/%2").arg(themeDir).arg(path);
        QDirIterator it(iconDir);
        while (it.hasNext()) {
            it.next();
            auto suffix = it.fileInfo().suffix();
            if (suffix != "svg" && suffix != "svgz" && suffix != "png") {
                continue; // Probably not an icon.
            }
            icons << it.fileInfo();
        }
        return icons;
    }

    QString themeDir;
    QString path;
    uint size;
    QString contextString;
    KIconLoaderDummy::Context context;
    KIconLoaderDummy::Type type;
};

// Declare so we can put them into the QTest data table.
Q_DECLARE_METATYPE(KIconLoaderDummy::Context)
Q_DECLARE_METATYPE(QSharedPointer<Dir>)

class ScalableTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_scalable_data()
    {
        for (auto dir : ICON_DIRS) {
            auto themeDir = PROJECT_SOURCE_DIR + QStringLiteral("/") + dir;

            QHash<KIconLoaderDummy::Context, QList<QSharedPointer<Dir>>> contextHash;
            QHash<KIconLoaderDummy::Context, QString> contextStringHash;

            QSettings config(themeDir + "/index.theme", QSettings::IniFormat);
            auto keys = config.allKeys();
            config.beginGroup("Icon Theme");
            auto directoryPaths = config.value("Directories", QString()).toStringList();
            config.endGroup();
            QVERIFY(!directoryPaths.isEmpty());
            for (auto directoryPath : directoryPaths) {
                config.beginGroup(directoryPath);
                QVERIFY2(keys.contains(directoryPath+"/Size"),QString("The theme %1 has an entry 'Directories' which specifies '%2' as directory, but there's no"
                                                                                         " have no associated entry '%2/Size'").arg(themeDir + "/index.theme", directoryPath).toLatin1());
                auto dir = QSharedPointer<Dir>::create(config, themeDir);
                config.endGroup();
                contextHash[dir->context].append(dir);
                contextStringHash[dir->context] = (dir->contextString);
            }

            QTest::addColumn<KIconLoaderDummy::Context>("context");
            QTest::addColumn<QList<QSharedPointer<Dir>>>("dirs");

            for (auto key : contextHash.keys()) {
                if (key != KIconLoaderDummy::Application) {
                    qDebug() << "Only testing Application context for now.";
                    continue;
                }
                // FIXME: go through qenum to stringify the bugger
                // Gets rid of the stupid second hash
                auto contextId = QString(QLatin1String(dir) + ":" + contextStringHash[key]).toLatin1();
                QTest::newRow(contextId) << key << contextHash[key];
            }
        }
    }

    void test_scalable()
    {
        QFETCH(KIconLoaderDummy::Context, context);
        QFETCH(QList<QSharedPointer<Dir>>, dirs);

        QList<QSharedPointer<Dir>> fixedDirs;
        QList<QSharedPointer<Dir>> scalableDirs;
        for (auto dir : dirs) {
            switch (dir->type) {
            case KIconLoaderDummy::Scalable:
                scalableDirs << dir;
                break;
            case KIconLoaderDummy::Fixed:
                fixedDirs << dir;
                break;
            case KIconLoaderDummy::Threshold:
                QVERIFY2(false, "Test does not support threshold icons right now.");
            }
        }

        // FIXME: context should be translated through qenum
        switch (context) {
        case KIconLoaderDummy::Application:
            // Treat this as a problem.
            QVERIFY2(!scalableDirs.empty(), "This icon context has no scalable directory at all!");
            break;
        default:
            qWarning() << "All context but Application are whitelisted from having a scalable directory.";
            return;
        }

        QStringList fixedIcons;
        for (auto dir : fixedDirs) {
            for (auto iconInfo : dir->allIcons()) {
                fixedIcons << iconInfo.completeBaseName();
            }
        }

        QHash<QString, QList<QFileInfo>> scalableIcons;
        for (auto dir : scalableDirs) {
            for (auto iconInfo : dir->allIcons()) {
                scalableIcons[iconInfo.completeBaseName()].append(iconInfo);
            }
        }

        QStringList notScalableIcons;
        for (auto fixed : fixedIcons) {
            if (scalableIcons.keys().contains(fixed)) {
                continue;
            }
            notScalableIcons << fixed;
        }

        // Assert that each icon has a scalable variant.
        if (notScalableIcons.empty()) {
            return;
        }
        notScalableIcons.removeDuplicates();
        QFAIL(QString("The following icons are not available in a scalable directory:\n  %1")
              .arg(notScalableIcons.join("\n  "))
              .toLatin1());
    }

    void test_scalableDuplicates_data()
    {
        test_scalable_data();
    }

    void test_scalableDuplicates()
    {
        QFETCH(QList<QSharedPointer<Dir>>, dirs);

        QList<QSharedPointer<Dir>> scalableDirs;
        for (auto dir : dirs) {
            switch (dir->type) {
            case KIconLoaderDummy::Scalable:
                scalableDirs << dir;
                break;
            case KIconLoaderDummy::Fixed:
                // Not of interest in this test.
                break;
            case KIconLoaderDummy::Threshold:
                QVERIFY2(false, "Test does not support threshold icons right now.");
            }
        }

        QHash<QString, QList<QFileInfo>> scalableIcons;
        for (auto dir : scalableDirs) {
            for (auto iconInfo : dir->allIcons()) {
                scalableIcons[iconInfo.completeBaseName()].append(iconInfo);
            }
        }

        QHash<QString, QList<QFileInfo>> duplicatedScalableIcons;
        for (auto icon: scalableIcons.keys()) {
            auto list = scalableIcons[icon];
            if (list.size() > 1) {
                duplicatedScalableIcons[icon] = list;
            }
        }

        // Assert that there is only one scalable version per icon name.
        // Otherwise apps/32/klipper.svg OR apps/48/klipper.svg may be used.
        if (!duplicatedScalableIcons.empty()) {
            QString msg;
            QTextStream stream(&msg);
            stream << "Duplicated scalable icons:" << endl;
            for (auto icon : duplicatedScalableIcons.keys()) {
                stream << QString("  %1:").arg(icon) << endl;
                for (auto info : duplicatedScalableIcons[icon]) {
                    stream << QString("    %1").arg(info.absoluteFilePath()) << endl;
                }
            }
            QFAIL(msg.toLatin1());
        }
    }
};

QTEST_GUILESS_MAIN(ScalableTest)

#include "scalabletest.moc"
