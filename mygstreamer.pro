#-------------------------------------------------
#
# Project created by QtCreator 2017-09-12T21:33:03
#
#-------------------------------------------------

unix: QT += x11extras
QT += core gui widgets network testlib dbus multimedia multimediawidgets

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
        mainwindow.ui \
        QvkNoPlayerDialog.ui
        
RESOURCES += screencast.qrc

# Clean target
QMAKE_CLEAN += $$TARGET */*~

CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-1.0

win32:INCLUDEPATH += C:\gstreamer\1.0\x86\include\gstreamer-1.0
win32:INCLUDEPATH += C:\gstreamer\1.0\x86\include\glib-2.0
win32:INCLUDEPATH += C:\gstreamer\1.0\x86\lib\glib-2.0\include

# region
include(region/regionselection.pri)

# winInfo
unix:include(winInfo/linux/winInfoLinux.pri)
win32:include(winInfo/windows/winInfoWindows.pri)

# libqxt
include(libqxtQt5/libqxt.pri)

# countdown
include(countdown/countdown.pri)

# webcam
include(webcam/webcam.pri)
