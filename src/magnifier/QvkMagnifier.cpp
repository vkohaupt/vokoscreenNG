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

#include "QvkMagnifier.h"

#include <QApplication>
#include <QDebug>
#include <QScreen>
#include <QRegion>

QvkMagnifier::QvkMagnifier()
{
    faktor = 2; // Vergrößerungsfaktor
    label = new QLabel( this );

    slot_magnifier200x200();

    resize( 2 * distanceX * faktor, 2 * distanceY * faktor );
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip ); //With tooltip, no entry in Taskbar
    border = 3;

    label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
    label->setAlignment( Qt::AlignCenter );
    label->setScaledContents( true );

    QPalette pal = QPalette();
    pal.setColor( QPalette::Window, Qt::gray );
    setAutoFillBackground( true );
    setPalette( pal );

    timer = new QTimer( this );
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( 40 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_mytimer() ) );
}


QvkMagnifier::~QvkMagnifier()
{
}


void QvkMagnifier::slot_magnifierShow( bool value )
{
    if ( value == true )
    {
        show();
        timer->start();
    }

    if ( value == false )
    {
        close();
        timer->stop();
    }
}


void QvkMagnifier::slot_magnifier200x200()
{
    distanceX = 50;
    distanceY = 50; // distanceY ist der Abstand Cursor zur Lupe
    resize( 2 * distanceX * faktor, 2 * distanceY * faktor );
    label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
}


void QvkMagnifier::slot_magnifier400x200()
{
    distanceX = 100;
    distanceY = 50; // distanceY ist der Abstand Cursor zur Lupe
    resize( 2 * distanceX * faktor, 2 * distanceY * faktor );
    label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
}


void QvkMagnifier::slot_magnifier600x200()
{
    distanceX = 150;
    distanceY = 50; // distanceY ist der Abstand Cursor zur Lupe
    resize( 2 * distanceX * faktor, 2 * distanceY * faktor );
    label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
}


void QvkMagnifier::setMagnifier()
{
    bool debug = false;

    // Magnifier area middle OK
    // Region beinhalte absolute Bildschirmwerte
    QRegion regionMiddle( this->width()/2,
                          2 * distanceY,
                          screen->size().width() - 2 * this->width()/2,
                          screen->size().height() - 2 * this->height()/2 ) ;

    if ( regionMiddle.contains( screenCursorPos ) )
    {
        move( screen->geometry().left() + screenCursorPos.x() - this->width()/2, globalCursorPos.y() + distanceY );

        if ( debug == true ) { qDebug() << "Magnifier regionMiddle:" << regionMiddle
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }

        return;
    }

    // Magnifier top middle OK
    // Region beinhalte absolute Bildschirmwerte
    QRegion regionTopMiddle( this->width()/2,
                             0,
                             screen->size().width() - 2 * this->width()/2,
                             2 * distanceY );

    if ( regionTopMiddle.contains( screenCursorPos ) )
    {
        move( screen->geometry().left() + screenCursorPos.x() - this->width()/2, screen->geometry().top() + 3 * distanceY);

        if ( debug == true ) { qDebug() << "Magnifier regionTopMiddle:" << regionTopMiddle
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }

        return;
    }


    // Magnifier right OK
    // Region beinhalte absolute Bildschirmwerte
    QRegion regionRight( screen->size().width() - this->width()/2,
                         0,
                         this->width()/2,
                         screen->size().height() - 2 * this->height()/2 );

    if ( regionRight.contains( screenCursorPos ) )
    {
        int valueY = globalCursorPos.y() + distanceY;
        if ( ( screenCursorPos.y() + distanceY ) <= ( 3 * distanceY ) ) {
            valueY = screen->geometry().top() + 3 * distanceY;
        }

        move( screen->geometry().right() - width(), valueY );

        if ( debug == true ) { qDebug() << "Magnifier regionRightMiddle:" << regionRight
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }

        return;
    }


    // Magnifier left OK
    // Region beinhalte absolute Bildschirmwerte
    QRegion regionLeft( 0,
                        0,
                        this->width()/2,
                        screen->size().height() - 2 * this->height()/2 );

    if ( regionLeft.contains( screenCursorPos ) )
    {
        int valueY = globalCursorPos.y() + distanceY;
        if ( ( screenCursorPos.y() + distanceY ) <= ( 3 * distanceY ) ) {
            valueY = screen->geometry().top() + 3 * distanceY;
        }

        move( screen->geometry().left(), valueY );

        if ( debug == true ) { qDebug() << "Magnifier regionLeft:" << regionLeft
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }

        return;
    }
}


