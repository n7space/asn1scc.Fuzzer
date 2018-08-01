TEMPLATE = app

TARGET = asn1scc-maltester

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
    maintask.cpp

HEADERS += \
    inputparametersparser.h \
    maintask.h

RESOURCES += \
    templates.qrc
