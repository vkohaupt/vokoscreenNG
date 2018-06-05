/*  This file is part of the KDE libraries
 *    Copyright (C) 2016 Kåre Särs <kare.sars@iki.fi>
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Library General Public
 *    License as published by the Free Software Foundation; either
 *    version 2 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Library General Public License for more details.
 *
 *    You should have received a copy of the GNU Library General Public License
 *    along with this library; see the file COPYING.LIB.  If not, write to
 *    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *    Boston, MA 02110-1301, USA.
 */
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QRegularExpression>
#include <QDebug>
#include <QCommandLineParser>

QString link(const QString &path, const QString &fileName)
{
    QFile in(path + QLatin1Char('/') + fileName);
    if (!in.open(QIODevice::ReadOnly)) {
        qDebug() << "failed to read" << path << fileName << in.fileName();
        return QString();
    }

    QString firstLine = in.readLine();
    if (firstLine.isEmpty()) {
        qDebug() << in.fileName() << "line could not be read...";
        return QString();
    }
    QRegularExpression fNameReg(QStringLiteral("(.*\\.(?:svg|png|gif|ico))"));
    QRegularExpressionMatch match = fNameReg.match(firstLine);
    if (!match.hasMatch()) {
        return QString();
    }

    QFileInfo linkInfo(path + QLatin1Char('/') + match.captured(1));
    QString aliasLink = link(linkInfo.path(), linkInfo.fileName());
    if (!aliasLink.isEmpty()) {
        //qDebug() <<  fileName << "=" << match.captured(1) << "=" << aliasLink;
        return aliasLink;
    }

    return  path + QLatin1Char('/') + match.captured(1);
}

int parseFile(const QString &infile, const QString &outfile)
{
    QFile in(infile);
    QFile out(outfile);
    QRegularExpression imageReg(QStringLiteral("<file>(.*\\.(?:svg|png|gif|ico))</file>"));

    if (!in.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open" << infile;
        return -1;
    }
    if (!out.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to create" << outfile;
        return -2;
    }

    while (in.bytesAvailable()) {
        QString line = QString::fromLocal8Bit(in.readLine());
        QRegularExpressionMatch match = imageReg.match(line);
        if (!match.hasMatch()) {
            //qDebug() << "No Match: " << line;
            out.write(qPrintable(line));
            continue;
        }

        QFileInfo info(match.captured(1));

        QString aliasLink = link(info.path(), info.fileName());
        if (aliasLink.isEmpty()) {
            //qDebug() << "No alias: " << line;
            out.write(qPrintable(line));
            continue;
        }

        QString newLine = QStringLiteral("<file alias=\"%1\">%2</file>\n").arg(match.captured(1), aliasLink);
        //qDebug() << newLine;
        out.write(qPrintable(newLine));
    }
    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;

    QCommandLineOption inOption(QStringList() << QLatin1String("i") << QLatin1String("infile"), QStringLiteral("Input qrc file"), QStringLiteral("infile"));
    QCommandLineOption outOption(QStringList() << QLatin1String("o") << QLatin1String("outfile"), QStringLiteral("Output qrc file"), QStringLiteral("outfile"));
    parser.setApplicationDescription(
        QLatin1String("On Windows git handles symbolic links by converting them "
        "to text files containing the links to the actual file. This application "
        "takes a .qrc file as input and outputs a .qrc file with the symbolic "
        "links converted to qrc-aliases."));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(inOption);
    parser.addOption(outOption);
    parser.process(app);

    const QString inName = parser.value(inOption);
    const QString outName = parser.value(outOption);

    return parseFile(inName, outName);
}
