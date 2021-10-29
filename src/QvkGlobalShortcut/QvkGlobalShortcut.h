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

#ifndef QVKGLOBALSHORTCUT_H
#define QVKGLOBALSHORTCUT_H

#include "ui_formMainWindow.h"
#include "qglobalshortcut.h"
#include "QvkSpezialCheckbox.h"

#include <QObject>
#include <QMainWindow>
#include <QMouseEvent>
#include <QEvent>

class QvkGlobalShortcut: public QObject
{
    Q_OBJECT
    
public:
    QvkGlobalShortcut( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow );
    virtual ~QvkGlobalShortcut();


public slots:
    void slot_startStop();

    void slot_checkbox_shortcut_start_clicked( bool value );
    void slot_checkbox_shortcut_start_currentIndexChanged( int value );
    void slot_toolButton_shortcut_start_reset();

    void slot_checkbox_shortcut_pause_clicked( bool value );
    void slot_checkbox_shortcut_pause_currentIndexChanged( int value );
    void slot_toolButton_shortcut_pause_reset();

    void slot_checkbox_shortcut_magnification_clicked( bool value );
    void slot_checkbox_shortcut_magnification_currentIndexChanged( int value );
    void slot_toolButton_shortcut_magnification_reset();

    void slot_checkbox_shortcut_camera_clicked( bool value );
    void slot_checkbox_shortcut_camera_currentIndexChanged( int value );
    void slot_toolButton_shortcut_camera_reset();

    void slot_checkbox_shortcut_showclick_clicked( bool value );
    void slot_checkbox_shortcut_showclick_currentIndexChanged( int value );
    void slot_toolButton_shortcut_showclick_reset();

    void slot_checkbox_shortcut_halo_clicked( bool value );
    void slot_checkbox_shortcut_halo_currentIndexChanged( int value );
    void slot_toolButton_shortcut_halo_reset();

private:
    Ui_formMainWindow *ui;
    bool isBusy(QString check);
    QString boolToString( bool bo );

    QGlobalShortcut *shortcutStart;
    QGlobalShortcut *shortcutPause;
    QGlobalShortcut *shortcutMagnification;
    QGlobalShortcut *shortcutCamera;
    QGlobalShortcut *shortcutShowclick;
    QGlobalShortcut *shortcutHalo;

    QvkSpezialCheckbox *spezialCheckboxShowclick;
    QvkSpezialCheckbox *spezialCheckboxHalo;

private slots:
    void slot_pauseContinue();
    void slot_setOrUnsetShortcut( bool value );

    
protected:

    
signals:

    
};

#endif
