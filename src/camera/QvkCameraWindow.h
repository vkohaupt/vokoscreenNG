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

#ifndef QVKCAMERAWINDOW_H
#define QVKCAMERAWINDOW_H

#include "ui_formMainWindow.h"
#include "ui_camerasettingsdialog.h"

#include "camerasettingsdialog.h"
#include "QvkSpezialSlider.h"

#include <QLabel>
#include <QWidget>

class QvkCameraWindow : public QLabel
{
    Q_OBJECT

public:
    QvkCameraWindow( Ui_formMainWindow *ui_surface, QvkSpezialSlider *gui_sliderCameraWindowSize );
    virtual ~QvkCameraWindow();
    Ui_cameraSettingsDialog *uiCameraSettingsDialog;
    cameraSettingsDialog *vkCameraSettingsDialog;
    void showError();


private:
   Ui_formMainWindow *ui_formMainWindow;


public slots:


private slots:
   void slot_switchToFullscreen();


protected:
    void closeEvent(QCloseEvent *event);
    void mouseDoubleClickEvent( QMouseEvent *event );
    void keyPressEvent( QKeyEvent *event );

    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

signals:
    void signal_cameraWindow_close( bool );

};

#endif // QVKWEBCAMWINDOW_H
