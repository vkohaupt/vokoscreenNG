INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD
HEADERS     += $$PWD/mainWindow_wl.h \
               $$PWD/portal_wl.h
  
SOURCES     += $$PWD/mainWindow_wl.cpp \
               $$PWD/portal_wl.cpp

FORMS       += $$PWD/formMainWindow_wl.ui

# Area margins
include(regionMargins/regionMargins_wl.pri)

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

# Settings
include(settings/settings_wl.pri)

# ShowMessage
include(showMessage/showMessage_wl.pri)

# Camera
include(camera/camera_wl.pri)

# Audio
include(audio/audio_wl.pri)
include(audiopipewire/audiopipewire.pri)

# Convert
include(convert/convert_wl.pri)

# Help
include(help/help_wl.pri)

# Systray
include(systray/systray_wl.pri)

# Log
include(log/log_wl.pri)
