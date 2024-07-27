#-------------------------------------------------
#
# Project created by QtCreator 2017-09-12T21:33:03
#
#-------------------------------------------------

equals(QT_MAJOR_VERSION, 6):!lessThan(QT_MINOR_VERSION, 4) {
   # there is nothing to do
} else {
    message("Cannot use Qt $${QT_VERSION}")
    error( "Need minimum Qt 6.6" )
exit
}

unix: QT += dbus
QT += core gui widgets multimedia network
# testlib // Achtung unter Windows öffnet sich damit die Konsole und qDebug() zeigt seine Ausgaben.

TARGET = vokoscreenNG
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp \
           mainwindow.cpp \
           global.cpp

HEADERS += mainwindow.h \
           global.h

FORMS +=   formMainWindow.ui

RESOURCES += screencast.qrc

CONFIG += c++17

#isEmpty(QMAKE_LRELEASE) {
  # Try invocation path of qmake for lrelease
  # NOTE: Usually from Qt Unified Installer
# win32: QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
#    else: QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    
  # As a last resort try to use lrelease from PATH
  # NOTE: Usually from a distro package
#  unix:!exists($$QMAKE_LRELEASE) {
#    QMAKE_LRELEASE = lrelease-qt5
#  }
#}
#system($$QMAKE_LRELEASE language/*.ts)

unix:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
unix:message($$QMAKE_LRELEASE)
unix:system($$QMAKE_LRELEASE translations.pri)

win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease.exe
win32:message($$QMAKE_LRELEASE)
win32:system($$QMAKE_LRELEASE translations.pri)

# Clean target
QMAKE_CLEAN += $$TARGET */*~

unix:QMAKE_LFLAGS += -Wl,--as-needed

unix:INSTALLER=$$(VOKOSCREENNG_INSTALLER)
unix:contains(INSTALLER, "IFW" ){
  # -rpath,\'\$$ORIGIN/lib\' is for the IFW linux installer
  QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN/lib\''
  message($$INSTALLER)
}

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

# settings
include(settings/settings.pri)

# region
include(region/regionselection.pri)

# winInfo
include(winInfo/winInfo.pri)

# countdown
include(countdown/countdown.pri)

# camera
include(camera/camera.pri)

# audio linux
unix:include(audio/linux/audioLinux.pri)

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

# log
include(log/log.pri)

# QGlobalshortcut
include(QGlobalShortcut/qglobalshortcut.pri)
include(QvkGlobalShortcut/QvkGlobalShortcut.pri)

# GlobalMouse
include(globalMouse/globalMouse.pri)

# player
win32:include(playerGST/windows/playerWindows.pri)
unix:include(playerGST/linux/playerLinux.pri)

# screenManager
unix:include(screenManager/screenManager.pri)
win32:include(screenManagerWindows/screenManagerWindows.pri)

# licenses
include(licenses/licenses.pri)

# limitDiskFreeSpace
include(limitDiskFreeSpace/limitDiskFreeSpace.pri)

# spezialSlider
include(spezialSlider/spezialSlider.pri)

# version
include(version/version.pri)

# systrayAlternative
include(systrayAlternative/systrayAlternative.pri)

# ciscoOpenh264
win32:include(ciscoOpenh264/ciscoOpenh264.pri)

# wayland
unix:include(wayland/wayland.pri)
unix:include(waylandRoutines/waylandRoutines.pri)

# showClick
include(showClick/showClick.pri)

# halo
include(halo/halo.pri)

# spezialCheckBox
include(spezialCheckBox/spezialCheckBox.pri)

# ImageFromTabs
include(imageFromTabs/imageFromTabs.pri)

# Folder Dialog
include(dirDialog/dirDialog.pri)

# File Dialog
include(fileDialog/fileDialog.pri)

# Virtual Maschine
include(virtual/virtual.pri)

# Snapshot
include(snapshot/snapshot.pri)

# Composite
unix:include(composite/composite.pri)

# ShowMessage
include(showMessage/showMessage.pri)

# padsAndCaps
include(padsAndCaps/padsAndCaps.pri)

# Container
include(container/container.pri)

# spezialProgressBarAudio
include(spezialProgressBarAudio/spezialProgressBarAudio.pri)
