TEMPLATE = app

TARGET = asn1scc-fuzzer

CONFIG += console
CONFIG += c++14
CONFIG -= app_bundle
QT -= gui

QMAKE_RPATHDIR += $ORIGIN/lib

include(../../includelib.pri)
include(../../warnings.pri)

SOURCES += \
    main.cpp \
    inputparametersparser.cpp \
    maintask.cpp \
    astfilegenerator.cpp \
    fuzzerrunner.cpp

HEADERS += \
    inputparametersparser.h \
    maintask.h \
    astfilegenerator.h \
    runparameters.h \
    fuzzerrunner.h

RESOURCES += \
    templates.qrc
