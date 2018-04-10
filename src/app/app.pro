TEMPLATE = app

CONFIG += console
CONFIG += c++14
CONFIG -= app_bundle
QT -= gui

include(../../includelib.pri)

SOURCES += \
    main.cpp \
    inputparametersparser.cpp \
    maintask.cpp

HEADERS += \
    inputparametersparser.h \
    maintask.h