void QvkMagnifier::slot_mytimer()
{
    bool debug = false;

    globalCursorPos = QCursor::pos();
    if( debug == true ) { qDebug() << "slot_mytimer globalCursorPos:" << globalCursorPos; }

    screen = QGuiApplication::screenAt( globalCursorPos );
    if ( screen ) {
        if( debug == true ) { qDebug() << "slot_mytimer screen:" << screen; }
    } else {
        // screenAt found no screen. Without a return the Application is crashed
        return;
    }

    screenCursorPos = globalCursorPos - screen->geometry().topLeft(); // screenCursorPos beginnt bei 0 auf dem jeweiliegen Bildschirm
    if( debug == true ) { qDebug() << "slot_mytimer screenCursorPos:" << screenCursorPos; }

    screenList = QGuiApplication::screens();
    if( debug == true ) { qDebug() << "slot_mytimer screenList:" << screenList; }

    screenIndex = screenList.indexOf( screen );
    if( debug == true ) { qDebug() << "slot_mytimer screenIndex:" << screenIndex; }

    QPixmap pixmap;

    setMagnifier();

    // Grab Area middle OK
    QRegion regionAreaMiddle( 0 + this->width()/2,
                              0 + this->height()/4,
                              screen->size().width() - 2 * this->width()/2,
                              screen->size().height() - 2 * this->height()/2 ) ;

    if ( regionAreaMiddle.contains( screenCursorPos ) )
    {
        WId id = 0;
        pixmap = screen->grabWindow( id,
                                     screenCursorPos.x() - distanceX,
                                     screenCursorPos.y() - distanceY,
                                     2 * distanceX ,
                                     2 * distanceY );

        label->setPixmap( pixmap );

        if ( debug == true ) { qDebug() << "Grab regionAreaMiddle:" << regionAreaMiddle
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }
        return;
    }


    // Grab top middle OK
    QRegion regionTopMiddle( 0 + this->width()/2,
                             0,
                             screen->size().width() - 2 * this->width()/2,
                             this->height()/2 );

    if ( regionTopMiddle.contains( screenCursorPos ) ) // screenCursorPos beinhaltet die Cursorkoordinaten vom aktuellen Bildschirm
    {
        WId id = 0;
        pixmap = screen->grabWindow( id,
                                     screenCursorPos.x() - distanceX,
                                     0,
                                     2 * distanceX,
                                     2 * distanceY );

        label->setPixmap( pixmap );

        if ( debug == true ) { qDebug() << "Grab regionTopMiddle:" << regionTopMiddle
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }

        return;
    }


    // Grab right
    QRegion regionRight( screen->size().width() - this->width()/2,
                         // 2 * distanceY,
                         0,
                         this->width()/2,
                         screen->size().height() - 2 * this->height()/2 );

    if ( regionRight.contains( screenCursorPos ) )
    {
        int valueX = screenCursorPos.x() - distanceX;
        if ( screenCursorPos.x() + distanceX >= screen->size().width() ) {
            valueX = screen->size().width() - 2 * distanceX;
        }

        int valueY = screenCursorPos.y() - distanceY;
        if ( screenCursorPos.y() <= distanceY ) {
            valueY = 0;
        }

        WId id = 0;
        pixmap = screen->grabWindow( id,
                                     valueX,
                                     valueY,
                                     this->width()/2,
                                     2 * distanceY );

        label->setPixmap( pixmap );

        if ( debug == true ) { qDebug() << "Grab regionRight:" << regionRight
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }

        return;
    }


    // Grab left OK
    QRegion regionLeft( 0,
                        0,
                        0 + this->width()/2,
                        screen->size().height() - 2 * this->height()/2 );

    if ( regionLeft.contains( screenCursorPos ) )
    {
        int valueX = screenCursorPos.x() - distanceX;
        if ( screenCursorPos.x() <= distanceX ) {
            valueX = 0;
        }

        int valueY = screenCursorPos.y() - distanceY;
        if ( screenCursorPos.y() <= distanceY ) {
            valueY = 0;
        }

        WId id = 0;
        pixmap = screen->grabWindow( id,
                                     valueX,
                                     valueY,
                                     2 * distanceX,
                                     2 * distanceY );

        label->setPixmap( pixmap );

        if ( debug == true ) { qDebug() << "Grab regionLeft:" << regionLeft
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }

        return;
    }
}

