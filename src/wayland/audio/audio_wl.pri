INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD
HEADERS     += $$PWD/QvkAudioController_wl.h \
               $$PWD/QvkPulseAudioWatcher_wl.h \
               $$PWD/QvkPulseAudioServer_wl.h \
               $$PWD/QvkPulseAudioDevices_wl.h \
               $$PWD/QvkDevices_wl.h \
               $$PWD/QvkLevelMeter_wl.h \
               $$PWD/QvkLevelMeterController_wl.h
                   
SOURCES     += $$PWD/QvkAudioController_wl.cpp \
               $$PWD/QvkPulseAudioWatcher_wl.cpp \
               $$PWD/QvkPulseAudioServer_wl.cpp \
               $$PWD/QvkPulseAudioDevices_wl.cpp \
               $$PWD/QvkDevices_wl.c \
               $$PWD/QvkLevelMeter_wl.cpp \
               $$PWD/QvkLevelMeterController_wl.cpp
