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

#ifndef QVKCAMERARESOLUTION_H
#define QVKCAMERARESOLUTION_H

#include "ui_formMainWindow.h"
#include "camerasettingsdialog.h"
#include "ui_camerasettingsdialog.h"

#include <QCamera>
#include <QComboBox>

class QvkCameraResolution : public QObject
{
    Q_OBJECT

public:
   QvkCameraResolution( Ui_formMainWindow *ui, cameraSettingsDialog *settingsDialog );
   virtual ~QvkCameraResolution();
   cameraSettingsDialog *vkCameraSettingsDialog;


public slots:
   void slot_resolution( int value );


private slots:
   void slot_statusChanged( QCamera::Status status );
   void slot_stateChanged( QCamera::State state );


private:
  Ui_formMainWindow *ui_formMainWindow;
  QCamera *camera = nullptr;


protected:


signals:

};

#endif
