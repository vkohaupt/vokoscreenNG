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

#ifndef QVKPADSANDCAPS_H
#define QVKPADSANDCAPS_H

#include "glib.h"
#include "gst/gst.h"

#include <QWidget>

class QvkPadsAndCaps : public QWidget
{
    Q_OBJECT

public:
    explicit QvkPadsAndCaps();
    ~QvkPadsAndCaps();
    static gboolean print_field( GQuark field, const GValue *value, gpointer pfx );
    void pad_profile();


private:


public slots:


private slots:


protected:


signals:
    void signal_openh264encProfile( QStringList list );

};

#endif
