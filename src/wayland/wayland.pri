INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD
HEADERS     += $$PWD/mainWindow_wl.h \
               $$PWD/portal_wl.h
  
SOURCES     += $$PWD/mainWindow_wl.cpp \
               $$PWD/portal_wl.cpp

FORMS       += $$PWD/formMainWindow_wl.ui

# Area
include(region/regionselection_wl.pri)

# Container
include(container/container_wl.pri)

# QvkImageFromTabs
include(imageFromTabs/imageFromTabs_wl.pri)

# Information
include(information/information_wl.pri)

# Countdown
include(countdown/countdown_wl.pri)
