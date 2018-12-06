INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

unix:HEADERS += $$PWD/QvkCameraController.h \
                $$PWD/QvkCameraWatcherLinux.h \
                $$PWD/QvkCameraWindow.h \
                $$PWD/QvkVideoSurface.h \
                $$PWD/QvkWidgetExit.h \
                $$PWD/QvkWidgetFrame.h \
                $$PWD/QvkWidgetHelp.h \
                $$PWD/QvkWidgetMenueBackground.h \
                $$PWD/QvkWidgetSettings.h
           
unix:SOURCES += $$PWD/QvkCameraController.cpp \
                $$PWD/QvkCameraWatcherLinux.cpp \
                $$PWD/QvkCameraWindow.cpp \
                $$PWD/QvkWidgetExit.cpp \
                $$PWD/QvkWidgetFrame.cpp \
                $$PWD/QvkWidgetHelp.cpp \
                $$PWD/QvkWidgetMenueBackground.cpp \
                $$PWD/QvkWidgetSettings.cpp

win32:HEADERS += $$PWD/QvkCameraController.h \
                 $$PWD/QvkCameraWatcherWindows.h \
                 $$PWD/QvkCameraWindow.h \
                 $$PWD/QvkVideoSurface.h \
                 $$PWD/QvkWidgetExit.h \
                 $$PWD/QvkWidgetFrame.h \
                 $$PWD/QvkWidgetHelp.h \
                 $$PWD/QvkWidgetMenueBackground.h \
                 $$PWD/QvkWidgetSettings.h

win32:SOURCES += $$PWD/QvkCameraController.cpp \
                 $$PWD/QvkCameraWatcherWindows.cpp \
                 $$PWD/QvkCameraWindow.cpp \
                 $$PWD/QvkWidgetExit.cpp \
                 $$PWD/QvkWidgetFrame.cpp \
                 $$PWD/QvkWidgetHelp.cpp \
                 $$PWD/QvkWidgetMenueBackground.cpp \
                 $$PWD/QvkWidgetSettings.cpp
