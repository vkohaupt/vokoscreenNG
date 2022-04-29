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
    connect( ui->toolButton_shortcut_reset_start, SIGNAL( clicked() ), this, SLOT( slot_toolButton_shortcut_start_reset() ) );

    shortcutPause = new QGlobalShortcut( this );
    connect( shortcutPause, SIGNAL( activated() ), this, SLOT( slot_pauseContinue() ) );
    connect( ui->checkBox_shortcut_pause_strg,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_pause_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_pause_shift, SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_pause_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_pause_alt,   SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_pause_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_pause_meta,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_pause_clicked( bool ) ) );
    connect( ui->comboBox_shortcut_pause, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_checkbox_shortcut_pause_currentIndexChanged( int ) ) );
    connect( ui->toolButton_shortcut_reset_pause, SIGNAL( clicked() ), this, SLOT( slot_toolButton_shortcut_pause_reset() ) );

    shortcutMagnification = new QGlobalShortcut( this );
    connect( shortcutMagnification, SIGNAL( activated() ), ui->checkBoxMagnifier, SLOT( click() ) );
    connect( ui->checkBox_shortcut_magnification_strg,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_magnification_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_magnification_shift, SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_magnification_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_magnification_alt,   SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_magnification_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_magnification_meta,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_magnification_clicked( bool ) ) );
    connect( ui->comboBox_shortcut_magnification, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_checkbox_shortcut_magnification_currentIndexChanged( int ) ) );
    connect( ui->toolButton_shortcut_reset_magnification, SIGNAL( clicked() ), this, SLOT( slot_toolButton_shortcut_magnification_reset() ) );

    shortcutCamera = new QGlobalShortcut( this );
    connect( shortcutCamera, SIGNAL( activated() ), ui->checkBoxCameraOnOff, SLOT( click() ) );
    connect( ui->checkBox_shortcut_camera_strg,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_camera_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_camera_shift, SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_camera_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_camera_alt,   SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_camera_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_camera_meta,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_camera_clicked( bool ) ) );
    connect( ui->comboBox_shortcut_camera, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_checkbox_shortcut_camera_currentIndexChanged( int ) ) );
    connect( ui->toolButton_shortcut_reset_camera, SIGNAL( clicked() ), this, SLOT( slot_toolButton_shortcut_camera_reset() ) );

    QList<QvkSpezialCheckbox *> listSpezialCheckboxShowclick = ui->centralWidget->findChildren<QvkSpezialCheckbox *>();
    for ( int i = 0; i < listSpezialCheckboxShowclick.count(); i++ ){
        if ( listSpezialCheckboxShowclick.at(i)->objectName() == "spezialCheckboxShowclick" ){
            spezialCheckboxShowclick = listSpezialCheckboxShowclick.at(i);
        }
    }
    shortcutShowclick = new QGlobalShortcut( this );
    connect( shortcutShowclick, SIGNAL( activated() ), spezialCheckboxShowclick, SLOT( slot_click() ) );
    connect( ui->checkBox_shortcut_showclick_strg,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_showclick_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_showclick_shift, SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_showclick_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_showclick_alt,   SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_showclick_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_showclick_meta,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_showclick_clicked( bool ) ) );
    connect( ui->comboBox_shortcut_showclick, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_checkbox_shortcut_showclick_currentIndexChanged( int ) ) );
    connect( ui->toolButton_shortcut_reset_showclick, SIGNAL( clicked() ), this, SLOT( slot_toolButton_shortcut_showclick_reset() ) );

    QList<QvkSpezialCheckbox *> listSpezialCheckboxHalo = ui->centralWidget->findChildren<QvkSpezialCheckbox *>();
    for ( int i = 0; i < listSpezialCheckboxHalo.count(); i++ ){
        if ( listSpezialCheckboxHalo.at(i)->objectName() == "spezialCheckboxHalo" ){
            spezialCheckboxHalo = listSpezialCheckboxHalo.at(i);
        }
    }
    shortcutHalo = new QGlobalShortcut( this );
    connect( shortcutHalo, SIGNAL( activated() ), spezialCheckboxHalo, SLOT( slot_click() ) );
    connect( ui->checkBox_shortcut_halo_strg,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_halo_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_halo_shift, SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_halo_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_halo_alt,   SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_halo_clicked( bool ) ) );
    connect( ui->checkBox_shortcut_halo_meta,  SIGNAL( clicked( bool ) ), this, SLOT( slot_checkbox_shortcut_halo_clicked( bool ) ) );
    connect( ui->comboBox_shortcut_halo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_checkbox_shortcut_halo_currentIndexChanged( int ) ) );
    connect( ui->toolButton_shortcut_reset_halo, SIGNAL( clicked() ), this, SLOT( slot_toolButton_shortcut_halo_reset() ) );

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
   QString camera;
   QString showclick;
   QString halo;

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

   camera.append( boolToString( ui->checkBox_shortcut_camera_strg->isChecked() ) );
   camera.append( boolToString( ui->checkBox_shortcut_camera_shift->isChecked() ) );
   camera.append( boolToString( ui->checkBox_shortcut_camera_alt->isChecked() ) );
   camera.append( boolToString( ui->checkBox_shortcut_camera_meta->isChecked() ) );
   camera.append( ui->comboBox_shortcut_camera->currentText() );

   showclick.append( boolToString( ui->checkBox_shortcut_showclick_strg->isChecked() ) );
   showclick.append( boolToString( ui->checkBox_shortcut_showclick_shift->isChecked() ) );
   showclick.append( boolToString( ui->checkBox_shortcut_showclick_alt->isChecked() ) );
   showclick.append( boolToString( ui->checkBox_shortcut_showclick_meta->isChecked() ) );
   showclick.append( ui->comboBox_shortcut_showclick->currentText() );

   halo.append( boolToString( ui->checkBox_shortcut_halo_strg->isChecked() ) );
   halo.append( boolToString( ui->checkBox_shortcut_halo_shift->isChecked() ) );
   halo.append( boolToString( ui->checkBox_shortcut_halo_alt->isChecked() ) );
   halo.append( boolToString( ui->checkBox_shortcut_halo_meta->isChecked() ) );
   halo.append( ui->comboBox_shortcut_halo->currentText() );

   if ( check == "start" )
   {
       if ( ( start == pause ) or ( start == magnification ) or ( start == camera ) or ( start == showclick ) or ( start == halo ) )
         return true;
   }

   if ( check == "pause" )
   {
       if ( ( pause == start ) or ( pause == magnification) or ( pause == camera ) or ( pause == showclick ) or ( pause == halo ) )
         return true;
   }

   if ( check == "magnification" )
   {
       if ( ( magnification == start ) or ( magnification == pause ) or ( magnification == camera ) or ( magnification == showclick ) or ( magnification == halo ) )
         return true;
   }

   if ( check == "camera" )
   {
       if ( ( camera == start ) or ( camera == pause ) or ( camera == magnification ) or ( camera == showclick ) or ( camera == halo ) )
         return true;
   }

   if ( check == "showclick" )
   {
       if ( ( showclick == start ) or ( showclick == pause ) or ( showclick == magnification ) or ( showclick == camera ) or ( showclick == halo ) )
         return true;
   }

   if ( check == "halo" )
   {
       if ( ( halo == start ) or ( halo == pause ) or ( halo == magnification ) or ( halo == camera ) or ( halo == showclick ) )
         return true;
   }

   return false;
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
            shortcut.append( "+CTRL" );
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

        ui->pushButtonStart->setToolTip( shortcut );
        ui->pushButtonStop->setToolTip( shortcut );

        emit signal_shortcutSystray( "Start", shortcut );
        emit signal_shortcutSystray( "Stop", shortcut );

        qDebug().noquote() << global::nameOutput << "Set global shortcut for Start:" << shortcut;
    } else
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_start->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        shortcutStart->unsetShortcut();
        ui->pushButtonStart->setToolTip( "None" );
        ui->pushButtonStop->setToolTip( "None" );

        emit signal_shortcutSystray( "Start", "None" );
        emit signal_shortcutSystray( "Stop", "None" );

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
            shortcut.append( "+CTRL" );
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

        ui->pushButtonPause->setToolTip( shortcut );
        ui->pushButtonContinue->setToolTip( shortcut );

        emit signal_shortcutSystray( "Pause", shortcut );
        emit signal_shortcutSystray( "Continue", shortcut );

        qDebug().noquote() << global::nameOutput << "Set global shortcut for Pause:" << shortcut;
    } else
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_pause->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        shortcutPause->unsetShortcut();
        ui->pushButtonPause->setToolTip( "None" );
        ui->pushButtonContinue->setToolTip( "None" );

        emit signal_shortcutSystray( "Pause", "None" );
        emit signal_shortcutSystray( "Continue", "None" );

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
            shortcut.append( "+CTRL" );
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

        ui->checkBoxMagnifier->setToolTip( shortcut );

        qDebug().noquote() << global::nameOutput << "Set global shortcut for Magnification:" << shortcut;
    } else
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_magnification->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        shortcutMagnification->unsetShortcut();
        ui->checkBoxMagnifier->setToolTip( "None" );
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
            shortcut.append( "+CTRL" );
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

        ui->checkBoxCameraOnOff->setToolTip( shortcut );

        qDebug().noquote() << global::nameOutput << "Set global shortcut for Camera:" << shortcut;
    } else
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_camera->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        shortcutCamera->unsetShortcut();
        ui->checkBoxCameraOnOff->setToolTip( "None" );
        qDebug().noquote() << global::nameOutput << "Set global shortcut for Camera: None";
    }
}

