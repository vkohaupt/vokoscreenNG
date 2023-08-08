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

#ifndef QVKSETTINGS_H 
#define QVKSETTINGS_H

#include "QvkRegionChoise.h"
#include "QvkCameraController.h"
#include "QvkSystrayAlternative.h"
#if (QT_VERSION <= QT_VERSION_CHECK(6, 0, 0))
    // Qt5
#include "QvkPlayer.h"
#else
    // Qt6
#endif

#include "QvkHalo.h"
#include "QvkShowClick.h"

#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QMainWindow>

class QvkSettings: public QObject
{ 
    Q_OBJECT
public:
    QvkSettings();
    virtual ~QvkSettings();

    void saveAll( Ui_formMainWindow *ui_mainwindow, QMainWindow *parent, bool log );
    void readAll( Ui_formMainWindow *ui_mainwindow, QMainWindow *parent );

    void saveAreaScreencast( qreal x, qreal y, qreal width, qreal height );
    void readAreaScreencast( QvkRegionChoise *vkRegionChoise );

    void saveCamera( int x, int y );
    void readCamera( QvkCameraController *vkCameraController );

    void saveSystrayAlternative( int x, int y );
    void readSystrayAlternative( QvkSystrayAlternative *vkSystrayAlternative );

#if (QT_VERSION <= QT_VERSION_CHECK(6, 0, 0))
    // Qt5
    void savePlayerPathOpenFile( QString pathFile );
    void readPlayerPathOpenFile( QvkPlayer *vkplayer );
#else
    // Qt6
#endif

    void saveHaloColor( QColor color );
    void readHaloColor( QvkHalo *vkHalo );

    void saveShowclickColor( QColor color );
    void readShowclickColor( QvkShowClick *vkShowClick );

    QString getFileName();
    QString getVideoPath();
    QString getOpenh264ProfilePathWithFilename(); //----------------------------------------------------------


public slots:


private slots:


signals:


protected:  


private:

};

#endif
