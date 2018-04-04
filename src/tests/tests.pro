TEMPLATE = app

QT += testlib
QT -= gui
CONFIG += console c++11
CONFIG -= app_bundle

include(../../includelib.pri)

SOURCES += \
    main.cpp \
    testclass_tests.cpp

HEADERS += \
    testclass_tests.h
