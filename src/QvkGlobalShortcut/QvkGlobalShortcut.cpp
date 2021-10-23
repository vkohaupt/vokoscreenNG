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

#include "QvkGlobalShortcut.h"
#include "global.h"

#include <QDebug>
#include <QCheckBox>
#include <QComboBox>

QvkGlobalShortcut::QvkGlobalShortcut(QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow )
{
    Q_UNUSED(mainWindow);
    ui = ui_mainwindow;

    shortcutStart = new QGlobalShortcut( this );
    connect( shortcutStart, SIGNAL( activated() ), this, SLOT( slot_startStop() ) );
    connect( ui->checkBox_shortcut_start_strg,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_start_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_start_shift, SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_start_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_start_alt,   SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_start_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_start_meta,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_start_clicked( bool ) ) );
    connect( ui->comboBox_shortcut_start, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_checkbox_shortcut_start_currentIndexChanged( int ) ) );

    shortcutPause = new QGlobalShortcut( this );
    connect( shortcutPause, SIGNAL( activated() ), this, SLOT( slot_pauseContinue() ) );
    connect( ui->checkBox_shortcut_pause_strg,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_pause_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_pause_shift, SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_pause_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_pause_alt,   SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_pause_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_pause_meta,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_pause_clicked( bool ) ) );
    connect( ui->comboBox_shortcut_pause, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_checkbox_shortcut_pause_currentIndexChanged( int ) ) );

    shortcutMagnification = new QGlobalShortcut( this );
    connect( shortcutMagnification, SIGNAL( activated() ), ui->checkBoxMagnifier, SLOT( click() ) );
    connect( ui->checkBox_shortcut_magnification_strg,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_magnification_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_magnification_shift, SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_magnification_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_magnification_alt,   SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_magnification_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_magnification_meta,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_magnification_clicked( bool ) ) );
    connect( ui->comboBox_shortcut_magnification, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_checkbox_shortcut_magnification_currentIndexChanged( int ) ) );

    shortcutCamera = new QGlobalShortcut( this );
    connect( shortcutCamera, SIGNAL( activated() ), ui->checkBoxCameraOnOff, SLOT( click() ) );
    connect( ui->checkBox_shortcut_camera_strg,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_camera_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_camera_shift, SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_camera_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_camera_alt,   SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_camera_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_camera_meta,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_camera_clicked( bool ) ) );
    connect( ui->comboBox_shortcut_camera, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_checkbox_shortcut_camera_currentIndexChanged( int ) ) );

    connect( ui->checkBoxStartTime, SIGNAL( clicked( bool ) ), this, SLOT( slot_setOrUnsetShortcut( bool ) ) );
}


QvkGlobalShortcut::~QvkGlobalShortcut()
{
}


QString QvkGlobalShortcut::boolToString( bool bo )
{
    int i = QVariant( bo ).toUInt();
    return QString::number( i );
}


bool QvkGlobalShortcut::isBusy( QString check )
{
   // Beispiel:
   // 1010F

   QString start;;
   QString pause;
   QString magnification;

   start.append( boolToString( ui->checkBox_shortcut_start_strg->isChecked() ) );
   start.append( boolToString( ui->checkBox_shortcut_start_shift->isChecked() ) );
   start.append( boolToString( ui->checkBox_shortcut_start_alt->isChecked() ) );
   start.append( boolToString( ui->checkBox_shortcut_start_meta->isChecked() ) );
   start.append( ui->comboBox_shortcut_start->currentText() );

   pause.append( boolToString( ui->checkBox_shortcut_pause_strg->isChecked() ) );
   pause.append( boolToString( ui->checkBox_shortcut_pause_shift->isChecked() ) );
   pause.append( boolToString( ui->checkBox_shortcut_pause_alt->isChecked() ) );
   pause.append( boolToString( ui->checkBox_shortcut_pause_meta->isChecked() ) );
   pause.append( ui->comboBox_shortcut_pause->currentText() );

   magnification.append( boolToString( ui->checkBox_shortcut_magnification_strg->isChecked() ) );
   magnification.append( boolToString( ui->checkBox_shortcut_magnification_shift->isChecked() ) );
   magnification.append( boolToString( ui->checkBox_shortcut_magnification_alt->isChecked() ) );
   magnification.append( boolToString( ui->checkBox_shortcut_magnification_meta->isChecked() ) );
   magnification.append( ui->comboBox_shortcut_magnification->currentText() );

   bool returnCode = false;
   if ( check == "start" )
   {
       if ( ( start == pause ) or ( start == magnification ) )
         returnCode = true;
   }

   if ( check == "pause" )
   {
       if ( ( pause == start ) or ( pause == magnification) )
         returnCode = true;
   }

   if ( check == "magnification" )
   {
       if ( ( magnification == start ) or ( magnification == pause ) )
         returnCode = true;
   }

   return returnCode;
}


