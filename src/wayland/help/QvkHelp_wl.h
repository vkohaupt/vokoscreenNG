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

#ifndef QVKHELP_WL_H
#define QVKHELP_WL_H

// #include "ui_formMainWindow.h"
#include "ui_formMainWindow_wl.h"

#include "ui_help_wl.h"
#include "QvkDownloader_wl.h"
#include "QvkLocale_wl.h"

#include <QObject>
#include <QTemporaryDir>
#include <QMouseEvent>
#include <QEvent>
#include <QStringList>
#include <QEvent>

class QvkHelp_wl: public QWidget
{
    Q_OBJECT
    
public:
    QvkHelp_wl( Ui_formMainWindow_wl *ui_mainwindow );
    virtual ~QvkHelp_wl() override;
    Ui_help_wl *uiHelp;
    QvkLocale_wl *vkLocale;


public slots:
    void slot_cleanUp();

  
private:
    Ui_formMainWindow_wl *ui;

    QStringList toDownloadFiles = { ".png", ".svg", ".css", ".jpg" };
    bool isFileInLine( QString line );

    QvkDownloader_wl *vkDownloadHTML;
    QvkDownloader_wl *vkDownloadFiles;

    void loadHTML( QString value );
    QString remotePath;
    QString remoteBasename;
    int getCountFileToDownload( QString tempPathFileName );
    QStringList localFiles;
    QString tempPath;
    QString vk_helpPath;

    QTemporaryDir temporaryDirLocal;


private slots:
    void slot_parseHTML( QString tempPathFileName );
    void slot_showHelp( QString tempPathFileName );
    void slot_parse_locale( QStringList list );


protected:
    bool eventFilter(QObject *object, QEvent *ev) override;


signals:

    
};

#endif
