INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD
HEADERS     += $$PWD/QvkAudioPipewireController.h \
               $$PWD/QvkAudioPipewireSingle.h \
               $$PWD/QvkAudioPipewireWatcher.h \
               $$PWD/QvkAudioPipewireLevelMeter.h
                   
SOURCES     += $$PWD/QvkAudioPipewireController.cpp \
               $$PWD/QvkAudioPipewireSingle.cpp \
               $$PWD/QvkAudioPipewireWatcher.cpp \
               $$PWD/QvkAudioPipewireLevelMeter.cpp

FORMS       += $$PWD/QvkAudioPipewireSingle.ui
