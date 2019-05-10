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

#include "QvkLicenses.h"

#include <QSettings>
#include <QDir>
#include <QStringList>
#include <QDirIterator>
#include <QFontDatabase>
#include <QFont>

#include <QTextDocumentFragment>

QvkLicenses::QvkLicenses( Ui_formMainWindow *ui_mainwindow ) : ui( new Ui::license )
{
    ui->setupUi( this );

    QIcon icon;
    icon.addFile( QString::fromUtf8( ":/pictures/logo.png" ), QSize(), QIcon::Normal, QIcon::Off );
    setWindowIcon( icon );

    connect( ui_mainwindow->pushButtonLicense, SIGNAL( clicked( bool ) ), this, SLOT( show() ) );

    const QFont fixedFont = QFontDatabase::systemFont( QFontDatabase::FixedFont );
    ui->textBrowser->setFont( fixedFont );
    ui->textBrowser->setContextMenuPolicy( Qt::NoContextMenu );
    ui->textBrowser->insertHtml( "<body leftmargin='100px'></body>" );

    QDirIterator dirIterator( ":/pictures/", QDir::Files, QDirIterator::Subdirectories );
    while ( dirIterator.hasNext() )
    {
        QString string = dirIterator.next();
        if ( ( string.contains( ".license" ) == true ) and ( string.contains( "template" ) == false ) )
        {
            QSettings settings( dirIterator.filePath(), QSettings::IniFormat );
            settings.beginGroup( "license" );

                ui->textBrowser->insertHtml( "<img src='" + dirIterator.filePath().replace( "license", "png" ) + "' width=40 height=40" );
                ui->textBrowser->insertHtml( "<br>" );

                ui->textBrowser->insertHtml( "Author : (C) " + settings.value( "author" ).toString() + "<br>" );

                if ( settings.value( "license" ).toString() > "" )
                {
                    ui->textBrowser->insertHtml( "License: " + settings.value( "license" ).toString() + "<br>" );
                }

                if ( settings.value( "url" ).toString().contains( "http" ) )
                {
                     ui->textBrowser->insertHtml( "Source : <a href='" + settings.value( "url" ).toString() + "'>" + settings.value( "url" ).toString() + "</a>" + "<br>" );
                }
                else
                {
                    ui->textBrowser->insertHtml( "Source : " + settings.value( "url" ).toString() + "<br>" );
                }

            ui->textBrowser->insertHtml( "<br><br><br>" );
            settings.endGroup();
        }
    }
    ui->textBrowser->moveCursor( QTextCursor::Start );

}

QvkLicenses::~QvkLicenses(){}
