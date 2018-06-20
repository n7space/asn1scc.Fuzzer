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
    nodereconstructingvisitor_tests.cpp \
    reconstructor_tests.cpp \
    \
    data/expressiontree/expressiontree_tests.cpp \
    \
    data/constraints/range_tests.cpp \
    data/constraints/rangelist_tests.cpp

HEADERS += \
    astxmlparser_tests.h \
    nodereconstructingvisitor_tests.h \
    reconstructor_tests.h \
    \
    data/expressiontree/expressiontree_tests.h \
    \
    data/constraints/range_tests.h \
    data/constraints/rangelist_tests.h

