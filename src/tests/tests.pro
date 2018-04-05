TEMPLATE = app

QT += testlib
QT -= gui
CONFIG += console c++11
CONFIG -= app_bundle

include(../../includelib.pri)

SOURCES += \
    main.cpp \
    \
    astxmlparser_tests.cpp

HEADERS += \
    astxmlparser_tests.h
