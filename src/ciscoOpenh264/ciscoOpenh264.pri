INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD
HEADERS     += $$PWD/QvkCiscoOpenh264Controller.h \
               $$PWD/QvkCiscoOpenh264Downloader.h \
               $$PWD/QvkBz2Decode.h \
               $$PWD/QvkCiscoFinishDialog.h \
               $$PWD/QvkCiscoWaitDialog.h
                   
SOURCES     += $$PWD/QvkCiscoOpenh264Controller.cpp \
               $$PWD/QvkCiscoOpenh264Downloader.cpp \
               $$PWD/QvkBz2Decode.cpp \
               $$PWD/QvkCiscoFinishDialog.cpp \
               $$PWD/QvkCiscoWaitDialog.cpp

FORMS       += $$PWD/QvkCiscoFinishDialog.ui \
               $$PWD/QvkCiscoWaitDialog.ui
