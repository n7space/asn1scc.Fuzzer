TEMPLATE = app

QT += testlib
QT -= gui
CONFIG += console
CONFIG += c++14
CONFIG -= app_bundle

include(../../includelib.pri)

SOURCES += \
    astxmlparser_tests.cpp \
    asn1nodereconstructingvisitor_tests.cpp \
    acnnodereconstructingvisitor_tests.cpp \
    reconstructor_tests.cpp \
    astxmlconstraintparser_tests.cpp \
    \
    cases/integerincorrectvalues_tests.cpp \
    cases/integerranges_tests.cpp \
    cases/testcaseprinter_tests.cpp \
    \
    data/range_tests.cpp \
    data/rangelist_tests.cpp \
    \
    data/constraints/printingvisitor_tests.cpp \
    \
    main.cpp

HEADERS += \
    astxmlparser_tests.h \
    asn1nodereconstructingvisitor_tests.h \
    acnnodereconstructingvisitor_tests.h \
    reconstructor_tests.h \
    astxmlconstraintparser_tests.h \
    \
    cases/integerincorrectvalues_tests.h \
    cases/integerranges_tests.h \
    cases/testcaseprinter_tests.h \
    \
    data/range_tests.h \
    data/rangelist_tests.h \
    \
    data/constraints/printingvisitor_tests.h
