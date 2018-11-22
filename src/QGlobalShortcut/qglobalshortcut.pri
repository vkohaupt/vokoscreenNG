
INCLUDEPATH += $$PWD

CONFIG += c++11

linux {
    QT       += x11extras
    CONFIG   += link_pkgconfig
    PKGCONFIG += x11
}

win32-mingw{
    QMAKE_CXXFLAGS += -Wliteral-suffix
}
win32-msvc*{
    LIBS += -luser32
}

HEADERS += \
    $$PWD/qglobalshortcut.h

win32: SOURCES += $$PWD/win/qglobalshortcut.cpp
linux: SOURCES += $$PWD/x11/qglobalshortcut.cpp


