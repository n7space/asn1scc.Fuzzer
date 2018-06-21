/****************************************************************************
**
** Copyright (C) 2018 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN MalTester - Tool for generating test cases
** based on ASN.1/ACN models and simulating malformed or malicious data.
**
** Tool was developed under a programme and funded by
** European Space Agency.
**
** This Tool is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Tool is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "nodereconstructingvisitor_tests.h"

#include <QtTest>

#include <nodereconstructingvisitor.h>
#include <reconstructor.h>

#include <data/choicevalue.h>
#include <data/multiplevalue.h>
#include <data/namedvalue.h>
#include <data/singlevalue.h>
#include <data/sourcelocation.h>

#include <data/types/choice.h>
#include <data/types/enumerated.h>
#include <data/types/integer.h>
#include <data/types/sequence.h>
#include <data/types/sequenceof.h>
#include <data/types/typefactory.h>
#include <data/types/userdefinedtype.h>

using namespace MalTester;
using namespace MalTester::Tests;

NodeReconstructingVisitorTests::NodeReconstructingVisitorTests(QObject *parent)
    : QObject(parent)
{}

void NodeReconstructingVisitorTests::test_singleImportedType()
{
    auto defs = createDefinitions(QStringLiteral("MyModule_1"));
    defs->addImportedType(
        Data::ImportedType(QStringLiteral("MyModule_1"), QStringLiteral("ImportedType_1")));

    NodeReconstructingVisitor visitor;
    defs->accept(visitor);

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "IMPORTS\n"
        "ImportedType_1 FROM MyModule_1\n"
        ";\n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(visitor.value(), expected);
}

void NodeReconstructingVisitorTests::test_multipleImportedTypes()
{
    auto defs = createDefinitions(QStringLiteral("MyModule_1"));

    defs->addImportedType(
        Data::ImportedType(QStringLiteral("MyModule_1"), QStringLiteral("ImportedType_1")));
    defs->addImportedType(
        Data::ImportedType(QStringLiteral("MyModule_2"), QStringLiteral("ImportedType_2")));

    NodeReconstructingVisitor visitor;
    defs->accept(visitor);

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "IMPORTS\n"
        "ImportedType_1 FROM MyModule_1\n"
        "ImportedType_2 FROM MyModule_2\n"
        ";\n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(visitor.value(), expected);
}

void NodeReconstructingVisitorTests::test_singleImportedValue()
{
    auto defs = createDefinitions(QStringLiteral("MyModule_1"));
    defs->addImportedValue(
        Data::ImportedValue(QStringLiteral("MyModule_1"), QStringLiteral("ImportedValue_1")));

    NodeReconstructingVisitor visitor;
    defs->accept(visitor);

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "IMPORTS\n"
        "ImportedValue_1 FROM MyModule_1\n"
        ";\n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(visitor.value(), expected);
}

void NodeReconstructingVisitorTests::test_multipleImportedValues()
{
    auto defs = createDefinitions(QStringLiteral("MyModule_1"));

    defs->addImportedValue(
        Data::ImportedValue(QStringLiteral("MyModule_1"), QStringLiteral("ImportedValue_1")));
    defs->addImportedValue(
        Data::ImportedValue(QStringLiteral("MyModule_2"), QStringLiteral("ImportedValue_2")));

    NodeReconstructingVisitor visitor;
    defs->accept(visitor);

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "IMPORTS\n"
        "ImportedValue_1 FROM MyModule_1\n"
        "ImportedValue_2 FROM MyModule_2\n"
        ";\n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(visitor.value(), expected);
}

void NodeReconstructingVisitorTests::test_valueAssignmentSimpleValue()
{
    auto actual = createSingleValueValueAssignmentValue(QStringLiteral("INTEGER"),
                                                        QStringLiteral("10"),
                                                        Data::printAsSelf);
    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "myValue INTEGER ::= 10\n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void NodeReconstructingVisitorTests::test_valueAssignmentBitString()
{
    auto actual = createSingleValueValueAssignmentValue(QStringLiteral("BIT_STRING"),
                                                        QStringLiteral("1010"),
                                                        Data::printAsBitString);

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "myValue BIT STRING ::= '1010'B\n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void NodeReconstructingVisitorTests::test_valueAssignmentBoolean()
{
    auto actual = createSingleValueValueAssignmentValue(QStringLiteral("BOOLEAN"),
                                                        QStringLiteral("TRUE"),
                                                        Data::printInBooleanFormat);
    // clang-format off
    QString expected =
            "MyModule_1 DEFINITIONS ::= BEGIN\n"
            "myValue BOOLEAN ::= TRUE\n"
            "\n"
            "END\n"
            "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void NodeReconstructingVisitorTests::test_valueAssignmentOctetString()
{
    auto actual = createSingleValueValueAssignmentValue(QStringLiteral("OCTET_STRING"),
                                                        QStringLiteral("1010"),
                                                        Data::printAsHexString);

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "myValue OCTET STRING ::= '1010'H\n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void NodeReconstructingVisitorTests::test_valueAssignmentIA5String()
{
    auto actual = createSingleValueValueAssignmentValue(QStringLiteral("IA5String"),
                                                        QStringLiteral("MyValue"),
                                                        Data::printAsASCIIString);
    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "myValue IA5String ::= \"MyValue\"\n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void NodeReconstructingVisitorTests::test_valueAssignmentChoice()
{
    auto actual = createChoiceValueValueAssignmentValue();

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "myChoice CHOICE ::= MyChoice:10\n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void NodeReconstructingVisitorTests::test_valueAssignmentMultipleValues()
{
    auto actual = createMultipleValueValueAssignmentValue();

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "mySequence SEQUENCE OF ::= {1, 2, 3}\n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void NodeReconstructingVisitorTests::test_valueAssignmentNamedValue()
{
    auto actual = createNamedValueAssignmentValue();

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "mySequence SEQUENCE ::= {v1 1, v2 TRUE, v3 3.0}\n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void NodeReconstructingVisitorTests::test_typeAssignmentBoolean()
{
    testSimpleTypeAssignment(QStringLiteral("BOOLEAN"), QStringLiteral("BOOLEAN"));
}

void NodeReconstructingVisitorTests::test_typeAssignmentNull()
{
    testSimpleTypeAssignment(QStringLiteral("NULL"), QStringLiteral("NULL"));
}

void NodeReconstructingVisitorTests::test_typeAssignmentBitString()
{
    testSimpleTypeAssignment(QStringLiteral("BIT_STRING"), QStringLiteral("BIT STRING"));
}

void NodeReconstructingVisitorTests::test_typeAssignmentOctetString()
{
    testSimpleTypeAssignment(QStringLiteral("OCTET_STRING"), QStringLiteral("OCTET STRING"));
}

void NodeReconstructingVisitorTests::test_typeAssignmentIA5String()
{
    testSimpleTypeAssignment(QStringLiteral("IA5String"), QStringLiteral("IA5String"));
}

void NodeReconstructingVisitorTests::test_typeAssignmentNumericString()
{
    testSimpleTypeAssignment(QStringLiteral("NumericString"), QStringLiteral("NumericString"));
}

void NodeReconstructingVisitorTests::test_typeAssignmentEnumerated()
{
    auto type = std::make_unique<Data::Types::Enumerated>();
    type->addItem(QStringLiteral("e1"),
                  Data::Types::EnumeratedItem(QStringLiteral("e1"), 1, Data::SourceLocation()));
    type->addItem(QStringLiteral("e2"),
                  Data::Types::EnumeratedItem(QStringLiteral("e2"), 2, Data::SourceLocation()));

    auto actual = createComponentialTypeAssignmentValue(std::move(type));

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "MyType ::= ENUMERATED \n"
        "{ \n"
        "    e1 ( 1 ) , \n"
        "    e2 ( 2 ) \n"
        "} \n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void NodeReconstructingVisitorTests::test_typeAssignmentChoice()
{
    auto type = std::make_unique<Data::Types::Choice>();

    type->addComponent(
        std::make_unique<Data::Types::ChoiceAlternative>(QStringLiteral("ch1"),
                                                         QStringLiteral(""),
                                                         QStringLiteral(""),
                                                         QStringLiteral(""),
                                                         QStringLiteral(""),
                                                         Data::SourceLocation(),
                                                         Data::Types::TypeFactory::createBuiltinType(
                                                             QStringLiteral("INTEGER"))));

    type->addComponent(
        std::make_unique<Data::Types::ChoiceAlternative>(QStringLiteral("ch2"),
                                                         QStringLiteral(""),
                                                         QStringLiteral(""),
                                                         QStringLiteral(""),
                                                         QStringLiteral(""),
                                                         Data::SourceLocation(),
                                                         Data::Types::TypeFactory::createBuiltinType(
                                                             QStringLiteral("REAL"))));

    auto actual = createComponentialTypeAssignmentValue(std::move(type));

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "MyType ::= CHOICE \n"
        "{ \n"
        "    ch1 INTEGER , \n"
        "    ch2 REAL \n"
        "} \n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void NodeReconstructingVisitorTests::test_typeAssignmentSequence()
{
    auto type = std::make_unique<Data::Types::Sequence>();
    type->addComponent(
        std::make_unique<Data::SequenceComponent>(QStringLiteral("seq1"),
                                                  Data::Types::TypeFactory::createBuiltinType(
                                                      QStringLiteral("INTEGER"))));

    type->addComponent(
        std::make_unique<Data::SequenceComponent>(QStringLiteral("seq2"),
                                                  Data::Types::TypeFactory::createBuiltinType(
                                                      QStringLiteral("REAL"))));

    auto actual = createComponentialTypeAssignmentValue(std::move(type));

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "MyType ::= SEQUENCE \n"
        "{ \n"
        "    seq1 INTEGER , \n"
        "    seq2 REAL \n"
        "} \n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void NodeReconstructingVisitorTests::test_typeAssignmentSequenceOf()
{
    auto type = std::make_unique<Data::Types::SequenceOf>();
    type->setItemsType(std::make_unique<Data::Types::Integer>());

    auto actual = createComponentialTypeAssignmentValue(std::move(type));

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "MyType ::= SEQUENCE  OF INTEGER\n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void NodeReconstructingVisitorTests::test_typeAssignmentReal()
{
    testSimpleTypeAssignment(QStringLiteral("REAL"), QStringLiteral("REAL"));
}

void NodeReconstructingVisitorTests::test_typeAssignmentInteger()
{
    testSimpleTypeAssignment(QStringLiteral("INTEGER"), QStringLiteral("INTEGER"));
}

void NodeReconstructingVisitorTests::test_typeAssignmentUserDefined()
{
    auto type = std::make_unique<Data::Types::UserdefinedType>(QStringLiteral("ReferencedType"),
                                                               QStringLiteral("MyModule"));
    type->setType(Data::Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER")));

    auto assignment = std::make_unique<Data::TypeAssignment>(QStringLiteral("MyType"),
                                                             Data::SourceLocation(),
                                                             std::move(type));

    auto defs = createDefinitions(QStringLiteral("MyModule_1"));
    defs->addType(std::move(assignment));

    NodeReconstructingVisitor visitor;
    defs->accept(visitor);

    auto actual = visitor.value();

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "MyType ::= ReferencedType\n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void NodeReconstructingVisitorTests::testSimpleTypeAssignment(const QString &astValue,
                                                              const QString &asn1Value) const
{
    auto type = Data::Types::TypeFactory::createBuiltinType(astValue);
    auto assignment = std::make_unique<Data::TypeAssignment>(QStringLiteral("MyType"),
                                                             Data::SourceLocation(),
                                                             std::move(type));

    auto defs = createDefinitions(QStringLiteral("MyModule_1"));
    defs->addType(std::move(assignment));

    NodeReconstructingVisitor visitor;
    defs->accept(visitor);

    auto actual = visitor.value();

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "MyType ::= " + asn1Value + "\n"
        "\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

std::unique_ptr<Data::Definitions> NodeReconstructingVisitorTests::createDefinitions(
    const QString &name) const
{
    return std::make_unique<Data::Definitions>(name, Data::SourceLocation());
}

QString NodeReconstructingVisitorTests::createSingleValueValueAssignmentValue(
    const QString &typeName,
    const QString &typeValue,
    std::function<QString(const QString &)> printer) const
{
    auto type = Data::Types::TypeFactory::createBuiltinType(typeName);
    auto value = std::make_unique<Data::SingleValue>(typeValue, printer);

    auto assignment = std::make_unique<Data::ValueAssignment>(QStringLiteral("myValue"),
                                                              Data::SourceLocation(),
                                                              std::move(type),
                                                              std::move(value));
    return createValueFromValueAssignment(assignment);
}

QString NodeReconstructingVisitorTests::createMultipleValueValueAssignmentValue() const
{
    auto multipleValue = std::make_unique<Data::MultipleValue>();

    multipleValue->addValue(std::make_unique<Data::SingleValue>(QStringLiteral("1")));
    multipleValue->addValue(std::make_unique<Data::SingleValue>(QStringLiteral("2")));
    multipleValue->addValue(std::make_unique<Data::SingleValue>(QStringLiteral("3")));

    auto type = Data::Types::TypeFactory::createBuiltinType(QStringLiteral("SEQUENCE_OF"));
    auto assignment = std::make_unique<Data::ValueAssignment>(QStringLiteral("mySequence"),
                                                              Data::SourceLocation(),
                                                              std::move(type),
                                                              std::move(multipleValue));
    return createValueFromValueAssignment(assignment);
}

QString NodeReconstructingVisitorTests::createNamedValueAssignmentValue() const
{
    auto multipleValue = std::make_unique<Data::NamedValue>();

    multipleValue->addValue(QStringLiteral("v1"), std::make_unique<Data::SingleValue>("1"));
    multipleValue->addValue(QStringLiteral("v2"), std::make_unique<Data::SingleValue>("TRUE"));
    multipleValue->addValue(QStringLiteral("v3"), std::make_unique<Data::SingleValue>("3.0"));

    auto type = Data::Types::TypeFactory::createBuiltinType(QStringLiteral("SEQUENCE"));
    auto assignment = std::make_unique<Data::ValueAssignment>(QStringLiteral("mySequence"),
                                                              Data::SourceLocation(),
                                                              std::move(type),
                                                              std::move(multipleValue));
    return createValueFromValueAssignment(assignment);
}

QString NodeReconstructingVisitorTests::createComponentialTypeAssignmentValue(
    std::unique_ptr<Data::Types::Type> type) const
{
    auto assignment = std::make_unique<Data::TypeAssignment>(QStringLiteral("MyType"),
                                                             Data::SourceLocation(),
                                                             std::move(type));

    auto defs = createDefinitions(QStringLiteral("MyModule_1"));
    defs->addType(std::move(assignment));

    NodeReconstructingVisitor visitor;
    defs->accept(visitor);

    return visitor.value();
}

QString NodeReconstructingVisitorTests::createChoiceValueValueAssignmentValue() const
{
    auto internalValue = std::make_unique<Data::SingleValue>(QStringLiteral("10"));
    auto choiceValue = std::make_unique<Data::ChoiceValue>(QStringLiteral("MyChoice"),
                                                           std::move(internalValue));

    auto type = Data::Types::TypeFactory::createBuiltinType(QStringLiteral("CHOICE"));
    auto assignment = std::make_unique<Data::ValueAssignment>(QStringLiteral("myChoice"),
                                                              Data::SourceLocation(),
                                                              std::move(type),
                                                              std::move(choiceValue));
    return createValueFromValueAssignment(assignment);
}

QString NodeReconstructingVisitorTests::createValueFromValueAssignment(
    std::unique_ptr<Data::ValueAssignment> &assignment) const
{
    auto defs = createDefinitions(QStringLiteral("MyModule_1"));
    defs->addValue(std::move(assignment));

    NodeReconstructingVisitor visitor;
    defs->accept(visitor);
    return visitor.value();
}
