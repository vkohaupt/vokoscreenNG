/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2021 Volker Kohaupt
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

#include <QDir>

#include "QvkCiscoOpenh264Controller.h"
#include "QvkCiscoOpenh264Downloader.h"
#include "global.h"
#include <gst/gst.h>


QvkCiscoOpenh264Controller::QvkCiscoOpenh264Controller( QString vk_pathWithSettingsFilename, Ui_formMainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;

    pathWithSettingsFilename = vk_pathWithSettingsFilename;

#ifdef Q_OS_WIN
    libopenh264_filename = "libopenh264.dll";
    QString downloadFile = "http://ciscobinary.openh264.org/openh264-2.1.1-win32.dll.bz2";
#endif
#ifdef Q_OS_LINUX
    libopenh264_filename = "libopenh264.so";
    QString downloadFile = "http://ciscobinary.openh264.org/libopenh264-2.1.1-linux64.6.so.bz2";
#endif

    connect( ui->radioButton_cisco_on,  SIGNAL( clicked( bool ) ), this, SLOT( slot_cisco_on( bool ) ) );
    connect( ui->radioButton_cisco_off, SIGNAL( clicked( bool ) ), this, SLOT( slot_cisco_off( bool ) ) );

    QFileInfo fileInfo( pathWithSettingsFilename );
    QFileInfo fileInfo_libopenh264( fileInfo.path() + "/" + libopenh264_filename );
    if ( !fileInfo_libopenh264.exists() )
    {
        QvkCiscoOpenh264Downloader *vkCiscoOpenh264Downloader = new QvkCiscoOpenh264Downloader( fileInfo.path() );
        connect( vkCiscoOpenh264Downloader, SIGNAL( signal_fileDownloaded( QString ) ), this, SLOT( slot_deCompress( QString ) ) );
        vkCiscoOpenh264Downloader->doDownload( downloadFile );
    }
}


QvkCiscoOpenh264Controller::~QvkCiscoOpenh264Controller()
{
}


void QvkCiscoOpenh264Controller::slot_cisco_on( bool )
{
    int index = ui->comboBoxVideoCodec->findText( "openh264" );
    if ( index == -1 )
    {
        QFileInfo fileInfo( pathWithSettingsFilename );
        QFileInfo fileInfo_libopenh264( fileInfo.path() + "/" + libopenh264_filename );
        if ( fileInfo_libopenh264.exists( ) )
        {
            ui->comboBoxVideoCodec->addItem( "openh264", "openh264enc" );
        }
    }
}


void QvkCiscoOpenh264Controller::slot_cisco_off( bool )
{
    int index = ui->comboBoxVideoCodec->findText( "openh264" );
    if ( index > -1 )
    {
        ui->comboBoxVideoCodec->removeItem( index );
    }
}


#include <QThread>
#include <QCoreApplication>
void QvkCiscoOpenh264Controller::slot_deCompress( QString pathWithDownloadedFile )
{
    QFileInfo fileInfoDownloadedFile( pathWithDownloadedFile );

    QStringList stringList;
    stringList << "filesrc location=\"" + pathWithDownloadedFile + "\"";
    stringList << "bz2dec";
    stringList << "filesink location=\"" + fileInfoDownloadedFile.path() + "/" + libopenh264_filename + "\"";
    QString string = stringList.join( " ! " );
    qDebug().noquote() << global::nameOutput << "[h264]" << string;

    QByteArray byteArray = string.toUtf8();
    const gchar *line = byteArray.constData();
    GError *error = Q_NULLPTR;
    GstElement *pipeline = gst_parse_launch( line, &error );

    // Start playing
    GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PLAYING );
    if ( ret == GST_STATE_CHANGE_FAILURE )
    {
        qDebug().noquote() << global::nameOutput << "[h264]" << "Unable to set the pipeline to the playing state.";
        gst_object_unref( pipeline );
        return;
    }

    // The rest of this code is a, speed workaround. We do this later better.
    int i = 0;
    while ( i < 3000 )
    {
        i = i + 50;
        QCoreApplication::processEvents( QEventLoop::AllEvents );
        QThread::msleep( 100 );
    }

    ret = gst_element_set_state( pipeline, GST_STATE_PAUSED );
    ret = gst_element_set_state( pipeline, GST_STATE_READY );
    ret = gst_element_set_state( pipeline, GST_STATE_NULL );
    gst_object_unref( pipeline );
}