void QvkGlobalShortcut::slot_checkbox_shortcut_camera_currentIndexChanged( int value )
{
    Q_UNUSED(value)
    slot_checkbox_shortcut_camera_clicked( true );
}


// Showclick
void QvkGlobalShortcut::slot_checkbox_shortcut_showclick_clicked( bool value )
{
    Q_UNUSED(value)

    if ( ( ui->checkBox_shortcut_showclick_strg->isChecked() | ui->checkBox_shortcut_showclick_shift->isChecked() | ui->checkBox_shortcut_showclick_alt->isChecked() | ui->checkBox_shortcut_showclick_meta->isChecked()  ) and !isBusy( "showclick" ) )
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/accept.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_showclick->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        QString shortcut;
        if ( ui->checkBox_shortcut_showclick_strg->isChecked() ) {
            shortcut.append( "+CTRL" );
        }
        if ( ui->checkBox_shortcut_showclick_shift->isChecked() ) {
            shortcut.append( "+SHIFT" );
        }
        if ( ui->checkBox_shortcut_showclick_alt->isChecked() ) {
            shortcut.append( "+ALT" );
        }
        if ( ui->checkBox_shortcut_showclick_meta->isChecked() ) {
            shortcut.append( "+META" );
        }

        shortcut.append( "+" + ui->comboBox_shortcut_showclick->currentText() );

        if ( shortcut.startsWith( "+" ) == true ) {
            shortcut.remove( 0, 1 );
        }

        shortcutShowclick->unsetShortcut();
        shortcutShowclick->setShortcut( QKeySequence( shortcut ) );

        spezialCheckboxShowclick->setToolTip( shortcut );

        qDebug().noquote() << global::nameOutput << "Set global shortcut for Showclick:" << shortcut;
    } else
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_showclick->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        shortcutShowclick->unsetShortcut();
        spezialCheckboxShowclick->setToolTip( "None" );
        qDebug().noquote() << global::nameOutput << "Set global shortcut for Showclick: None";
    }
}

