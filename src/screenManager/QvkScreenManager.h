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

#ifndef QVKSCREENMANAGER_H 
#define QVKSCREENMANAGER_H

#include <QObject>
#include <QMainWindow>
#include <QLabel>

class QvkScreenManager : public QObject
{
    Q_OBJECT

public:
    QvkScreenManager( QMainWindow *parent );
    virtual ~QvkScreenManager();
    void init();


public slots:


private slots:
    void slot_screen_count_changed();
    void slot_geometryChanged( const QRect &rect );
    void slot_toolButton_toggled( bool checked );


signals:
    void signal_clear_widget();
    void signal_screen_count_changed( QString stringText, QString stringData );


protected:  


private:
    QList<QLabel *> labelList;

};

#endif
