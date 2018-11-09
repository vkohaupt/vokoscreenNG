#-------------------------------------------------
#
# Project created by QtCreator 2017-09-12T21:33:03
#
#-------------------------------------------------

unix: QT += x11extras
QT += core gui widgets multimedia multimediawidgets network
# testlib // Achtung unter Windows öffnet sich damit die Konsole und qDebug() zeigt seine Ausgaben.

TARGET = vokoscreen
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

unix:CONFIG += link_pkgconfig
unix:PKGCONFIG += gstreamer-1.0

win32:GStreamerDir=$$(GSTREAMER_1_0_ROOT_X86)
win32:INCLUDEPATH += $${GStreamerDir}\include\gstreamer-1.0
win32:INCLUDEPATH += $${GStreamerDir}\include\glib-2.0
win32:INCLUDEPATH += $${GStreamerDir}\lib\glib-2.0\include

win32:LIBS += -LC:\gstreamer\1.0\x86\bin
win32:LIBS += -LC:\gstreamer\1.0\x86\lib\gstreamer-1.0
win32:LIBS += -llibgstreamer-1.0-0
win32:LIBS += -llibglib-2.0-0
win32:LIBS += -llibgobject-2.0-0

# settings
include(settings/settings.pri)

# region
include(region/regionselection.pri)

# winInfo
unix:LIBS += -lX11
include(winInfo/winInfo.pri)

# countdown
include(countdown/countdown.pri)

# webcam
include(camera/camera.pri)

# screenshot
include(screenshot/screenshot.pri)

# pulse
unix:include(audio/pulse/audioPulse.pri)

# alsa
unix:include(audio/alsa/audioAlsa.pri)

# audio windows
win32:include(audio/windows/audioWindows.pri)

# help
include(help/help.pri)

# systray
include(systray/systray.pri)

# information
include(information/information.pri)

# magnifier
include(magnifier/magnifier.pri)

# showclick
include(showclick/showclick.pri)
