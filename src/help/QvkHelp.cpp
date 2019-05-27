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

#include "QvkHelp.h"
#include "global.h"

#include <QMessageBox>
#include <QLabel>
#include <QDir>
#include <QTextDocument>

/*
 * The remote HTML-file and the toolbutton have the same name.
 *
 * Example:
 * QToolbutton -> help_screencast_fullscreen
 * Remote file -> help_screencast_fullscreen.html
 *
 * We write as snake_case, so the underline character is the seperator.
 * The first section is help, this we need for set for icon and installEventFilter, see constructor.
 * The second section is the tab in the sidebar and the subdir on remote.
 * The third section is the name from the function that we want help.
 * See also QvkHelp::eventFilter(QObject *object, QEvent *event)
 */

QvkHelp::QvkHelp(QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow, Ui_player *ui_player) : uiHelp(new(Ui::help))
{
    connect( mainWindow, SIGNAL( destroyed( QObject*) ), this, SLOT( slot_cleanUp() ) );

    ui = ui_mainwindow;
    uiPlayer = ui_player;
    uiHelp->setupUi( this );

    resize( 800, 600 );
    setWindowTitle( global::name + " " + global::version + " " + "help" );

    QIcon icon;
    icon.addFile( QString::fromUtf8( ":/pictures/logo/logo.png" ), QSize(), QIcon::Normal, QIcon::Off );
    setWindowIcon( icon );

    QStringList helpStringList;
    helpStringList << "http:/"
                   << "vokoscreen.volkoh.de111111111111"
                   << "3.0"
                   << "help";

    vk_helpPath = helpStringList.join( "/" ).append( "/");
    QString language = "en";
    vk_helpPath = vk_helpPath + language + "/";

    vkDownloadHTML = new QvkDownloader( temporaryDirLocal.path() );
    vkDownloadFiles = new QvkDownloader( temporaryDirLocal.path() );

    QIcon iconHelp = style()->standardIcon( QStyle::SP_MessageBoxInformation );
    QList<QToolButton *> listToolButton = ui->centralWidget->findChildren<QToolButton *>();
    for ( int i = 0; i < listToolButton.count(); i++ )
    {
        if ( listToolButton.at(i)->objectName().contains( QRegExp( "^help_") ) )
        {
            listToolButton.at(i)->setIcon( iconHelp );
            listToolButton.at(i)->installEventFilter( this );
        }
    }
}


QvkHelp::~QvkHelp()
{
}


bool QvkHelp::eventFilter(QObject *object, QEvent *event)
{
    if ( event->type() == QEvent::MouseButtonRelease )
    {
       loadHTML( vk_helpPath + object->objectName().section( "_", 1, 1 ) + "/" + object->objectName() + ".html" );
       return false;
    }
    else
    {
        return QObject::eventFilter( object, event );
    }
}


void QvkHelp::slot_cleanUp()
{
    temporaryDirLocal.remove();
}


/*
 * To first we load the html file ...
 */
void QvkHelp::loadHTML( QString value )
{
    QFileInfo fileInfo( value );
    remotePath = fileInfo.path();
    remoteBasename = fileInfo.baseName();
    disconnect( vkDownloadHTML, nullptr, nullptr, nullptr );
    connect( vkDownloadHTML, SIGNAL( signal_fileDownloaded( QString ) ), this, SLOT( slot_parseHTML( QString ) ) );
    vkDownloadHTML->doDownload( value );
}


bool QvkHelp::isFileInLine( QString line )
{
    bool value = false;
    for ( int i = 0; i < toDownloadFiles.count(); i++ )
    {
        if ( line.contains( toDownloadFiles.at(i), Qt::CaseInsensitive ) )
        {
            value = true;
            break;
        }
        else
        {
            value = false;
        }
    }
    return value;
}


int QvkHelp::getCountFileToDownload( QString tempPathFileName )
{
    QFile file( tempPathFileName );
    if( !file.open( QIODevice::ReadOnly ) )
    {
        QMessageBox::information( nullptr, "Help error", "QvkHelp::getCountFileToDownload\n" + tempPathFileName + "\n" + file.errorString() );
    }

    int count = 0;
    QTextStream textStream( &file );
    while( !textStream.atEnd() )
    {
        QString line = textStream.readLine();
        if ( isFileInLine( line ) )
        {
            count++;
        }
    }
    file.close();
    return count;
}


/*
 * ... then we parse *.html of files and download all files from remote Url and save local in tmp ...
 */
void QvkHelp::slot_parseHTML( QString tempPathFileName )
{
    localFiles.clear();
    localFiles << tempPathFileName;

    QFileInfo fileInfo( tempPathFileName );
    QString tmpPath = fileInfo.absolutePath();

    QFile file( tempPathFileName );
    if( !file.open( QIODevice::ReadOnly ) )
    {
        QMessageBox::information( nullptr, "Help error", "No network connection\nQvkHelp::slot_parseHTML\n" + tempPathFileName + "\n" + file.errorString() );
        return;
    }

    int countFiles = getCountFileToDownload( tempPathFileName );
    //qDebug().noquote() << global::nameOutput << "HTML file parsed, downloading" << countFiles << "files";
    int counter = 0;
    QTextStream textStream( &file );
    while( !textStream.atEnd() )
    {
        QString line = textStream.readLine();
        if ( isFileInLine( line ) )
        {
            QString fileForHTML = line.section( "\"", 1, 1 );
            counter++;
            if ( counter == countFiles )
            {
                disconnect( vkDownloadFiles, nullptr, nullptr, nullptr );
                connect( vkDownloadFiles, SIGNAL( signal_fileDownloaded( QString ) ), this, SLOT( slot_showHelp( QString ) ) );
            }
            vkDownloadFiles->doDownload( remotePath + "/" + fileForHTML );
            localFiles << tmpPath + + "/" + fileForHTML;
        }
    }

    if ( counter == 0 )
    {
        // "dummy.png" is a fake, we need this if no file is downlowded
        slot_showHelp( tmpPath + "/" + "dummy.png");
    }

    file.close();
}

/*
 * ... then we show the html file
 */
void QvkHelp::slot_showHelp( QString tempPathFileName )
{
    QDir dir;
    QString currentdir = dir.currentPath();

    QFileInfo fileInfo( tempPathFileName );
    QString tmpPath = fileInfo.absolutePath();
    dir.setCurrent( tmpPath );

    QString htmlFile = tmpPath + "/" + remoteBasename + ".html";
    QFile file( htmlFile );
    if( !file.open( QIODevice::ReadOnly ) )
    {
        QMessageBox::information( nullptr, "Help error", "QvkHelp::slot_showHelp\n" + tempPathFileName + "\n" + file.errorString() );
    }

    QTextStream textStream( &file );
    QString value = textStream.readAll();

    // Antialiasing for Font
    QFont font = uiHelp->textBrowser->font();
    font.setStyleStrategy( QFont::PreferAntialias );
    uiHelp->textBrowser->setFont(font);

    uiHelp->textBrowser->setText( value );
    file.close();

    show();
    //qDebug().noquote() << global::nameOutput << "Show help";

    dir.setCurrent( currentdir );

    // remove all tmp files
    for ( int i = 0; i < localFiles.count(); i++  )
    {
        QFile file( localFiles.at( i ) );
        file.remove();
    }
}
