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

#ifndef QVKAUDIOCONTROLLER_H
#define QVKAUDIOCONTROLLER_H

#include "ui_formMainWindow.h"
#include "WASAPI/QvkWASAPIController.h"
#include "DirectSound/QvkDirectSoundController.h"

#include <QObject>

class QvkAudioController: public QObject
{
    Q_OBJECT


public:
    QvkAudioController( Ui_formMainWindow *ui_mainwindow );
    virtual ~QvkAudioController();
    Ui_formMainWindow *ui;
    QRadioButton *radioButtonWASAPI;
    QRadioButton *radioButtonDirectSound;
    QvkWASAPIController *vkWASAPIController;
    QvkDirectSoundController *vkDirectSoundController;


public slots:


private:


private slots:
    void slot_WASAPI( bool value );
    void slot_DirectSound( bool value );


protected:


signals:


};

#endif
