TEMPLATE = app

CONFIG += console c++11
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
