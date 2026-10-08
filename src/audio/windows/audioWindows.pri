INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD
HEADERS     += $$PWD/QvkAudioWindowsController.h \
               $$PWD/QvkAudioWindowsSingle.h \
               $$PWD/QvkAudioWindowsWatcher.h \
               $$PWD/QvkAudioWindowsLevelMeter.h

SOURCES     += $$PWD/QvkAudioWindowsController.cpp \
               $$PWD/QvkAudioWindowsSingle.cpp \
               $$PWD/QvkAudioWindowsWatcher.cpp \
               $$PWD/QvkAudioWindowsLevelMeter.cpp

FORMS       += $$PWD/QvkAudioWindowsSingle.ui
