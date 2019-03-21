INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

unix:HEADERS += $$PWD/QvkCameraController.h \
                $$PWD/QvkCameraWatcherLinux.h \
                $$PWD/QvkCameraWindow.h \
                $$PWD/QvkVideoSurface.h
           
unix:SOURCES += $$PWD/QvkCameraController.cpp \
                $$PWD/QvkCameraWatcherLinux.cpp \
                $$PWD/QvkCameraWindow.cpp

win32:HEADERS += $$PWD/QvkCameraController.h \
                 $$PWD/QvkCameraWatcherWindows.h \
                 $$PWD/QvkCameraWindow.h \
                 $$PWD/QvkVideoSurface.h

win32:SOURCES += $$PWD/QvkCameraController.cpp \
                 $$PWD/QvkCameraWatcherWindows.cpp \
                 $$PWD/QvkCameraWindow.cpp
