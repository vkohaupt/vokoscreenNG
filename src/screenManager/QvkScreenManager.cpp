/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
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

#include "QvkScreenManager.h"
#include "global.h"

#include <QScreen>
#include <QGuiApplication>
#include <QDebug>

#include <QApplication>

QvkScreenManager::QvkScreenManager()
{
    connect( qApp, SIGNAL( screenAdded( QScreen* ) ),   this, SLOT( slot_screen_count_changed() ) );
    connect( qApp, SIGNAL( screenRemoved( QScreen* ) ), this, SLOT( slot_screen_count_changed() ) );
    connect( qApp, SIGNAL( primaryScreenChanged( QScreen* ) ), this, SLOT( slot_screen_count_changed() ) );
}


void QvkScreenManager::init()
{
    slot_screen_count_changed();
}


QvkScreenManager::~QvkScreenManager()
{}

void QvkScreenManager::slot_screen_count_changed()
{
    QList<QScreen *> screen = QGuiApplication::screens();
    if ( !screen.empty() )
    {
        emit signal_clear_widget();
        for ( int i = 0; i < screen.size(); i++ )
        {
            if ( screen.at(i)->name() == QGuiApplication::primaryScreen()->name() )
            {
                qDebug().noquote() << global::nameOutput << "This screen is the primary screen: " << QGuiApplication::primaryScreen()->name();
            }
            qDebug().noquote() << global::nameOutput << "Name from screen: " << screen.at(i)->name();
            qDebug().noquote() << global::nameOutput << "Screen available desktop width :" << QString::number( screen.at(i)->geometry().width() * screen.at(i)->devicePixelRatio() );
            qDebug().noquote() << global::nameOutput << "Screen available desktop height:" << QString::number( screen.at(i)->geometry().height() * screen.at(i)->devicePixelRatio() );
            qDebug().noquote() << global::nameOutput << "DevicePixelRatio:" << screen.at(i)->devicePixelRatio() << " (Normal displays is 1, Retina display is 2)";
            qDebug().noquote() << global::nameOutput << "Vertical refresh rate of the screen in Hz:" << screen.at(i)->refreshRate();
            qDebug().noquote() << global::nameOutput << "Screen orientation" << screen.at(i)->orientation();
            qDebug().noquote() << global::nameOutput << "Color depth of the screen: " << screen.at(i)->depth();
            qDebug().noquote() << global::nameOutput << "Model from screen: " << screen.at(i)->model();
            qDebug().noquote() << global::nameOutput << "Manufactur from screen: " << screen.at(i)->manufacturer();
            qDebug().noquote() << global::nameOutput << "SerialNumber from screen: " << screen.at(i)->serialNumber();

            QString X = QString::number( static_cast<int>( screen.at(i)->geometry().left() * screen.at(i)->devicePixelRatio() ) );
            QString Y = QString::number( static_cast<int>( screen.at(i)->geometry().top() * screen.at(i)->devicePixelRatio() ) );
            QString Width = QString::number( static_cast<int>( screen.at(i)->geometry().width() * screen.at(i)->devicePixelRatio() ) );
            QString Height = QString::number( static_cast<int>( screen.at(i)->geometry().height() * screen.at(i)->devicePixelRatio() ) );
            QString stringText = screen.at(i)->name() + " " + ":  " + Width + " x " + Height;
            QString stringData = "x=" + X + " " +
                                 "y=" + Y + " " +
                                 "with=" + Width + " " +
                                 "height=" + Height;

            disconnect( screen.at(i), nullptr, nullptr, nullptr );
            connect( screen.at(i), SIGNAL( geometryChanged( const QRect ) ), this, SLOT( slot_geometryChanged( const QRect ) ) );

            emit signal_screen_count_changed( stringText, stringData );
        }
    }
}

void QvkScreenManager::slot_geometryChanged( const QRect &rect )
{
    Q_UNUSED(rect);
    slot_screen_count_changed();
}

