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

#ifndef SLIDERVIDEO_H
#define SLIDERVIDEO_H

#include <QSlider>
#include <QWidget>
#include <QMouseEvent>
#include <QDebug>

// Hint: Defined in GUI as placeholder

class slidervideo:public QSlider
{
    Q_OBJECT

public:
    slidervideo(QWidget *parent=0)
    {
        Q_UNUSED(parent);
    }


public slots:


protected:
    void mousePressEvent ( QMouseEvent * event )
    {
        if ( event->button() == Qt::LeftButton )
        {
            if ( orientation() == Qt::Vertical )
            {
                setValue( minimum() + ( ( maximum()-minimum() ) * ( height()-event->y() ) ) / height() ) ;
            }
            else
            {
                setValue(minimum() + ( (maximum()-minimum() ) * event->x() ) / width() ) ;
            }
            event->accept();
        }
        QSlider::mousePressEvent(event);
    }


    void keyPressEvent( QKeyEvent *event )
    {
        if ( ( event->key() == Qt::Key_Right ) or ( event->key() == Qt::Key_Left  ) )
        {
            emit signal_sliderVideo_KeyRight_KeyLeft( value() );
            event->accept();
        }
        QSlider::keyPressEvent( event );
    }


signals:
    void signal_sliderVideo_KeyRight_KeyLeft( int value );


};
#endif // SLIDERVIDEO_H
