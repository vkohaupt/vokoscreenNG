QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    formMainWindow.ui

unix:CONFIG += link_pkgconfig
unix:PKGCONFIG += gstreamer-1.0
unix:PKGCONFIG += libpulse
unix:PKGCONFIG += wayland-client
unix:LIBS += -lpulse-simple
unix:LIBS += -lX11

# Player
unix:PKGCONFIG += gstreamer-video-1.0
unix:PKGCONFIG += gstreamer-pbutils-1.0
unix:PKGCONFIG += glib-2.0

win32:contains(QMAKE_HOST.arch, x86_64) {
message("[vokoscreenNG] Create x86_64 build")
RC_ICONS = vokoscreenNG.ico
GStreamerDir=$$(GSTREAMER_1_0_ROOT_MSVC_X86_64)
INCLUDEPATH += $${GStreamerDir}\include\gstreamer-1.0
INCLUDEPATH += $${GStreamerDir}\include\glib-2.0
INCLUDEPATH += $${GStreamerDir}\lib\glib-2.0\include
INCLUDEPATH += $${GStreamerDir}\include
LIBS += -L$${GStreamerDir}\bin
LIBS += -L$${GStreamerDir}\lib\gstreamer-1.0
LIBS += $${GStreamerDir}\lib\libgstreamer-1.0.dll.a
LIBS += $${GStreamerDir}\lib\libgstvideo-1.0.dll.a
LIBS += $${GStreamerDir}\lib\libgstpbutils-1.0.dll.a
LIBS += $${GStreamerDir}\lib\libglib-2.0.dll.a
LIBS += $${GStreamerDir}\lib\libgobject-2.0.dll.a
LIBS += $${GStreamerDir}\lib\libbz2.dll.a
}

# Volumeter
include(levelMeter/levelMeter.pri)
