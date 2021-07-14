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

#ifndef QVKSETTINGS_H 
#define QVKSETTINGS_H

#include "ui_formMainWindow.h"
#include "QvkRegionChoise.h"
#include "QvkCameraController.h"
#include "QvkSystrayAlternative.h"
#include "QvkPlayer.h"
#include "QvkHalo.h"

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

    void saveAll(Ui_formMainWindow *ui_mainwindow, QMainWindow *parent, bool log);
    void readAll(Ui_formMainWindow *ui_mainwindow, QMainWindow *parent);

    void saveAreaScreencast(qreal x, qreal y, qreal width, qreal height);
    void readAreaScreencast(QvkRegionChoise *vkRegionChoise);

    void saveCamera( int x, int y );
    void readCamera( QvkCameraController *vkCameraController );

    void saveSystrayAlternative( int x, int y );
    void readSystrayAlternative( QvkSystrayAlternative *vkSystrayAlternative );

    void savePlayerPathOpenFile(QString pathFile );
    void readPlayerPathOpenFile(QvkPlayer *vkplayer);

    void saveHaloColor( QColor value );
    void readHaloColor( QvkHalo *vkHalo );

    void saveShowclickColor( QColor value );

    QString getFileName();
    QString getVideoPath();


public slots:


private slots:


signals:


protected:  


private:

};

#endif
