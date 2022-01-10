/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2021 Volker Kohaupt
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

#ifndef QVKCISCOOPENH264CONTROLLER_H
#define QVKCISCOOPENH264CONTROLLER_H

#include <QObject>

#include "ui_formMainWindow.h"
#include "QvkCiscoWaitDialog.h"

class QvkCiscoOpenh264Controller: public QObject
{
    Q_OBJECT

public:
    QvkCiscoOpenh264Controller( QString pathWithSettingsFilename , Ui_formMainWindow *ui_mainwindow );
    virtual ~QvkCiscoOpenh264Controller();
    void init();
    bool isShowCiscoFinishDialog = false;
    void showWaitDialog();


public slots:


private:
    Ui_formMainWindow *ui;
    QString libopenh264_filename;
    QString pathWithSettingsFilename;
    QvkCiscoWaitDialog *vkCiscoWaitDialog;
    QString downloadFile;


private slots:
    void slot_deCompress( QString string );
    void slot_showCiscoFinishDialog();
    void slot_pushButtonCiscoLicense();
    void slot_cisco_on( bool );
    void slot_cisco_off( bool );
    void slot_closeWaitDialog();


    void slot_downloadProgress( qint64 downloadedSize, qint64 fileSize );


protected:


signals:
    void signal_ciscofile_now_downloded( bool );

};

#endif
