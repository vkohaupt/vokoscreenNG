/* vokoscreenNG
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

#include "QvkHaloWindow.h"
#include "QvkPushButton.h"
#include "QvkHaloCircleWidget.h"

#include <QPainter>
#include <QDebug>
#include <QBitmap>


QvkHaloWindow::QvkHaloWindow( QWidget *parent, Ui_formMainWindow *ui_formMainWindow )
{
    ui = ui_formMainWindow;

    setParent( parent );
    setAttribute( Qt::WA_TranslucentBackground, true );
    setCursor( Qt::BlankCursor );
    show();

    setSpezialCheckBox();
    setTimer();
    setColorButtons();
    setSpezialSlider();
    setToolButtonDefaultValues();

    QvkHaloCircleWidget *vkHaloCircleWidget = new QvkHaloCircleWidget( ui->widget );
    vkHaloCircleWidget->setObjectName( "widgetHaloCircle" );
    vkHaloCircleWidget->show();


}


QvkHaloWindow::~QvkHaloWindow()
{
}


void QvkHaloWindow::init()
{
    diameter = vkSpezialSliderDiameter->value();

    qreal a = vkSpezialSliderOpacity->value();
    opacity = a / 100.0;

    resize( diameter, diameter );

    QRegion window( 0,
                    0,
                    diameter,
                    diameter,
                    QRegion::Rectangle );

    QRegion mouseHole( diameter / 2 - 1,
                       diameter / 2 - 1,
                       2,
                       2,
                       QRegion::Rectangle );

    QRegion r1 = window.QRegion::subtracted( mouseHole );

    this->setMask( r1 );
}


void QvkHaloWindow::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event);

    QPixmap pixmap( 100 * devicePixelRatioF(), 100 * devicePixelRatioF() );
    pixmap.fill( Qt::transparent );
    pixmap.setDevicePixelRatio( devicePixelRatioF() );

    QPainter painterPixmap;
    painterPixmap.begin( &pixmap );
    painterPixmap.setRenderHints( QPainter::Antialiasing, true );

    qreal penWith = 1.0;
    QPen pen;
    pen.setWidthF( penWith );
    pen.setColor( color );
    pen.setStyle( Qt::SolidLine );
    painterPixmap.setPen( pen );

    QBrush brush;
    brush.setColor( color );
    brush.setStyle( Qt::SolidPattern );
    painterPixmap.setBrush( color );
    painterPixmap.setOpacity( opacity );
    painterPixmap.drawEllipse( 1, 1, diameter-2, diameter-2 );

    painterPixmap.end();

    QPainter painter;
    painter.begin( this );
    painter.drawPixmap( QPoint( 0, 0 ), pixmap );
    painter.end();
}


void QvkHaloWindow::setTimer()
{
    timer = new QTimer(this);
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_followMouse() ) );
}


void QvkHaloWindow::slot_followMouse()
{
    QCursor cursor;
    move( cursor.pos().x() - diameter/2, cursor.pos().y() - diameter/2 );
}


void QvkHaloWindow::setSpezialCheckBox()
{
    vkSpezialCheckbox = new QvkSpezialCheckbox();
    vkSpezialCheckbox->setObjectName( "spezialCheckBoxHalo" );
    ui->horizontalLayout_48->insertWidget( 1, vkSpezialCheckbox );
    connect( vkSpezialCheckbox, SIGNAL( signal_clicked( bool ) ), this, SLOT( slot_haloOnOff( bool ) ) );
}


void QvkHaloWindow::slot_haloOnOff( bool value )
{
    if ( value == true )
    {
        timer->start( 40 );
        show();
    }

    if ( value == false )
    {
        timer->stop();
        hide();
    }
}


void QvkHaloWindow::setSpezialSlider()
{
    vkSpezialSliderDiameter = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayoutDiameter->insertWidget( 0, vkSpezialSliderDiameter );
    vkSpezialSliderDiameter->setObjectName( "sliderhaloDiameter" );
    vkSpezialSliderDiameter->setTracking( true );
    vkSpezialSliderDiameter->setMinimum( 50 );
    vkSpezialSliderDiameter->setMaximum( 100 );
    vkSpezialSliderDiameter->setValue( 80 );
    vkSpezialSliderDiameter->setShowValue( false );
    vkSpezialSliderDiameter->show();
    connect( vkSpezialSliderDiameter, SIGNAL( valueChanged( int ) ), this, SLOT( slot_valueChanged_SpezialSliderDiameter( int ) ) );

    vkSpezialSliderOpacity = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayoutHaloOpacity->insertWidget( 0, vkSpezialSliderOpacity );
    vkSpezialSliderOpacity->setObjectName( "sliderhaloOpacity" );
    vkSpezialSliderOpacity->setTracking( true );
    vkSpezialSliderOpacity->setMinimum( 1 );
    vkSpezialSliderOpacity->setMaximum( 100 );
    vkSpezialSliderOpacity->setValue( 60 );
    vkSpezialSliderOpacity->setShowValue( false );
    vkSpezialSliderOpacity->show();
    connect( vkSpezialSliderOpacity, SIGNAL( valueChanged( int ) ), this, SLOT( slot_valueChanged_SpezialSliderDiameter( int ) ) );

}


void QvkHaloWindow::slot_valueChanged_SpezialSliderDiameter( int value )
{
    Q_UNUSED(value)
    init();
    repaint();
}


void QvkHaloWindow::setToolButtonDefaultValues()
{
    connect( ui->toolButtonDiameterDefault,  SIGNAL( clicked() ), this, SLOT( slot_DiameterDefault() ) );
    connect( ui->toolButtonOpacityDefault, SIGNAL( clicked() ), this, SLOT( slot_OpacityDefault() ) );
}


void QvkHaloWindow::slot_DiameterDefault()
{
    vkSpezialSliderDiameter->setValue( 80 );
}


void QvkHaloWindow::slot_OpacityDefault()
{
    vkSpezialSliderOpacity->setValue( 60 );
}


void QvkHaloWindow::setColorButtons()
{
    int minimumHeight = 34;
    QvkPushButton *vkPushButton_halo_white = new QvkPushButton( Qt::white );
    vkPushButton_halo_white->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_white->addWidget( vkPushButton_halo_white );
    connect( vkPushButton_halo_white, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_white() ) );

    QvkPushButton *vkPushButton_halo_black = new QvkPushButton( Qt::black );
    vkPushButton_halo_black->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_black->addWidget( vkPushButton_halo_black );
    connect( vkPushButton_halo_black, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_black() ) );

    QvkPushButton *vkPushButton_halo_red = new QvkPushButton( Qt::red );
    vkPushButton_halo_red->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_red->addWidget( vkPushButton_halo_red );
    connect( vkPushButton_halo_red, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_red() ) );

    QvkPushButton *vkPushButton_halo_darkRed = new QvkPushButton( Qt::darkRed );
    vkPushButton_halo_darkRed->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_darkRed->addWidget( vkPushButton_halo_darkRed );
    connect( vkPushButton_halo_darkRed, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_darkRed() ) );

    QvkPushButton *vkPushButton_halo_green = new QvkPushButton( Qt::green );
    vkPushButton_halo_green->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_green->addWidget( vkPushButton_halo_green );
    connect( vkPushButton_halo_green, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_green() ) );

    QvkPushButton *vkPushButton_halo_darkGreen = new QvkPushButton( Qt::darkGreen );
    vkPushButton_halo_darkGreen->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_darkGreen->addWidget( vkPushButton_halo_darkGreen );
    connect( vkPushButton_halo_darkGreen, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_darkGreen() ) );

    QvkPushButton *vkPushButton_halo_blue = new QvkPushButton( Qt::blue );
    vkPushButton_halo_blue->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_blue->addWidget( vkPushButton_halo_blue );
    connect( vkPushButton_halo_blue, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_blue() ) );

    QvkPushButton *vkPushButton_halo_darkBlue = new QvkPushButton( Qt::darkBlue );
    vkPushButton_halo_darkBlue->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_darkBlue->addWidget( vkPushButton_halo_darkBlue );
    connect( vkPushButton_halo_darkBlue, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_darkBlue() ) );

    QvkPushButton *vkPushButton_halo_cyan = new QvkPushButton( Qt::cyan );
    vkPushButton_halo_cyan->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_cyan->addWidget( vkPushButton_halo_cyan );
    connect( vkPushButton_halo_cyan, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_cyan() ) );

    QvkPushButton *vkPushButton_halo_darkCyan = new QvkPushButton( Qt::darkCyan );
    vkPushButton_halo_darkCyan->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_darkCyan->addWidget( vkPushButton_halo_darkCyan );
    connect( vkPushButton_halo_darkCyan, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_darkCyan() ) );

    QvkPushButton *vkPushButton_halo_magenta = new QvkPushButton( Qt::magenta );
    vkPushButton_halo_magenta->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_magenta->addWidget( vkPushButton_halo_magenta );
    connect( vkPushButton_halo_magenta, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_magenta() ) );

    QvkPushButton *vkPushButton_halo_darkMagenta = new QvkPushButton( Qt::darkMagenta );
    vkPushButton_halo_darkMagenta->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_darkMagenta->addWidget( vkPushButton_halo_darkMagenta );
    connect( vkPushButton_halo_darkMagenta, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_darkMagenta() ) );

    QvkPushButton *vkPushButton_halo_yellow = new QvkPushButton( Qt::yellow );
    vkPushButton_halo_yellow->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_yellow->addWidget( vkPushButton_halo_yellow );
    connect( vkPushButton_halo_yellow, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_yellow() ) );

    QvkPushButton *vkPushButton_halo_darkYellow = new QvkPushButton( Qt::darkYellow );
    vkPushButton_halo_darkYellow->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_darkYellow->addWidget( vkPushButton_halo_darkYellow );
    connect( vkPushButton_halo_darkYellow, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_darkYellow() ) );

    QvkPushButton *vkPushButton_halo_gray = new QvkPushButton( Qt::gray );
    vkPushButton_halo_gray->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_gray->addWidget( vkPushButton_halo_gray );
    connect( vkPushButton_halo_gray, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_gray() ) );

    QvkPushButton *vkPushButton_halo_darkGray = new QvkPushButton( Qt::darkGray );
    vkPushButton_halo_darkGray->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_darkGray->addWidget( vkPushButton_halo_darkGray );
    connect( vkPushButton_halo_darkGray, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_darkGray() ) );
}


void QvkHaloWindow::slot_vkPushButton_halo_white()
{
    color = Qt::white;
    repaint();
}

void QvkHaloWindow::slot_vkPushButton_halo_black()
{
    color = Qt::black;
    repaint();
}

void QvkHaloWindow::slot_vkPushButton_halo_red()
{
    color = Qt::red;
    repaint();
}

void QvkHaloWindow::slot_vkPushButton_halo_darkRed()
{
    color = Qt::darkRed;
    repaint();
}

void QvkHaloWindow::slot_vkPushButton_halo_green()
{
    color = Qt::green;
    repaint();
}

void QvkHaloWindow::slot_vkPushButton_halo_darkGreen()
{
    color = Qt::darkGreen;
    repaint();
}

void QvkHaloWindow::slot_vkPushButton_halo_blue()
{
    color = Qt::blue;
    repaint();
}

void QvkHaloWindow::slot_vkPushButton_halo_darkBlue()
{
    color = Qt::darkBlue;
    repaint();
}

void QvkHaloWindow::slot_vkPushButton_halo_cyan()
{
    color = Qt::cyan;
    repaint();
}

void QvkHaloWindow::slot_vkPushButton_halo_darkCyan()
{
    color = Qt::darkCyan;
    repaint();
}

void QvkHaloWindow::slot_vkPushButton_halo_magenta()
{
    color = Qt::magenta;
    repaint();
}

void QvkHaloWindow::slot_vkPushButton_halo_darkMagenta()
{
    color = Qt::darkMagenta;
    repaint();
}

void QvkHaloWindow::slot_vkPushButton_halo_yellow()
{
    color = Qt::yellow;
    repaint();
}

void QvkHaloWindow::slot_vkPushButton_halo_darkYellow()
{
    color = Qt::darkYellow;
    repaint();
}

void QvkHaloWindow::slot_vkPushButton_halo_gray()
{
    color = Qt::gray;
    repaint();
}

void QvkHaloWindow::slot_vkPushButton_halo_darkGray()
{
    color = Qt::darkGray;
    repaint();
}
