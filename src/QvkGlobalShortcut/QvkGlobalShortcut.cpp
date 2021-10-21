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

#include <QDebug>
#include <QCheckBox>

QvkGlobalShortcut::QvkGlobalShortcut(QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow )
{
    Q_UNUSED(mainWindow);
    ui = ui_mainwindow;

    connect( ui->checkBox_shortcut_OnOff, SIGNAL( clicked( bool ) ), ui->frame_screencast_shortcut, SLOT( setEnabled( bool ) ) );

    QList<QCheckBox *> listCheckBox = ui->frame_screencast_shortcut->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        listCheckBox.at(i)->installEventFilter( this );
        connect( listCheckBox.at(i), SIGNAL( clicked( bool ) ), this, SLOT( slot_checkboxClicked( bool ) ) );
    }

    QGlobalShortcut *shortcutWebcam = new QGlobalShortcut( this );
    connect( shortcutWebcam, SIGNAL( activated() ), ui->checkBoxCameraOnOff, SLOT( click() ) );
    shortcutWebcam->setShortcut( QKeySequence( "Ctrl+Shift+F8" ) );

    QGlobalShortcut *shortcutMagnifier = new QGlobalShortcut( this );
    connect( shortcutMagnifier, SIGNAL( activated() ), ui->checkBoxMagnifier, SLOT( click() ) );
    shortcutMagnifier->setShortcut( QKeySequence( "Ctrl+Shift+F9" ) );

    shortcutStart = new QGlobalShortcut( this );
    connect( shortcutStart, SIGNAL( activated() ), ui->pushButtonStart, SLOT( click() ) );
    shortcutStart->setShortcut( QKeySequence( "Ctrl+Shift+F10" ) );

    QGlobalShortcut *shortcutStop = new QGlobalShortcut( this );
    connect( shortcutStop, SIGNAL( activated() ), ui->pushButtonStop, SLOT( click() ) );
    shortcutStop->setShortcut( QKeySequence( "Ctrl+Shift+F11" ) );

    QGlobalShortcut *shortcutPauseContinue = new QGlobalShortcut( this );
    connect( shortcutPauseContinue, SIGNAL( activated() ), this, SLOT( slot_pauseContinue() ) );
    shortcutPauseContinue->setShortcut( QKeySequence( "Ctrl+Shift+F12" ) );

    connect( ui->checkBoxStartTime, SIGNAL( clicked( bool ) ), this, SLOT( slot_setOrUnsetShortcut( bool ) ) );
}


QvkGlobalShortcut::~QvkGlobalShortcut()
{
}


void QvkGlobalShortcut::slot_checkboxClicked( bool value )
{
    Q_UNUSED(value)
    QString shortcut;

    QList<QCheckBox *> listCheckBox = ui->frame_screencast_shortcut->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        if ( ( listCheckBox.at(i)->objectName().section( "_", 2, 2 ) == "start" ) and
             ( listCheckBox.at(i)->isChecked() ) and
             ( checkBox_shortcut_objectName_was_clicked.section("_", 2, 2 ) == "start" ) )
        {
            shortcut.append( "+" );
            shortcut.append( listCheckBox.at(i)->objectName().section( "_", 3, 3 ).toUpper() );
            continue;
        }

        if ( ( listCheckBox.at(i)->objectName().section( "_", 2, 2 ) == "stop" ) and
             ( listCheckBox.at(i)->isChecked() ) and
             ( checkBox_shortcut_objectName_was_clicked.section("_", 2, 2 ) == "stop" ) )
        {
            shortcut.append( "+" );
            shortcut.append( listCheckBox.at(i)->objectName().section( "_", 3, 3 ).toUpper() );
            continue;
        }

        if ( ( listCheckBox.at(i)->objectName().section( "_", 2, 2 ) == "pause" ) and
             ( listCheckBox.at(i)->isChecked() ) and
             ( checkBox_shortcut_objectName_was_clicked.section("_", 2, 2 ) == "pause" ) )
        {
            shortcut.append( "+" );
            shortcut.append( listCheckBox.at(i)->objectName().section( "_", 3, 3 ).toUpper() );
            continue;
        }

        if ( ( listCheckBox.at(i)->objectName().section( "_", 2, 2 ) == "continue" ) and
             ( listCheckBox.at(i)->isChecked() ) and
             ( checkBox_shortcut_objectName_was_clicked.section("_", 2, 2 ) == "continue" ) )
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

    if ( checkBox_shortcut_objectName_was_clicked.section("_", 2, 2 ) == "start" )
    {
        qDebug().noquote() << "Start" << shortcut;
        return;
    }

    if ( checkBox_shortcut_objectName_was_clicked.section("_", 2, 2 ) == "stop" )
    {
        qDebug().noquote() << "Stop" << shortcut;
        return;
    }

    if ( checkBox_shortcut_objectName_was_clicked.section("_", 2, 2 ) == "pause" )
    {
        qDebug().noquote() << "Pause" << shortcut;
        return;
    }

    if ( checkBox_shortcut_objectName_was_clicked.section("_", 2, 2 ) == "continue" )
    {
        qDebug().noquote() << "Continue" << shortcut;
        return;
    }

}


bool QvkGlobalShortcut::eventFilter(QObject *object, QEvent *event)
{
    QCheckBox *checkBox = qobject_cast<QCheckBox *>(object);
    checkBox_shortcut_objectName_was_clicked = checkBox->objectName();
    return QObject::eventFilter( object, event );
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