/*
void QvkMagnifier::setMagnifier()
{
    // Dient nur zum testen
    QPoint globalCursorPos = QCursor::pos();
    QScreen *screen = QGuiApplication::screenAt( globalCursorPos );
    QPoint screenCursorPos = globalCursorPos - screen->geometry().topLeft();
    QList<QScreen *> screenList = QGuiApplication::screens();
    int screenIndex = screenList.indexOf( screen );

    QCursor cursor;
    // Lupe an oberen linke Ecke setzen
    if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() <  distanceY ) )
    {
        move( 2 * distanceX,  2 * distanceY );
        return;
    }

    // Lupe obere rechte Ecke setzen
    if ( ( cursor.pos().x() > ( rectVirtualScreen.width() - distanceX ) ) and ( cursor.pos().y() < distanceY ) )
    {
        move( rectVirtualScreen.width() - 2 * distanceX - width(), 2 * distanceY);
        return;
    }

    // Lupe am oberen Rand setzen
    // Linke Hälfte am oberen Rand
    if ( ( cursor.pos().y() < distanceY ) and ( cursor.pos().x() < rectVirtualScreen.width() / 2 ) )
    {
        move( cursor.pos().x() + NewDistanceXLeft(), 2 * distanceY );
        return;
    }
    // Rechte Hälfte am oberen Rand
    if ( ( cursor.pos().y() < distanceY ) and ( cursor.pos().x() > rectVirtualScreen.width() / 2 ) )
    {
        move( cursor.pos().x() - NewDistanceXRight() - width(), 2 * distanceY );
        return;
    }

    // Lupe an untere rechte Ecke setzen
    if ( ( cursor.pos().x() > rectVirtualScreen.width() - distanceX ) and ( cursor.pos().y() > rectVirtualScreen.height() - distanceY ) )
    {
        move( rectVirtualScreen.width() - ( 2 * distanceX + width() ), rectVirtualScreen.height() - ( 2 * distanceY + height() ) );
        return;
    }

    // Lupe am rechten Rand setzen

  // Obere Hälfte am rechten Rand
  //if ( ( cursor.pos().x() > rectVirtualScreen.width() - distanceX ) and ( cursor.pos().y() < rectVirtualScreen.height() / 10 * 8 ) )// div 2
  //{
//    move( rectVirtualScreen.width() - ( 2 * distanceX + width() ), cursor.pos().y() + 1 * distanceY );
//    return;
//  }

    // Obere Hälfte am rechten Rand OK
    if ( ( screenCursorPos.x() > screenList.at(screenIndex)->size().width() - width()/2 ) and
         ( screenCursorPos.x() < screenList.at(screenIndex)->size().width() ) and
         ( screenCursorPos.y() < screenList.at(screenIndex)->size().height() / 10 * 8 ) )
    {
        move( globalCursorPos.x() - width(), screenCursorPos.y() + 1 * distanceY );
        qDebug() << "Obere Hälfte am rechten Rand" << globalCursorPos << screenIndex;;
        return;
    }
    return;
    // untere Hälfte am rechten Rand
    if ( ( cursor.pos().x() > rectVirtualScreen.width() - distanceX ) and ( cursor.pos().y() > rectVirtualScreen.height() / 10 * 8 ) )
    {
        move( rectVirtualScreen.width() - ( 2 * distanceX + width() ), cursor.pos().y() - distanceY - height() );
        return;
    }

    // Lupe an linken unteren Ecke setzen
    if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() > rectVirtualScreen.height() - distanceY ) )
    {
        move( 2 * distanceX, rectVirtualScreen.height() - 2 * distanceY - height() );
        return;
    }

    // Lupe am unteren Rand setzen
    // Linke Hälfte unterer Rand
    if ( ( cursor.pos().x() < rectVirtualScreen.width() / 2 ) and ( cursor.pos().y() > rectVirtualScreen.height() - distanceY ) )
    {
        move( cursor.pos().x() + NewDistanceXLeft(), rectVirtualScreen.height() - ( 2 * distanceY + height() ) );
        return;
    }
    // Rechte Hälfte unterer Rand
    if ( ( cursor.pos().x() > rectVirtualScreen.width() / 2 ) and ( cursor.pos().y() > rectVirtualScreen.height() - distanceY ) )
    {
        move( cursor.pos().x() - NewDistanceXRight() - width(), rectVirtualScreen.height() - 2 * distanceY - height() );
        return;
    }

    // Lupe am linken Rand setzen

  // Obere Hälfte am linken Rand
  //if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() < rectVirtualScreen.height() / 10 * 8 ) ) // div 2
  //{
//    move( 2 * distanceX, cursor.pos().y() + distanceY );
//    return;
//  }

    if ( ( globalCursorPos.x() < globalCursorPos.x() + NewDistanceXLeft() ) and ( globalCursorPos.y() < screenList.at(screenIndex)->size().height() / 10 * 8 ) )

    {
        move( globalCursorPos.x() + 2 * distanceX, screenCursorPos.y() + distanceY );
        return;
    }

    // Untere Hälfte am linken Rand
    if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() > rectVirtualScreen.height() / 10 * 8 ) )
    {
        move( 2 * distanceX, cursor.pos().y() - distanceY - height() );
        return;
    }

    // Linke obere Hälfte
    if ( ( cursor.pos().x() < rectVirtualScreen.width() / 2 ) and ( cursor.pos().y() < rectVirtualScreen.height() / 10 * 8 ) )
        move( cursor.pos().x() + NewDistanceXLeft(), cursor.pos().y() + distanceY );

    // Rechte obere Hälfte
    if ( ( cursor.pos().x() > rectVirtualScreen.width() / 2 ) and ( cursor.pos().y() < rectVirtualScreen.height() / 10 * 8 ) )
        move( cursor.pos().x() - NewDistanceXRight() - width(), cursor.pos().y() + distanceY );

    // Linke untere Hälfte
    if ( ( cursor.pos().x() < rectVirtualScreen.width() / 2 ) and ( cursor.pos().y() > rectVirtualScreen.height() / 10 * 8 ) )
        move( cursor.pos().x() + NewDistanceXLeft(), cursor.pos().y() - distanceY - height() );

    // Rechte untere Hälfte
    if ( ( cursor.pos().x() > rectVirtualScreen.width() / 2 ) and ( cursor.pos().y() > rectVirtualScreen.height() / 10 * 8 ) )
        move( cursor.pos().x() - NewDistanceXRight() -width(), cursor.pos().y() - distanceY - height() );
}
*/

