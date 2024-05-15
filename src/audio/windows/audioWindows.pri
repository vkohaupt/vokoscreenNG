INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD
HEADERS     += $$PWD/QvkAudioController.h \
               $$PWD/WASAPI/QvkWASAPIController.h \
               $$PWD/WASAPI/QvkWASAPIGstreamer.h \
               $$PWD/WASAPI/QvkWASAPIWatcher.h
                   
SOURCES     += $$PWD/QvkAudioController.cpp \
               $$PWD/WASAPI/QvkWASAPIController.cpp \
               $$PWD/WASAPI/QvkWASAPIGstreamer.cpp \
               $$PWD/WASAPI/QvkWASAPIWatcher.cpp
