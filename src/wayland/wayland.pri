INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD
HEADERS     += $$PWD/mainWindow_wl.h \
               $$PWD/portal_wl.h
  
SOURCES     += $$PWD/mainWindow_wl.cpp \
               $$PWD/portal_wl.cpp

FORMS       += $$PWD/formMainWindow_wl.ui

# Area
include(region/regionselection_wl.pri)

# container
#include(container/container_wl.pri)
