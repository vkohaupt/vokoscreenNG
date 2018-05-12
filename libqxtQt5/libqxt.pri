 
INCLUDEPATH	+= $$PWD
DEPENDPATH      += $$PWD

HEADERS		+= $$PWD/qxtglobal.h \
                   $$PWD/qxtwindowsystem.h \
                   $$PWD/qxtglobalshortcut.h \
                   $$PWD/qxtglobalshortcut_p.h 

SOURCES		+= $$PWD/qxtglobal.cpp \
                   $$PWD/qxtwindowsystem_x11.cpp \
                   $$PWD/qxtglobalshortcut_x11.cpp \
                   $$PWD/qxtglobalshortcut.cpp

LIBS += -lX11