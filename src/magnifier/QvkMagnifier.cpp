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

#include "QvkMagnifier.h"

#include <QApplication>
#include <QDebug>
#include <QScreen>
#include <QRegion>
#include <QPainter>
#include <QPaintEvent>
#include <QBitmap>
#include <QPainterPath>

QvkMagnifier::QvkMagnifier()
{
    factor = 2; // Vergrößerungsfaktor

    slot_magnifier200x200();

    resize( 2 * distanceX * factor, 2 * distanceY * factor );
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip );
    setAttribute( Qt::WA_TranslucentBackground, true );
    distanceCopyMagnifier = 30;

    timer = new QTimer( this );
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( 40 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_mytimer() ) );
}


QvkMagnifier::~QvkMagnifier()
{
}


void QvkMagnifier::slot_setMagnifierChangedSize(int value )
{
    factor = value;
    resize( 2 * distanceX * factor, 2 * distanceY * factor );
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
    resize( 2 * distanceX * factor, 2 * distanceY * factor );
}


void QvkMagnifier::slot_magnifier400x200()
{
    distanceX = 100;
    distanceY = 50; // distanceY ist der Abstand Cursor zur Lupe
    resize( 2 * distanceX * factor, 2 * distanceY * factor );
}


void QvkMagnifier::slot_magnifier600x200()
{
    distanceX = 150;
    distanceY = 50; // distanceY ist der Abstand Cursor zur Lupe
    resize( 2 * distanceX * factor, 2 * distanceY * factor );
}


