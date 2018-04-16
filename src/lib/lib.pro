TEMPLATE = lib

CONFIG += staticlib
CONFIG += c++14

QT -= gui

SOURCES += \
    data/definitions.cpp \
    data/file.cpp \
    data/node.cpp \
    data/project.cpp \
    data/root.cpp \
    data/sourcelocation.cpp \
    data/typeassignment.cpp \
    data/typereference.cpp \
    data/valueassignment.cpp \
    data/visitor.cpp \
    data/acnparameters.cpp \
    \
    data/types/builtintypes.cpp \
    data/types/labeltype.cpp \
    data/types/userdefinedtype.cpp \
    \
    astxmlparser.cpp \
    astfilegenerator.cpp \
    astfileprocessor.cpp \
    testgenerator.cpp

HEADERS += \
    data/definitions.h \
    data/errormessage.h \
    data/file.h \
    data/importedtype.h \
    data/importedvalue.h \
    data/node.h \
    data/project.h \
    data/root.h \
    data/sourcelocation.h \
    data/typeassignment.h \
    data/typereference.h \
    data/valueassignment.h \
    data/visitor.h \
    data/visitorwithvalue.h \
    data/acnparameters.h \
    data/constraints.h \
    \
    data/types/builtintypes.h \
    data/types/labeltype.h \
    data/types/type.h \
    data/types/userdefinedtype.h \
    data/types/integertype.h \
    data/types/realtype.h \
    \
    astxmlparser.h \
    runparameters.h \
    astfilegenerator.h \
    astfileprocessor.h \
    testgenerator.h
