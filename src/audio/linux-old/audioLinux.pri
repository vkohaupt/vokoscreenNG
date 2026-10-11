INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD
HEADERS     += $$PWD/QvkAudioController.h \
               $$PWD/QvkPulseAudioWatcher.h \
               $$PWD/QvkPulseAudioServer.h \
               $$PWD/QvkPulseAudioDevices.h \
               $$PWD/QvkDevices.h \
               $$PWD/QvkLevelMeter.h \
               $$PWD/QvkLevelMeterController.h
                   
SOURCES     += $$PWD/QvkAudioController.cpp \
               $$PWD/QvkPulseAudioWatcher.cpp \
               $$PWD/QvkPulseAudioServer.cpp \
               $$PWD/QvkPulseAudioDevices.cpp \
               $$PWD/QvkDevices.c \
               $$PWD/QvkLevelMeter.cpp \
               $$PWD/QvkLevelMeterController.cpp
