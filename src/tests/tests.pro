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
    \
    data/expressiontree/expressiontree_tests.cpp

HEADERS += \
    astxmlparser_tests.h \
    \
    data/expressiontree/expressiontree_tests.h
