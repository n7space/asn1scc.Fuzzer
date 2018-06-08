TEMPLATE = app

QT += testlib
QT -= gui
CONFIG += console
CONFIG += c++14
CONFIG -= app_bundle

include(../../includelib.pri)

SOURCES += \
    main.cpp \
    \
    astxmlparser_tests.cpp \
    expressiontree_tests.cpp

HEADERS += \
    astxmlparser_tests.h \
    expressiontree_tests.h