void QvkGlobalShortcut::slot_checkbox_shortcut_showclick_currentIndexChanged( int value )
{
    Q_UNUSED(value)
    slot_checkbox_shortcut_showclick_clicked( true );
}


// Halo
void QvkGlobalShortcut::slot_checkbox_shortcut_halo_clicked( bool value )
{
    Q_UNUSED(value)

    if ( ( ui->checkBox_shortcut_halo_strg->isChecked() | ui->checkBox_shortcut_halo_shift->isChecked() | ui->checkBox_shortcut_halo_alt->isChecked() | ui->checkBox_shortcut_halo_meta->isChecked()  ) and !isBusy( "halo" ) )
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/accept.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_halo->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        QString shortcut;
        if ( ui->checkBox_shortcut_halo_strg->isChecked() ) {
            shortcut.append( "+CTRL" );
        }
        if ( ui->checkBox_shortcut_halo_shift->isChecked() ) {
            shortcut.append( "+SHIFT" );
        }
        if ( ui->checkBox_shortcut_halo_alt->isChecked() ) {
            shortcut.append( "+ALT" );
        }
        if ( ui->checkBox_shortcut_halo_meta->isChecked() ) {
            shortcut.append( "+META" );
        }

        shortcut.append( "+" + ui->comboBox_shortcut_halo->currentText() );

        if ( shortcut.startsWith( "+" ) == true ) {
            shortcut.remove( 0, 1 );
        }

        shortcutHalo->unsetShortcut();
        shortcutHalo->setShortcut( QKeySequence( shortcut ) );

        spezialCheckboxHalo->setToolTip( shortcut );

        qDebug().noquote() << global::nameOutput << "Set global shortcut for Halo:" << shortcut;
    } else
    {
        QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
        QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        ui->label_shortcut_picture_halo->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

        shortcutHalo->unsetShortcut();
        spezialCheckboxHalo->setToolTip( "None" );
        qDebug().noquote() << global::nameOutput << "Set global shortcut for Halo: None";
    }
}

