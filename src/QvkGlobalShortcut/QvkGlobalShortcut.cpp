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

    QGlobalShortcut *shortcutWebcam = new QGlobalShortcut( this );
    connect( shortcutWebcam, SIGNAL( activated() ), ui->checkBoxCameraOnOff, SLOT( click() ) );
    shortcutWebcam->setShortcut( QKeySequence( "Ctrl+Shift+F8" ) );

    QGlobalShortcut *shortcutMagnifier = new QGlobalShortcut( this );
    connect( shortcutMagnifier, SIGNAL( activated() ), ui->checkBoxMagnifier, SLOT( click() ) );
    shortcutMagnifier->setShortcut( QKeySequence( "Ctrl+Shift+F9" ) );

//    shortcutStart = new QGlobalShortcut( this );
//    connect( shortcutStart, SIGNAL( activated() ), ui->pushButtonStart, SLOT( click() ) );
//    shortcutStart->setShortcut( QKeySequence( "Ctrl+Shift+F10" ) );

//    shortcutStop = new QGlobalShortcut( this );
//    connect( shortcutStop, SIGNAL( activated() ), ui->pushButtonStop, SLOT( click() ) );
//    shortcutStop->setShortcut( QKeySequence( "Ctrl+Shift+F11" ) );

//    QGlobalShortcut *shortcutPauseContinue = new QGlobalShortcut( this );
//    connect( shortcutPauseContinue, SIGNAL( activated() ), this, SLOT( slot_pauseContinue() ) );
//    shortcutPauseContinue->setShortcut( QKeySequence( "Ctrl+Shift+F12" ) );

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


bool QvkGlobalShortcut::isBusy()
{
   // Beispiel:
   // 1010F

   QString start;;
   start.append( boolToString( ui->checkBox_shortcut_start_strg->isChecked() ) );
   start.append( boolToString( ui->checkBox_shortcut_start_shift->isChecked() ) );
   start.append( boolToString( ui->checkBox_shortcut_start_alt->isChecked() ) );
   start.append( boolToString( ui->checkBox_shortcut_start_meta->isChecked() ) );
   start.append( ui->comboBox_shortcut_start->currentText() );

   QString pause;
   pause.append( boolToString( ui->checkBox_shortcut_pause_strg->isChecked() ) );
   pause.append( boolToString( ui->checkBox_shortcut_pause_shift->isChecked() ) );
   pause.append( boolToString( ui->checkBox_shortcut_pause_alt->isChecked() ) );
   pause.append( boolToString( ui->checkBox_shortcut_pause_meta->isChecked() ) );
   pause.append( ui->comboBox_shortcut_pause->currentText() );

   QStringList list;

   if ( start == pause )
       return true;
   else
       return false;
}

// Start
void QvkGlobalShortcut::slot_checkbox_shortcut_start_clicked( bool value )
{
    Q_UNUSED(value)

    if ( ( ui->checkBox_shortcut_start_alt->isChecked() | ui->checkBox_shortcut_start_meta->isChecked() | ui->checkBox_shortcut_start_shift->isChecked() | ui->checkBox_shortcut_start_strg->isChecked() ) and !isBusy() )
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/accept.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_start->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        shortcut_start();
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

void QvkGlobalShortcut::shortcut_start()
{
    QString shortcut;

    QList<QCheckBox *> listCheckBox = ui->frame_screencast_shortcut->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        if ( ( listCheckBox.at(i)->objectName().section( "_", 2, 2 ) == "start" ) and ( listCheckBox.at(i)->isChecked() ) )
        {
            shortcut.append( "+" );
            shortcut.append( listCheckBox.at(i)->objectName().section( "_", 3, 3 ).toUpper() );
            continue;
        }
    }

    if ( shortcut.startsWith( "+" ) == true )
    {
        shortcut.remove( 0, 1 );
    }

    shortcut.append( "+" + ui->comboBox_shortcut_start->currentText() );

    shortcutStart->unsetShortcut();
    shortcutStart->setShortcut( QKeySequence( shortcut ) );

    qDebug().noquote() << global::nameOutput << "Set global shortcut for Start:" << shortcut;
}


// Pause
void QvkGlobalShortcut::slot_checkbox_shortcut_pause_clicked( bool value )
{
    Q_UNUSED(value)

    if ( ( ui->checkBox_shortcut_pause_alt->isChecked() | ui->checkBox_shortcut_pause_meta->isChecked() | ui->checkBox_shortcut_pause_shift->isChecked() | ui->checkBox_shortcut_pause_strg->isChecked() ) and !isBusy() )
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/accept.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_pause->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        shortcut_pause();
    } else
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_pause->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        shortcutPause->unsetShortcut();
        qDebug().noquote() << global::nameOutput << "Set global shortcut for Stop: None";
    }
}

void QvkGlobalShortcut::slot_checkbox_shortcut_pause_currentIndexChanged( int value )
{
    Q_UNUSED(value)
    slot_checkbox_shortcut_pause_clicked( true );
}

void QvkGlobalShortcut::shortcut_pause()
{
    QString shortcut;

    QList<QCheckBox *> listCheckBox = ui->frame_screencast_shortcut->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        if ( ( listCheckBox.at(i)->objectName().section( "_", 2, 2 ) == "pause" ) and ( listCheckBox.at(i)->isChecked() ) )
        {
            shortcut.append( "+" );
            shortcut.append( listCheckBox.at(i)->objectName().section( "_", 3, 3 ).toUpper() );
            continue;
        }
    }

    if ( shortcut.startsWith( "+" ) == true )
    {
        shortcut.remove( 0, 1 );
    }

    shortcut.append( "+" + ui->comboBox_shortcut_pause->currentText() );

    shortcutPause->unsetShortcut();
    shortcutPause->setShortcut( QKeySequence( shortcut ) );

    qDebug().noquote() << global::nameOutput << "Set global shortcut for Pause:" << shortcut;
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
