INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

HEADERS += $$PWD/QvkCameraController.h \
           $$PWD/QvkCameraWatcher.h \
           $$PWD/QvkCameraWindow.h \
           $$PWD/QvkVideoSurface.h \
           $$PWD/QvkCameraResolution.h \
           $$PWD/QvkCameraSettingsDialog.h

SOURCES += $$PWD/QvkCameraController.cpp \
           $$PWD/QvkCameraWatcher.cpp \
           $$PWD/QvkCameraWindow.cpp \
           $$PWD/QvkCameraResolution.cpp \
           $$PWD/QvkCameraSettingsDialog.cpp

FORMS +=   $$PWD/camerasettingsdialog.ui
