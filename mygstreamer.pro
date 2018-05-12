#-------------------------------------------------
#
# Project created by QtCreator 2017-09-12T21:33:03
#
#-------------------------------------------------

QT += core gui widgets x11extras network testlib dbus multimedia multimediawidgets

TARGET = mygstreamer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-1.0

# region
include(region/regionselection.pri)

# winInfo
include(winInfo/winInfo.pri)

# libqxt
include(libqxtQt5/libqxt.pri)