// Start
void QvkGlobalShortcut::slot_checkbox_shortcut_start_clicked( bool value )
{
    Q_UNUSED(value)

    if ( ( ui->checkBox_shortcut_start_strg->isChecked() | ui->checkBox_shortcut_start_shift->isChecked() | ui->checkBox_shortcut_start_alt->isChecked() | ui->checkBox_shortcut_start_meta->isChecked() ) and !isBusy( "start" ) )
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/accept.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_start->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ) );

        QString shortcut;
        if ( ui->checkBox_shortcut_start_strg->isChecked() ) {
            shortcut.append( "+STRG" );
        }
        if ( ui->checkBox_shortcut_start_shift->isChecked() ) {
            shortcut.append( "+SHIFT" );
        }
        if ( ui->checkBox_shortcut_start_alt->isChecked() ) {
            shortcut.append( "+ALT" );
        }
        if ( ui->checkBox_shortcut_start_meta->isChecked() ) {
            shortcut.append( "+META" );
        }

        shortcut.append( "+" + ui->comboBox_shortcut_start->currentText() );

        if ( shortcut.startsWith( "+" ) == true ) {
            shortcut.remove( 0, 1 );
        }

        shortcutStart->unsetShortcut();
        shortcutStart->setShortcut( QKeySequence( shortcut ) );

        qDebug().noquote() << global::nameOutput << "Set global shortcut for Start:" << shortcut;
    } else
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_start->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        shortcutStart->unsetShortcut();
        qDebug().noquote() << global::nameOutput << "Set global shortcut for Start: None";
    }
}

void QvkGlobalShortcut::slot_checkbox_shortcut_start_currentIndexChanged( int value )
{
    Q_UNUSED(value)
    slot_checkbox_shortcut_start_clicked( true );
}


// Pause
void QvkGlobalShortcut::slot_checkbox_shortcut_pause_clicked( bool value )
{
    Q_UNUSED(value)

    if ( ( ui->checkBox_shortcut_pause_strg->isChecked() | ui->checkBox_shortcut_pause_shift->isChecked() | ui->checkBox_shortcut_pause_alt->isChecked() | ui->checkBox_shortcut_pause_meta->isChecked()  ) and !isBusy( "pause" ) )
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/accept.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_pause->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        QString shortcut;
        if ( ui->checkBox_shortcut_pause_strg->isChecked() ) {
            shortcut.append( "+STRG" );
        }
        if ( ui->checkBox_shortcut_pause_shift->isChecked() ) {
            shortcut.append( "+SHIFT" );
        }
        if ( ui->checkBox_shortcut_pause_alt->isChecked() ) {
            shortcut.append( "+ALT" );
        }
        if ( ui->checkBox_shortcut_pause_meta->isChecked() ) {
            shortcut.append( "+META" );
        }

        shortcut.append( "+" + ui->comboBox_shortcut_pause->currentText() );

        if ( shortcut.startsWith( "+" ) == true ) {
            shortcut.remove( 0, 1 );
        }

        shortcutPause->unsetShortcut();
        shortcutPause->setShortcut( QKeySequence( shortcut ) );

        qDebug().noquote() << global::nameOutput << "Set global shortcut for Pause:" << shortcut;
    } else
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_pause->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        shortcutPause->unsetShortcut();
        qDebug().noquote() << global::nameOutput << "Set global shortcut for Pause: None";
    }
}

void QvkGlobalShortcut::slot_checkbox_shortcut_pause_currentIndexChanged( int value )
{
    Q_UNUSED(value)
    slot_checkbox_shortcut_pause_clicked( true );
}


