/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2024 Volker Kohaupt
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

#ifndef QVKLEVELMETERCONTROLLER_H
#define QVKLEVELMETERCONTROLLER_H

#include <QObject>
#include <QCheckBox>
#include <QProgressBar>
#include <QHBoxLayout>

#include "QvkLevelMeter.h"

class QvkLevelMeterController : public QProgressBar
{
    Q_OBJECT

public:
    explicit QvkLevelMeterController();
    ~QvkLevelMeterController();
    void add_ProgressBar( QCheckBox *checkBox, QHBoxLayout *layout );
    void remove_ProgressBar(QCheckBox *checkBox);
    QvkLevelMeter *vkLevelMeter;


private:
    int maxSteps;

public slots:


private slots:
    void slot_textChanged( QString string );


protected:


signals:


};

#endif
