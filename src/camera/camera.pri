INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

HEADERS += $$PWD/QvkCameraController.h \
           $$PWD/QvkCameraWatcher.h \
           $$PWD/QvkCameraWindow.h \
           $$PWD/QvkVideoSurface.h
           
SOURCES += $$PWD/QvkCameraController.cpp \
           $$PWD/QvkCameraWatcher.cpp \
           $$PWD/QvkCameraWindow.cpp


FORMS += \
    $$PWD/camerasettingsdialog.ui

HEADERS += \
    $$PWD/camerasettingsdialog.h

SOURCES += \
    $$PWD/camerasettingsdialog.cpp