// Magnifier
void QvkGlobalShortcut::slot_checkbox_shortcut_magnification_clicked( bool value )
{
    Q_UNUSED(value)

    if ( ( ui->checkBox_shortcut_magnification_strg->isChecked() | ui->checkBox_shortcut_magnification_shift->isChecked() | ui->checkBox_shortcut_magnification_alt->isChecked() | ui->checkBox_shortcut_magnification_meta->isChecked()  ) and !isBusy( "magnification" ) )
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/accept.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_magnification->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        QString shortcut;
        if ( ui->checkBox_shortcut_magnification_strg->isChecked() ) {
            shortcut.append( "+STRG" );
        }
        if ( ui->checkBox_shortcut_magnification_shift->isChecked() ) {
            shortcut.append( "+SHIFT" );
        }
        if ( ui->checkBox_shortcut_magnification_alt->isChecked() ) {
            shortcut.append( "+ALT" );
        }
        if ( ui->checkBox_shortcut_magnification_meta->isChecked() ) {
            shortcut.append( "+META" );
        }

        shortcut.append( "+" + ui->comboBox_shortcut_magnification->currentText() );

        if ( shortcut.startsWith( "+" ) == true ) {
            shortcut.remove( 0, 1 );
        }

        shortcutMagnification->unsetShortcut();
        shortcutMagnification->setShortcut( QKeySequence( shortcut ) );

        qDebug().noquote() << global::nameOutput << "Set global shortcut for Magnification:" << shortcut;
    } else
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_magnification->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        shortcutMagnification->unsetShortcut();
        qDebug().noquote() << global::nameOutput << "Set global shortcut for Magnification: None";
    }
}


void QvkGlobalShortcut::slot_checkbox_shortcut_magnification_currentIndexChanged( int value )
{
    Q_UNUSED(value)
    slot_checkbox_shortcut_magnification_clicked( true );
}


// Camera
void QvkGlobalShortcut::slot_checkbox_shortcut_camera_clicked( bool value )
{
    Q_UNUSED(value)

    if ( ( ui->checkBox_shortcut_camera_strg->isChecked() | ui->checkBox_shortcut_camera_shift->isChecked() | ui->checkBox_shortcut_camera_alt->isChecked() | ui->checkBox_shortcut_camera_meta->isChecked()  ) and !isBusy( "camera" ) )
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/accept.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_camera->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        QString shortcut;
        if ( ui->checkBox_shortcut_camera_strg->isChecked() ) {
            shortcut.append( "+STRG" );
        }
        if ( ui->checkBox_shortcut_camera_shift->isChecked() ) {
            shortcut.append( "+SHIFT" );
        }
        if ( ui->checkBox_shortcut_camera_alt->isChecked() ) {
            shortcut.append( "+ALT" );
        }
        if ( ui->checkBox_shortcut_camera_meta->isChecked() ) {
            shortcut.append( "+META" );
        }

        shortcut.append( "+" + ui->comboBox_shortcut_camera->currentText() );

        if ( shortcut.startsWith( "+" ) == true ) {
            shortcut.remove( 0, 1 );
        }

        shortcutCamera->unsetShortcut();
        shortcutCamera->setShortcut( QKeySequence( shortcut ) );

        qDebug().noquote() << global::nameOutput << "Set global shortcut for Magnification:" << shortcut;
    } else
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_camera->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        shortcutCamera->unsetShortcut();
        qDebug().noquote() << global::nameOutput << "Set global shortcut for Camera: None";
    }
}

void QvkGlobalShortcut::slot_checkbox_shortcut_camera_currentIndexChanged( int value )
{
    Q_UNUSED(value)
    slot_checkbox_shortcut_camera_clicked( true );
}





void QvkGlobalShortcut::slot_startStop()
{
    if ( ui->pushButtonStart->isEnabled() == true )
    {
        ui->pushButtonStart->click();
    } else
    {
        ui->pushButtonStop->click();
    }
}


void QvkGlobalShortcut::slot_pauseContinue()
{
    if ( ui->pushButtonContinue->isHidden() == true )
    {
        ui->pushButtonPause->click();
    }
    else
    {
        ui->pushButtonContinue->click();
    }
}


void QvkGlobalShortcut::slot_setOrUnsetShortcut( bool value )
{
   if ( value == true )
   {
       shortcutStart->setEnabled( false );
   }
   else
   {
       shortcutStart->setEnabled( true );
   }
}
