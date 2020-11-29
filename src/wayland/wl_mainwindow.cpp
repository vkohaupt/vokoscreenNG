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

#include "ui_wl_formMainWindow.h"
#include "wl_mainwindow.h"
#include "global.h"

#include <QDebug>
#include <QMessageBox>


Qvk_wl_MainWindow::Qvk_wl_MainWindow(QWidget *parent) : QMainWindow(parent),
                                                        ui(new Ui::wl_formMainWindow)
{
    ui->setupUi(this);

    qDebug().noquote() << global::nameOutput << tr( "Desktop session is a Wayland session" );

    QMessageBox *messageBox = new QMessageBox();
    QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
    messageBox->setWindowIcon( icon );
    messageBox->setWindowTitle( global::name + " " + global::version );
    messageBox->setIcon( QMessageBox::Critical );
    messageBox->setText( tr( "Wayland desktop session detected" ) );
    messageBox->setInformativeText( tr( "To time Wayland is not supported. Please close vokoscreenNG, logout from your Desktop and start a X11 Desktop session" ) );
    messageBox->exec();

}

Qvk_wl_MainWindow::~Qvk_wl_MainWindow()
{}