/*
void QvkMagnifier::slot_mytimer()
{
    // Dient nur zum testen
    QPoint globalCursorPos = QCursor::pos();
    QScreen *screen_1 = QGuiApplication::screenAt( globalCursorPos );
    QPoint screenCursorPos = globalCursorPos - screen_1->geometry().topLeft();

    QList<QScreen *> screenList = QGuiApplication::screens();
    int screenIndex = screenList.indexOf( screen_1 );
    // Test Ende


    QCursor cursor;
    QPixmap pixmap = QPixmap();
    QList<QScreen *> screen = QGuiApplication::screens();

    setMagnifier();

    // Obere linke Ecke
    if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() <  distanceY ) )
    {
        pixmap = screen.at(0)->grabWindow( 0,
                                           0,
                                           0,
                                           2 * distanceX,
                                           2 * distanceY );
        label->setPixmap( pixmap );
        return;
    }

    // Obere rechte Ecke
    if ( ( cursor.pos().x() > ( rectVirtualScreen.width() - distanceX ) ) and ( cursor.pos().y() < distanceY ) )
    {
        pixmap = screen.at(0)->grabWindow( 0,
                                           rectVirtualScreen.width() - 2 * distanceX,
                                           0,
                                           2 * distanceX ,
                                           2 * distanceY );
        label->setPixmap( pixmap );
        return;
    }

    // Linke untere Ecke
    if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() > rectVirtualScreen.height() - distanceY ) )
    {
        pixmap = screen.at(0)->grabWindow( 0,
                                           0,
                                           rectVirtualScreen.height() - 2 * distanceY,
                                           2 * distanceX ,
                                           2 * distanceY );
        label->setPixmap( pixmap );
        return;
    }

    // Rechte untere Ecke
    if ( ( cursor.pos().x() > rectVirtualScreen.width() - distanceX ) and ( cursor.pos().y() > rectVirtualScreen.height() - distanceY ) )
    {
        pixmap = screen.at(0)->grabWindow( 0,
                                           rectVirtualScreen.width() - 2 * distanceX,
                                           rectVirtualScreen.height() - 2 * distanceY,
                                           2 * distanceX ,
                                           2 * distanceY );
        label->setPixmap( pixmap );
        return;
    }

    // Unterer Rand
    if ( cursor.pos().y() > rectVirtualScreen.height() - distanceY )
    {
        pixmap = screen.at(0)->grabWindow( 0,
                                           cursor.pos().x() - distanceX,
                                           rectVirtualScreen.height() - 2 * distanceY,
                                           2 * distanceX ,
                                           2 * distanceY );
        label->setPixmap( pixmap );
        return;
    }

    // Oberen Rand
    if ( cursor.pos().y() < distanceY )
    {
        pixmap = screen.at(0)->grabWindow( 0,
                                           cursor.pos().x() - distanceX,
                                           0,
                                           2 * distanceX,
                                           2 * distanceY );
        label->setPixmap( pixmap );
        return;
    }

    // Rechter Rand
    if ( cursor.pos().x() > rectVirtualScreen.width() - distanceX )
    {
        pixmap = screen.at(0)->grabWindow( 0,
                                           rectVirtualScreen.width() - 2 * distanceX,
                                           cursor.pos().y() - distanceY,
                                           2 * distanceX ,
                                           2 * distanceY );
        label->setPixmap( pixmap );
        return;
    }

    // Linker Rand
    if ( cursor.pos().x() < distanceX )
    {
        pixmap = screen.at(0)->grabWindow( 0,
                                           0,
                                           cursor.pos().y() - distanceY,
                                           2 * distanceX ,
                                           2 * distanceY );
        label->setPixmap( pixmap );
        return;
    }

    // Fläche
    pixmap = screen.at(0)->grabWindow( 0,
                                       cursor.pos().x() - distanceX,
                                       cursor.pos().y() - distanceY,
                                       2 * distanceX ,
                                       2 * distanceY );
    label->setPixmap( pixmap );
}
*/
