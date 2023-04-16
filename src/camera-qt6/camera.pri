INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

HEADERS += $$PWD/QvkCameraController.h \
           $$PWD/QvkCameraWatcher.h \
           $$PWD/QvkCameraWindow.h \
           $$PWD/QvkVideoSurface.h \
           $$PWD/QvkCameraResolution.h
           
SOURCES += $$PWD/QvkCameraController.cpp \
           $$PWD/QvkCameraWatcher.cpp \
           $$PWD/QvkCameraWindow.cpp \
           $$PWD/QvkCameraResolution.cpp


FORMS += \
    $$PWD/camerasettingsdialog.ui

HEADERS += \
    $$PWD/camerasettingsdialog.h

SOURCES += \
    $$PWD/camerasettingsdialog.cpp