void QvkMagnifier::setMagnifier()
{
    nameRegion = region::none;

    // Magnifier top left
    // Region includes absolute screen values
    regionTopLeft = QRegion( 0,
                             0,
                             this->width()/2,
                             this->height()/2 );

    if ( regionTopLeft.contains( screenCursorPos ) )
    {
        nameRegion = region::topLeft;
        valueRegion = regionTopLeft;

        int valueY = globalCursorPos.y() + distanceY + distanceCopyMagnifier;
        if ( screenCursorPos.y() <= distanceY ) {
            //                                   div zum oberen Rand                     Unterhalb cursor
            valueY = globalCursorPos.y() + ( distanceY - screenCursorPos.y() ) + distanceY + distanceCopyMagnifier;
        }

        // Move works with global mouse coordinates like screen->geometry().left() and globalCursorPos
        move( screen->geometry().left(), valueY  );

        return;
    }


    // Magnifier top middle
    // Region includes absolute screen values
    regionTopMiddle = QRegion( this->width()/2,
                               0,
                               screen->size().width() - 2*this->width()/2,
                               this->height()/2 );

    if ( regionTopMiddle.contains( screenCursorPos ) )
    {
        nameRegion = region::topMiddle;
        valueRegion = regionTopMiddle;

        int valueY = globalCursorPos.y() + distanceY + distanceCopyMagnifier;
        if ( screenCursorPos.y() <= distanceY ) {
            //                                   div zum oberen Rand                     Unterhalb cursor
            valueY = globalCursorPos.y() + ( distanceY - screenCursorPos.y() ) + distanceY + distanceCopyMagnifier;
        }

        // Move works with global mouse coordinates like screen->geometry().left() and globalCursorPos
        move( globalCursorPos.x() - width()/2, valueY );

        return;
    }


    // Magnifier top right
    // Region includes absolute screen values
    regionTopRight = QRegion( screen->size().width() - this->width()/2,
                              0,
                              this->width()/2,
                              this->height()/2 );

    if ( regionTopRight.contains( screenCursorPos ) )
    {
        nameRegion = region::topRight;
        valueRegion = regionTopRight;

        int valueY = globalCursorPos.y() + distanceY + distanceCopyMagnifier;
        if ( screenCursorPos.y() <= distanceY ) {
            //                                   div zum oberen Rand                     Unterhalb cursor
            valueY = globalCursorPos.y() + ( distanceY - screenCursorPos.y() ) + distanceY + distanceCopyMagnifier;
        }

        // Move works with global mouse coordinates like screen->geometry().left() and globalCursorPos
        move( screen->geometry().right() - this->width(), valueY );

        return;
    }


    // Magnifier right middle
    // Region includes absolute screen values
    regionRightMiddle = QRegion( screen->size().width() - this->width()/2,
                                 this->height()/2,
                                 this->width()/2,
                                 //                        (    Top       )   (             Bottom                      )
                                 screen->size().height() - this->height()/2 - ( this->height() + distanceY + distanceCopyMagnifier ) );

    if ( regionRightMiddle.contains( screenCursorPos ) )
    {
        nameRegion = region::rightMiddle;
        valueRegion = regionRightMiddle;

        // Move works with global mouse coordinates like screen->geometry().left() and globalCursorPos
        move( screen->geometry().right() - this->width(), globalCursorPos.y() + distanceY + distanceCopyMagnifier );

        return;
    }


    // Magnifier bottom right
    // Region includes absolute screen values
    regionBottomRight = QRegion( screen->size().width() - this->width()/2,
                                 screen->size().height() - this->height() - distanceY - distanceCopyMagnifier,
                                 this->width()/2,
                                 this->height() + 2*distanceY + distanceCopyMagnifier );

    if ( regionBottomRight.contains( screenCursorPos ) )
    {
        nameRegion = region::bottomRight;
        valueRegion = regionBottomRight;

        int valueY = globalCursorPos.y() - ( distanceY + distanceCopyMagnifier + this->height() );
        if ( screenCursorPos.y() >= ( screen->size().height() - distanceY ) ) {
            valueY = globalCursorPos.y() - ( screenCursorPos.y() + ( distanceY - screen->size().height() ) ) - ( distanceY + distanceCopyMagnifier + this->height() );
        }

        // Move works with global mouse coordinates like screen->geometry().left() and globalCursorPos
        move( screen->geometry().right() - this->width(), valueY );

        return;
    }


    // Magnifier bottom middle
    // Region includes absolute screen values
    regionBottomMiddle = QRegion( this->width()/2,
                                  screen->size().height() - this->height() - distanceY - distanceCopyMagnifier,
                                  screen->size().width() - 2*this->width()/2,
                                  this->height() + 2*distanceY + distanceCopyMagnifier );

    if ( regionBottomMiddle.contains( screenCursorPos ) )
    {
        nameRegion = region::bottomMiddle;
        valueRegion = regionBottomMiddle;

        int valueY = globalCursorPos.y() - ( distanceY + distanceCopyMagnifier + this->height() );
        if ( screenCursorPos.y() >= ( screen->size().height() - distanceY ) ) {
            valueY = globalCursorPos.y() - ( screenCursorPos.y() + ( distanceY - screen->size().height() ) ) - ( distanceY + distanceCopyMagnifier + this->height() );
        }

        // Move works with global mouse coordinates like screen->geometry().left() and globalCursorPos
        move( globalCursorPos.x() - this->width()/2, valueY );

        return;
    }


    // Magnifier bottom left
    // Region includes absolute screen values
    regionBottomLeft = QRegion( 0,
                                screen->size().height() - this->height() - distanceY - distanceCopyMagnifier,
                                this->width()/2,
                                this->height() + 2*distanceY + distanceCopyMagnifier );

    if ( regionBottomLeft.contains( screenCursorPos ) )
    {
        nameRegion = region::bottomLeft;
        valueRegion = regionBottomLeft;

        int valueY = globalCursorPos.y() - ( distanceY + distanceCopyMagnifier + this->height() );
        if ( screenCursorPos.y() >= ( screen->size().height() - distanceY ) ) {
            valueY = globalCursorPos.y() - ( screenCursorPos.y() + ( distanceY - screen->size().height() ) ) - ( distanceY + distanceCopyMagnifier + this->height() );
        }

        // Move works with global mouse coordinates like screen->geometry().left() and globalCursorPos
        move( screen->geometry().left(), valueY );

        return;
    }


    // Magnifier left middle
    // Region includes absolute screen values
    regionLeftMiddle = QRegion( 0,
                                this->height()/2,
                                this->width()/2,
                                //                        (    Top       )   (             Bottom                      )
                                screen->size().height() - this->height()/2 - ( this->height() + distanceY + distanceCopyMagnifier ) );

    if ( regionLeftMiddle.contains( screenCursorPos ) )
    {
        nameRegion = region::leftMiddle;
        valueRegion = regionLeftMiddle;

        int valueX = screen->geometry().left();
        int valueY = globalCursorPos.y() + distanceY + distanceCopyMagnifier;

        // Move works with global mouse coordinates like screen->geometry().left() and globalCursorPos
        move( valueX, valueY );

        return;
    }


    // Magnifier middle
    // Region includes absolute screen values
    regionMiddle = QRegion( this->width()/2,
                            this->height()/2,
                            screen->size().width() - 2*this->width()/2,
                            screen->size().height() - this->height()/2 - ( this->height() + distanceY + distanceCopyMagnifier ) );

    if ( regionMiddle.contains( screenCursorPos ) )
    {
        nameRegion = region::middle;
        valueRegion = regionMiddle;

        int valueX = globalCursorPos.x() - width()/2;
        int valueY = globalCursorPos.y() + distanceY + distanceCopyMagnifier;

        // Move works with global mouse coordinates like screen->geometry().left() and globalCursorPos
        move( valueX, valueY );

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
        emit signal_magnifier_on_screen( screen );
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

    // Begin grab
    int valueX = 0;
    int valueY = 0;
    switch ( nameRegion )
    {
        case none : {
                break;
            }
        case topLeft : {
                valueX = screenCursorPos.x() - distanceX;
                if ( screenCursorPos.x() - distanceX <= 0 ) {
                    valueX = 0;
                }

                valueY = screenCursorPos.y() - distanceY;
                if ( screenCursorPos.y() <= distanceY ) {
                    valueY = 0;
                }

                break;
            }
        case topMiddle : {
                valueX = screenCursorPos.x() - distanceX;

                valueY = screenCursorPos.y() - distanceY;
                if ( screenCursorPos.y() <= distanceY ) {
                    valueY = 0;
                }

                break;
            }
        case topRight : {
                valueX = screenCursorPos.x() - distanceX;
                if ( screenCursorPos.x() >= screen->size().width() - distanceX ) {
                    valueX = screen->size().width() - 2*distanceX;
                }

                valueY = screenCursorPos.y() - distanceY;
                if ( screenCursorPos.y() <= distanceY ) {
                    valueY = 0;
                }

                break;
            }
        case rightMiddle : {
                valueX = screenCursorPos.x() - distanceX;
                if ( screenCursorPos.x() >= screen->size().width() - distanceX ) {
                    valueX = screen->size().width() - 2*distanceX;
                }

                valueY = screenCursorPos.y() - distanceY;
                if ( screenCursorPos.y() <= distanceY ) {
                    valueY = 0;
                }

                break;
            }
        case bottomRight : {
                valueX = screenCursorPos.x() - distanceX;
                if ( screenCursorPos.x() >= screen->size().width() - distanceX ) {
                    valueX = screen->size().width() - 2*distanceX;
                }

                valueY = screenCursorPos.y() - distanceY;
                if ( screenCursorPos.y() >= screen->size().height() - distanceY ) {
                    valueY = screen->size().height() - 2*distanceY;
                }

                break;
            }
        case bottomMiddle : {
                valueX = screenCursorPos.x() - distanceX;

                valueY = screenCursorPos.y() - distanceY;
                if ( screenCursorPos.y() >= screen->size().height() - distanceY ) {
                    valueY = screen->size().height() - 2*distanceY;
                }

                break;
            }
        case bottomLeft : {
                valueX = screenCursorPos.x() - distanceX;
                if ( screenCursorPos.x() - distanceX <= 0 ) {
                    valueX = 0;
                }

                valueY = screenCursorPos.y() - distanceY;
                if ( screenCursorPos.y() >= screen->size().height() - distanceY ) {
                    valueY = screen->size().height() - 2*distanceY;
                }

                break;
            }
        case leftMiddle : {
                valueX = screenCursorPos.x() - distanceX;
                if ( screenCursorPos.x() - distanceX <= 0 ) {
                    valueX = 0;
                }

                valueY = screenCursorPos.y() - distanceY;
                if ( screenCursorPos.y() >= screen->size().height() - distanceY ) {
                    valueY = screen->size().height() - 2*distanceY;
                }

                break;
            }
        case middle : {
                valueX = screenCursorPos.x() - distanceX;

                valueY = screenCursorPos.y() - distanceY;

                break;
            }
    }

    if ( nameRegion != region::none ) {
        pixmap = screen->grabWindow( 0,
                                     valueX,
                                     valueY,
                                     2*distanceX,
                                     2*distanceY);

        if ( pixmap.isNull() == true ) {
                    return;
        }

        QPixmap painterPixmap( pixmap.width() * factor, pixmap.height() * factor );
        painterPixmap.fill( Qt::transparent );

        QPainter painter;
        painter.begin( &painterPixmap );
        painter.setRenderHints( QPainter::Antialiasing, true );
        painter.setRenderHint( QPainter::SmoothPixmapTransform, true );

        if ( ( isToolButtonElipse == true ) or ( isToolButtonCircle == true ) ) {
                    QPainterPath path;
                    path.addEllipse( 0, 0, width(), height() );
                    painter.setClipPath( path );
        }

        QPixmap pix = pixmap.scaled( painterPixmap.width(), painterPixmap.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
        painter.drawPixmap( QPoint( 0, 0 ), pix );

        QPen pen;
        pen.setWidth( 7 );
        pen.setColor( Qt::lightGray );
        painter.setPen( pen );
        if ( ( isToolButtonElipse == true ) or ( isToolButtonCircle == true ) ) {
                    painter.drawEllipse( 0, 0, width(), height() );
        } else {
                    painter.drawRect( 0, 0, width(), height() );
        }
        painter.end();

        setPixmap( painterPixmap );
        setMagnifier();

#ifdef Q_OS_LINUX
        setMask( painterPixmap.mask() );
#endif

        if ( debug == true ) { qDebug().noquote() << "Grab" << enumToString( nameRegion ) << valueRegion
                                                  << "globalCursorPos:" << globalCursorPos
                                                  << "screenCursorPos:" << screenCursorPos << screenIndex; }
    }
}


QString QvkMagnifier::enumToString( region reg )
{
    QString string;
    switch ( reg )
    {
        case none : {
                string = "none";
                break;
            }
        case topLeft : {
                string = "topLeft";
                break;
            }
        case topMiddle : {
                string = "topMiddle";
                break;
            }
        case topRight : {
                string = "topRight";
                break;
            }
        case rightMiddle : {
                string = "rightMiddle";
                break;
            }
        case bottomRight : {
                string = "bottomRight";
                break;
            }
        case bottomMiddle : {
                string = "bottomMiddle";
                break;
            }
        case bottomLeft : {
                string = "bottomLeft";
                break;
            }
        case leftMiddle : {
                string = "leftMiddle";
                break;
            }
        case middle : {
                string = "middle";
                break;
            }
    }

    return string;
}
