TEMPLATE = lib

CONFIG += staticlib
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
    \
    data/types/builtintypes.cpp \
    data/types/labeltype.cpp \
    data/types/userdefinedtype.cpp \
    \
    astxmlparser.cpp \
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
    \
    data/types/builtintypes.h \
    data/types/labeltype.h \
    data/types/type.h \
    data/types/userdefinedtype.h \
    \
    astxmlparser.h \
    runparameters.h \
    testgenerator.h
