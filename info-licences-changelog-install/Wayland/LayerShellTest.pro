QT += core gui widgets

CONFIG += c++17
INCLUDEPATH += /usr/include/LayerShellQt
LIBS += -L/usr/lib64 -lLayerShellQtInterface

SOURCES += \
    main.cpp

TARGET = LayerShellTest
