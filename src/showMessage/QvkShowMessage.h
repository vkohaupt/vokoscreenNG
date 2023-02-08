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

#ifndef QVKSHOWMESSAGE_H 
#define QVKSHOWMESSAGE_H

#include <QWidget>
#include <QLabel>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>

class QvkShowMessage : public QWidget
{
    Q_OBJECT

public:
    QvkShowMessage();
    virtual ~QvkShowMessage();
    void showMessage( QString text );
    void set_URL( QString url );
    void set_Image( QImage image );
    void set_StatusIcon( QString statusIcon );
    void set_WindowIcon( QIcon icon );


public slots:


private slots:
    void slot_durationButton();

    
signals:


protected:  
    void closeEvent( QCloseEvent *event );
    void mouseReleaseEvent(QMouseEvent *event);


private:
    QHBoxLayout *hBoxLayoutWindow;
    QVBoxLayout *vBoxLayoutIcon;
    QVBoxLayout *vBoxLayoutTextImage;
    QLabel *labelIcon;
    QLabel *labelText;
    QLabel *labelImage;
    QScreen *myScreen;
    QLabel *labelImageDuration;
    int degree = +36;
    QTimer *timer;
    int secCounter = 0;
    QString path = "";
    QImage image;
    QString statusIcon = "";
};

#endif
