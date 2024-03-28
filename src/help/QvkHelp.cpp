/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2022 Volker Kohaupt
 * 
 * Author:
 *      Volker Kohaupt <vkohaupt@volkoh.de>
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
#include <QSettings>

/*
 * The remote HTML-file and the toolbutton have the same name.
 *
 * Example:
 * QToolbutton -> help_screencast_fullscreen
 * Remote file -> help_screencast_fullscreen.html
 *
 * We write as snake_case, so the underline character is the separator.
 * The first section is help, this we need for set for icon and installEventFilter, see constructor.
 * The second section is the tab in the sidebar and the subdir on remote.
 * The third section is the name from the function that we want help.
 * See also QvkHelp::eventFilter(QObject *object, QEvent *event)
 */

QvkHelp::QvkHelp( Ui_formMainWindow *ui_mainwindow ) : uiHelp(new(Ui::help))
{
    ui = ui_mainwindow;
    uiHelp->setupUi( this );

    // Is needed only for the translated text
    QDialogButtonBox *buttonBox = new QDialogButtonBox( QDialogButtonBox::Close, this );
    buttonBox->hide();
    QList<QPushButton *> list = buttonBox->findChildren<QPushButton *>();
    uiHelp->pushButtonClose->setText( list.at(0)->text() );

    connect( uiHelp->pushButtonClose, SIGNAL( clicked( bool ) ), this, SLOT( close() ) );

    resize( 800, 600 );
    setWindowTitle( QString( tr( "Help") ) );

    QIcon icon;
    icon.addFile( QString::fromUtf8( ":/pictures/logo/logo.png" ), QSize(), QIcon::Normal, QIcon::Off );
    setWindowIcon( icon );

    QStringList helpStringList;
    helpStringList << "https:/"
                   << "vokoscreen.volkoh.de"
                   << "3.0"
                   << "help";

    vk_helpPath = helpStringList.join( "/" ).append( "/");

    vkLocale = new QvkLocale();
    connect( vkLocale, SIGNAL( signal_locale( QStringList) ), this, SLOT( slot_parse_locale( QStringList ) ) );

    vkDownloadHTML = new QvkDownloader( temporaryDirLocal.path() );
    vkDownloadFiles = new QvkDownloader( temporaryDirLocal.path() );

    QIcon iconHelp( ":/pictures/help/information.png" );
    QList<QToolButton *> listToolButton = ui->centralWidget->findChildren<QToolButton *>();
    for ( int i = 0; i < listToolButton.count(); i++ ) {
        if ( listToolButton.at(i)->objectName().startsWith( "help_") ) {
            listToolButton.at(i)->setIcon( iconHelp );
            listToolButton.at(i)->installEventFilter( this );
        }
    }
}


QvkHelp::~QvkHelp()
{
}


void QvkHelp::slot_parse_locale( QStringList list )
{
    for ( int i = 0; i < list.count(); i++ ) {
       QLocale locale( list.at( i ) );
       ui->comboBoxOnlineHelp->addItem( locale.nativeLanguageName() + " " + "(" + list.at(i) + ")", list.at( i ) );
    }

    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    QString valueText = settings.value( ui->comboBoxOnlineHelp->objectName(), "" ).toString();
    int valueInt = ui->comboBoxOnlineHelp->findText( valueText );
    if ( valueInt > -1 ) {
        ui->comboBoxOnlineHelp->setCurrentIndex( valueInt );
    }
}


bool QvkHelp::eventFilter(QObject *object, QEvent *event)
{
    QToolButton *toolButton = qobject_cast<QToolButton *>(object);

    // Automatic language detection is set in combobox for the online help.
    if ( ( event->type() == QEvent::MouseButtonRelease ) and
         ( toolButton->isEnabled() == true ) and
         ( ui->comboBoxOnlineHelp->currentIndex() == 0 ) )
    {

        QString language;
        if ( ui->comboBoxOnlineHelp->findText( "(" + QLocale::system().name() + ")", Qt::MatchEndsWith ) > -1 ) {
            language = QLocale::system().name();
        } else {
            language = "en";
        }

        QString vk_helpPath_locale = vk_helpPath + language + "/";

        loadHTML( vk_helpPath_locale + object->objectName().section( "_", 1, 1 ) + "/" + object->objectName() + ".html" );
        uiHelp->labelURL->setText( vk_helpPath_locale + object->objectName().section( "_", 1, 1 ) + "/" + object->objectName() + ".html" );

        return false;
    }

    // Selected Language is set in combobox
    if ( ( event->type() == QEvent::MouseButtonRelease ) and
         ( toolButton->isEnabled() == true ) and
         ( ui->comboBoxOnlineHelp->currentIndex() > 0 ) )
    {
        QString language = ui->comboBoxOnlineHelp->currentText().section( "(", 1 ).replace( ")", "" );
        QString vk_helpPath_locale = vk_helpPath + language + "/";
        loadHTML( vk_helpPath_locale + object->objectName().section( "_", 1, 1 ) + "/" + object->objectName() + ".html" );
        uiHelp->labelURL->setText( vk_helpPath_locale + object->objectName().section( "_", 1, 1 ) + "/" + object->objectName() + ".html" );
        return false;
    } else {
        return QObject::eventFilter( object, event );
    }
}


