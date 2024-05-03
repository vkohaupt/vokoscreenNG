TEMPLATE = app
TARGET = audiosource

QT += multimedia widgets

HEADERS       = audiosource.h

SOURCES       = audiosource.cpp \
                main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/multimedia/audiosource
INSTALLS += target
include(../shared/shared.pri)
