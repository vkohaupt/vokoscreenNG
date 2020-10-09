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

#include "QvkBzlib.h"
#include "global.h"

#include <QDebug>
#include <bzlib.h>


QvkBzlib::QvkBzlib()
{
}


QvkBzlib::~QvkBzlib()
{
}


void QvkBzlib::deCompress( QString fileWithPath )
{
    qDebug().noquote() << global::nameOutput << "[h264] bzlibVersion:" << BZ2_bzlibVersion();
    QString unCompresseFileName = fileWithPath;
    char *strIN = fileWithPath.toLatin1().data();

    FILE* inFile;
    BZFILE* bzFile;
    char buf[ 1024 ];
    int bzerror;

    inFile = fopen ( strIN, "r" );
    if ( !inFile )
    {
        /* handle error */
        qDebug().noquote() << global::nameOutput << "[h264] can not open file " << strIN;
        return;
    }

    bzFile = BZ2_bzReadOpen ( &bzerror, inFile, 0, 0, NULL, 0 );
    if ( bzerror != BZ_OK )
    {
        /* handle error */
        BZ2_bzReadClose ( &bzerror, bzFile );
        qDebug().noquote() << global::nameOutput << "[h264] can not read file " << strIN;
        return;
    }

    // https://stackoverflow.com/questions/3912157/how-do-i-extract-all-the-data-from-a-bzip2-archive-with-c
    fileWithPath = fileWithPath.replace( ".bz2", "" );
    char *strOut = fileWithPath.toLatin1().data();
    FILE *outFile;
    outFile = fopen ( strOut, "wb");
    while ( bzerror == BZ_OK )
    {
        int nread = BZ2_bzRead( &bzerror, bzFile, buf, sizeof buf );
        if ( bzerror == BZ_OK || bzerror == BZ_STREAM_END )
        {
            size_t nwritten = fwrite( buf, 1, nread, outFile );
            if ( nwritten != (size_t) nread )
            {
                fprintf( stderr, "E: short write\n" );
            }
        }
    }

    fclose( outFile );
    fclose( inFile );

    if ( bzerror != BZ_STREAM_END )
    {
        BZ2_bzReadClose ( &bzerror, bzFile );
        /* handle error */
    } else
    {
        BZ2_bzReadClose ( &bzerror, bzFile );
    }
    
    qDebug().noquote() << global::nameOutput << "[h264] Decompress" << unCompresseFileName << "to" << fileWithPath;

    emit signal_deCompressed( fileWithPath );
}
