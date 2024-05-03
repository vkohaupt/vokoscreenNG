// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "audiosource.h"

#include <QApplication>

int main(int argv, char **args)
{
    QApplication app(argv, args);
    QCoreApplication::setApplicationName("Audio Source Test");

    InputTest input;
    input.show();

    return QCoreApplication::exec();
}
