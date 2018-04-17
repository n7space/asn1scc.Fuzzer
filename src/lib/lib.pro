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
    \
    data/types/type.cpp \
    data/types/typevisitor.cpp \
    data/types/typefactory.cpp \
    data/types/labeltype.cpp \
    data/types/userdefinedtype.cpp \
    data/types/integer.cpp \
    data/types/bitstring.cpp \
    data/types/boolean.cpp \
    data/types/choice.cpp \
    data/types/enumerated.cpp \
    data/types/ia5string.cpp \
    data/types/null.cpp \
    data/types/numericstring.cpp \
    data/types/octetstring.cpp \
    data/types/sequence.cpp \
    data/types/sequenceof.cpp \
    data/types/real.cpp \
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
    data/constraints.h \
    \
    data/types/typefactory.h \
    data/types/labeltype.h \
    data/types/type.h \
    data/types/typevisitor.h \
    data/types/userdefinedtype.h \
    data/types/integer.h \
    data/types/bitstring.h \
    data/types/boolean.h \
    data/types/choice.h \
    data/types/enumerated.h \
    data/types/ia5string.h \
    data/types/null.h \
    data/types/numericstring.h \
    data/types/octetstring.h \
    data/types/sequence.h \
    data/types/sequenceof.h \
    data/types/real.h \
    \
    astxmlparser.h \
    runparameters.h \
    astfilegenerator.h \
    astfileprocessor.h \
    testgenerator.h