void QvkGlobalShortcut::slot_checkbox_shortcut_halo_currentIndexChanged( int value )
{
    Q_UNUSED(value)
    slot_checkbox_shortcut_halo_clicked( true );
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


void QvkGlobalShortcut::slot_toolButton_shortcut_start_reset()
{
    if ( ui->checkBox_shortcut_start_strg->isChecked() == false )
        ui->checkBox_shortcut_start_strg->click();

    if ( ui->checkBox_shortcut_start_shift->isChecked() == false )
        ui->checkBox_shortcut_start_shift->click();

    if ( ui->checkBox_shortcut_start_alt->isChecked() == true )
        ui->checkBox_shortcut_start_alt->click();

    if ( ui->checkBox_shortcut_start_meta->isChecked() == true )
        ui->checkBox_shortcut_start_meta->click();

    ui->comboBox_shortcut_start->setCurrentText( "F10" );
}


void QvkGlobalShortcut::slot_toolButton_shortcut_pause_reset()
{
    if ( ui->checkBox_shortcut_pause_strg->isChecked() == false )
        ui->checkBox_shortcut_pause_strg->click();

    if ( ui->checkBox_shortcut_pause_shift->isChecked() == false )
        ui->checkBox_shortcut_pause_shift->click();

    if ( ui->checkBox_shortcut_pause_alt->isChecked() == true )
        ui->checkBox_shortcut_pause_alt->click();

    if ( ui->checkBox_shortcut_pause_meta->isChecked() == true )
        ui->checkBox_shortcut_pause_meta->click();

    ui->comboBox_shortcut_pause->setCurrentText( "F12" );
}


void QvkGlobalShortcut::slot_toolButton_shortcut_magnification_reset()
{
    if ( ui->checkBox_shortcut_magnification_strg->isChecked() == false )
        ui->checkBox_shortcut_magnification_strg->click();

    if ( ui->checkBox_shortcut_magnification_shift->isChecked() == false )
        ui->checkBox_shortcut_magnification_shift->click();

    if ( ui->checkBox_shortcut_magnification_alt->isChecked() == true )
        ui->checkBox_shortcut_magnification_alt->click();

    if ( ui->checkBox_shortcut_magnification_meta->isChecked() == true )
        ui->checkBox_shortcut_magnification_meta->click();

    ui->comboBox_shortcut_magnification->setCurrentText( "F9" );
}


void QvkGlobalShortcut::slot_toolButton_shortcut_camera_reset()
{
    if ( ui->checkBox_shortcut_camera_strg->isChecked() == false )
        ui->checkBox_shortcut_camera_strg->click();

    if ( ui->checkBox_shortcut_camera_shift->isChecked() == false )
        ui->checkBox_shortcut_camera_shift->click();

    if ( ui->checkBox_shortcut_camera_alt->isChecked() == true )
        ui->checkBox_shortcut_camera_alt->click();

    if ( ui->checkBox_shortcut_camera_meta->isChecked() == true )
        ui->checkBox_shortcut_camera_meta->click();

    ui->comboBox_shortcut_camera->setCurrentText( "F8" );
}


void QvkGlobalShortcut::slot_toolButton_shortcut_showclick_reset()
{
    if ( ui->checkBox_shortcut_showclick_strg->isChecked() == false )
        ui->checkBox_shortcut_showclick_strg->click();

    if ( ui->checkBox_shortcut_showclick_shift->isChecked() == false )
        ui->checkBox_shortcut_showclick_shift->click();

    if ( ui->checkBox_shortcut_showclick_alt->isChecked() == true )
        ui->checkBox_shortcut_showclick_alt->click();

    if ( ui->checkBox_shortcut_showclick_meta->isChecked() == true )
        ui->checkBox_shortcut_showclick_meta->click();

    ui->comboBox_shortcut_showclick->setCurrentText( "F7" );
}


void QvkGlobalShortcut::slot_toolButton_shortcut_halo_reset()
{
    if ( ui->checkBox_shortcut_halo_strg->isChecked() == false )
        ui->checkBox_shortcut_halo_strg->click();

    if ( ui->checkBox_shortcut_halo_shift->isChecked() == false )
        ui->checkBox_shortcut_halo_shift->click();

    if ( ui->checkBox_shortcut_halo_alt->isChecked() == true )
        ui->checkBox_shortcut_halo_alt->click();

    if ( ui->checkBox_shortcut_halo_meta->isChecked() == true )
        ui->checkBox_shortcut_halo_meta->click();

    ui->comboBox_shortcut_halo->setCurrentText( "F6" );
}
