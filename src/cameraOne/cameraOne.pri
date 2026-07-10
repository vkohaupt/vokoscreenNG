INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

HEADERS += $$PWD/QvkCameraOneController.h \
           $$PWD/QvkCameraOneOptions.h \
           $$PWD/QvkCameraOneWatcher.h \
           $$PWD/QvkCameraOneSingle.h \
           $$PWD/QvkCameraOneWindow.h

SOURCES += $$PWD/QvkCameraOneController.cpp \
           $$PWD/QvkCameraOneOptions.cpp \
           $$PWD/QvkCameraOneWatcher.cpp \
           $$PWD/QvkCameraOneSingle.cpp \
           $$PWD/QvkCameraOneWindow.cpp

FORMS +=   $$PWD/QvkCameraOneWindow.ui \
           $$PWD/QvkCameraOneOptions.ui \
           $$PWD/QvkCameraOneSingle.ui