void QvkHelp::slot_cleanUp()
{
    bool bo = temporaryDirLocal.remove();
    if ( bo == true ) {
        qDebug().noquote() << global::nameOutput << "QvkHelp::slot_cleanUp TempDir is removed" << temporaryDirLocal.path();
    } else {
        qDebug().noquote() << global::nameOutput << "QvkHelp::slot_cleanUp TempDir not removed" << temporaryDirLocal.path();
    }
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
    for ( int i = 0; i < toDownloadFiles.count(); i++ ) {
        if ( line.contains( toDownloadFiles.at(i), Qt::CaseInsensitive ) ) {
            value = true;
            break;
        } else {
            value = false;
        }
    }
    return value;
}


int QvkHelp::getCountFileToDownload( QString tempPathFileName )
{
    QFile file( tempPathFileName );
    if( !file.open( QIODevice::ReadOnly ) ) {
        qDebug().noquote() << global::nameOutput << "QvkHelp::getCountFileToDownload" << tempPathFileName  << file.errorString();
        //QMessageBox::information( nullptr, "Help error", "QvkHelp::getCountFileToDownload\n" + tempPathFileName + "\n" + file.errorString() );
    }

    int count = 0;
    QTextStream textStream( &file );
    while( !textStream.atEnd() ) {
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
    if( !file.open( QIODevice::ReadOnly ) ) {
        qDebug().noquote() << global::nameOutput << "QvkHelp::slot_parseHTML" << tempPathFileName  << file.errorString() << "The online help needs an internet connection";
        QMessageBox msgBox;
        msgBox.setWindowTitle( global::name + " " + global::version );
        msgBox.setWindowIcon( QIcon( ":/pictures/logo/logo.png" ) );
        msgBox.setIconPixmap( QPixmap( ":/pictures/status/information.png" ).scaledToHeight( 50, Qt::SmoothTransformation ) );
        msgBox.setText("\nThe online help needs an internet connection");
        msgBox.exec();

        return;
    }

    int countFiles = getCountFileToDownload( tempPathFileName );
    int counter = 0;
    QTextStream textStream( &file );
    while( !textStream.atEnd() ) {
        QString line = textStream.readLine();
        if ( isFileInLine( line ) ) {
            QString fileForHTML = line.section( "\"", 1, 1 );
            counter++;
            if ( counter == countFiles ) {
                disconnect( vkDownloadFiles, nullptr, nullptr, nullptr );
                connect( vkDownloadFiles, SIGNAL( signal_fileDownloaded( QString ) ), this, SLOT( slot_showHelp( QString ) ) );
            }
            vkDownloadFiles->doDownload( remotePath + "/" + fileForHTML );
            localFiles << tmpPath + + "/" + fileForHTML;
        }
    }

    if ( counter == 0 ) {
        // "dummy.png" is a fake, we need this if no file is downloaded
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
    if( !file.open( QIODevice::ReadOnly ) ) {
        qDebug().noquote() << global::nameOutput << "QvkHelp::slot_showHelp" << tempPathFileName  << file.errorString();
        //QMessageBox::information( nullptr, "Help error", "QvkHelp::slot_showHelp\n" + tempPathFileName + "\n" + file.errorString() );
    }

    QString value = QString::fromUtf8( file.readAll() );

    // Entfernen von REM
    // Es wird nur ein einziges REM unterstützt
    int begin = value.indexOf( "<!--" );
    int end = value.indexOf( "-->" );
    int len = QString( "-->" ).length();
    value = value.remove( begin, end - begin + len );

    // Antialiasing for Font
    QFont font = uiHelp->textBrowser->font();
    font.setStyleStrategy( QFont::PreferAntialias );
    uiHelp->textBrowser->setFont(font);

    uiHelp->textBrowser->setText( value );

    file.close();

    show();

    dir.setCurrent( currentdir );

    // remove all tmp files
    for ( int i = 0; i < localFiles.count(); i++  ) {
        QFile file( localFiles.at( i ) );
        file.remove();
    }
}
