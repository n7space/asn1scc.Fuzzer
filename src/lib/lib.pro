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
    data/mutatingvisitor.cpp \
    data/acnsequencecomponent.cpp \
    data/asnsequencecomponent.cpp \
    data/singlevalue.cpp \
    data/multiplevalue.cpp \
    data/namedvalue.cpp \
    data/choicevalue.cpp \
    \
    data/types/type.cpp \
    data/types/typemutatingvisitor.cpp \
    data/types/typefactory.cpp \
    data/types/labeltype.cpp \
    data/types/userdefinedtype.cpp \
    data/types/integer.cpp \
    data/types/integeracnparams.cpp \
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
    data/types/asciistringacnparameters.cpp \
    \
    cases/integerincorrectvalues.cpp \
    cases/constraintsrelaxingvisitor.cpp \
    cases/integerranges.cpp \
    cases/testcaseprinter.cpp \
    \
    astxmlparser.cpp \
    astfilegenerator.cpp \
    astfileprocessor.cpp \
    testgenerator.cpp \
    reconstructor.cpp \
    asn1nodereconstructingvisitor.cpp \
    acnnodereconstructingvisitor.cpp \
    asn1typecomponentreconstructingvisitor.cpp \
    acntypecomponentreconstructingvisitor.cpp \
    typeconstraintsreconstructingvisitor.cpp

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
    data/mutatingvisitor.h \
    data/acnparameter.h \
    data/asnsequencecomponent.h \
    data/acnsequencecomponent.h \
    data/sequencecomponent.h \
    data/acnargument.h \
    data/values.h \
    data/value.h \
    data/singlevalue.h \
    data/multiplevalue.h \
    data/namedvalue.h \
    data/choicevalue.h \
    data/range.h \
    data/rangelist.h \
    \
    data/types/typefactory.h \
    data/types/labeltype.h \
    data/types/type.h \
    data/types/typemutatingvisitor.h \
    data/types/typereadingvisitor.h \
    data/types/typevisitorwithvalue.h \
    data/types/userdefinedtype.h \
    data/types/integer.h \
    data/types/integeracnparams.h \
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
    data/types/string.h \
    data/types/acnparameterizablecomposite.h \
    data/types/asciistringacnparameters.h \
    \
    data/constraints/constraint.h \
    data/constraints/rangeconstraint.h \
    data/constraints/logicoperators.h \
    data/constraints/fromconstraint.h \
    data/constraints/sizeconstraint.h \
    data/constraints/constraintlist.h \
    data/constraints/withconstraints.h \
    data/constraints/constraintvisitor.h \
    data/constraints/printingvisitor.h \
    data/constraints/rangelistingvisitor.h \
    \
    cases/integerincorrectvalues.h \
    cases/constraintsrelaxingvisitor.h \
    cases/integerranges.h \
    cases/testcase.h \
    cases/testcaseprinter.h \
    \
    astxmlparser.h \
    astxmlconstraintparser.h \
    runparameters.h \
    astfilegenerator.h \
    astfileprocessor.h \
    testgenerator.h \
    reconstructor.h \
    asn1typecomponentreconstructingvisitor.h \
    typeconstraintsreconstructingvisitor.h \
    asn1nodereconstructingvisitor.h \
    acnnodereconstructingvisitor.h \
    acntypecomponentreconstructingvisitor.h
