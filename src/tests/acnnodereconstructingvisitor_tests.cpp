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

#include "acnnodereconstructingvisitor_tests.h"

#include <QtTest>

#include <acnnodereconstructingvisitor.h>
#include <reconstructor.h>

#include <data/acnsequencecomponent.h>
#include <data/asnsequencecomponent.h>
#include <data/choicevalue.h>
#include <data/multiplevalue.h>
#include <data/namedvalue.h>
#include <data/singlevalue.h>
#include <data/sourcelocation.h>

#include <data/types/bitstring.h>
#include <data/types/boolean.h>
#include <data/types/choice.h>
#include <data/types/enumerated.h>
#include <data/types/ia5string.h>
#include <data/types/integer.h>
#include <data/types/null.h>
#include <data/types/numericstring.h>
#include <data/types/octetstring.h>
#include <data/types/real.h>
#include <data/types/sequence.h>
#include <data/types/sequenceof.h>
#include <data/types/typefactory.h>
#include <data/types/userdefinedtype.h>

using namespace MalTester;
using namespace MalTester::Tests;

AcnNodeReconstructingVisitorTests::AcnNodeReconstructingVisitorTests(QObject *parent)
    : QObject(parent)
{}

void AcnNodeReconstructingVisitorTests::test_emptyModule()
{
    auto defs = std::make_unique<Data::Definitions>(QStringLiteral("MyModule"),
                                                    Data::SourceLocation());

    auto actual = restoreNode(*defs);
    auto expected = QStringLiteral("MyModule DEFINITIONS ::= BEGIN\n"
                                   "END\n"
                                   "\n");

    QCOMPARE(actual, expected);
}

void AcnNodeReconstructingVisitorTests::test_boolean()
{
    auto type = std::make_unique<Data::Types::Boolean>();

    type->setTrueValue(QStringLiteral("1010"));
    type->setFalseValue(QStringLiteral("0101"));
    type->setAlignToNext(Data::Types::Type::mapAlignToNext(QString("word")));

    auto expectedResult = QStringLiteral(
        "MyBool [align-to-next word, true-value '1010'B, false-value '0101'B]\n");

    performTest(QStringLiteral("MyBool"), expectedResult, std::move(type));
}

void AcnNodeReconstructingVisitorTests::test_null()
{
    auto type = std::make_unique<Data::Types::Null>();

    type->setPattern(QStringLiteral("0011"));
    type->setAlignToNext(Data::Types::Type::mapAlignToNext(QString("word")));

    auto expectedResult = QStringLiteral("MyNull [align-to-next word, pattern '0011'B]\n");

    performTest(QStringLiteral("MyNull"), expectedResult, std::move(type));
}

void AcnNodeReconstructingVisitorTests::test_integer()
{
    auto type = std::make_unique<Data::Types::Integer>();

    type->setAlignToNext(Data::Types::Type::mapAlignToNext(QString("word")));
    type->setSize(10);
    type->setEncoding(Data::Types::IntegerAcnParameters::mapEncoding(QStringLiteral("pos-int")));
    type->setEndianness(Data::Types::Type::mapEndianess(QStringLiteral("big")));

    auto expectedResult = QStringLiteral(
        "MyInteger [align-to-next word, size 10, encoding pos-int, endianness big]\n");

    performTest(QStringLiteral("MyInteger"), expectedResult, std::move(type));
}

void AcnNodeReconstructingVisitorTests::test_real()
{
    auto type = std::make_unique<Data::Types::Real>();

    type->setAlignToNext(Data::Types::Type::mapAlignToNext(QString("dword")));
    type->setEncoding(Data::Types::Real::mapEncoding(QStringLiteral("IEEE754-1985-32")));
    type->setEndianness(Data::Types::Type::mapEndianess(QStringLiteral("big")));

    auto expectedResult = QStringLiteral(
        "MyReal [align-to-next dword, encoding IEEE754-1985-32, endianness big]\n");

    performTest(QStringLiteral("MyReal"), expectedResult, std::move(type));
}

void AcnNodeReconstructingVisitorTests::test_bitString()
{
    auto type = std::make_unique<Data::Types::BitString>();

    type->setAcnSize(QStringLiteral("10"));
    type->setAlignToNext(Data::Types::Type::mapAlignToNext(QString("dword")));

    auto expectedResult = QStringLiteral("MyBitString [align-to-next dword, size 10]\n");

    performTest(QStringLiteral("MyBitString"), expectedResult, std::move(type));
}

void AcnNodeReconstructingVisitorTests::test_octetString()
{
    auto type = std::make_unique<Data::Types::OctetString>();

    type->setAcnSize(QStringLiteral("10"));
    type->setAlignToNext(Data::Types::Type::mapAlignToNext(QString("dword")));

    auto expectedResult = QStringLiteral("MyOctetString [align-to-next dword, size 10]\n");

    performTest(QStringLiteral("MyOctetString"), expectedResult, std::move(type));
}

void AcnNodeReconstructingVisitorTests::test_IA5String()
{
    auto type = std::make_unique<Data::Types::IA5String>();

    type->setAcnSize(QStringLiteral("10"));
    type->setAlignToNext(Data::Types::Type::mapAlignToNext(QString("dword")));
    type->setTerminationPattern(QStringLiteral("10101010"));
    type->setEncoding(Data::Types::AsciiStringAcnParameters::mapEncoding(QStringLiteral("ASCII")));

    auto expectedResult = QStringLiteral(
        "MyIA5String [align-to-next dword, size 10, encoding ASCII, termination-pattern '10101010'H]\n");

    performTest(QStringLiteral("MyIA5String"), expectedResult, std::move(type));
}

void AcnNodeReconstructingVisitorTests::test_numericString()
{
    auto type = std::make_unique<Data::Types::NumericString>();

    type->setAcnSize(QStringLiteral("10"));
    type->setAlignToNext(Data::Types::Type::mapAlignToNext(QString("dword")));
    type->setTerminationPattern(QStringLiteral("10101010"));
    type->setEncoding(Data::Types::AsciiStringAcnParameters::mapEncoding(QStringLiteral("ASCII")));

    auto expectedResult = QStringLiteral(
        "MyNumericString [align-to-next dword, size 10, encoding ASCII, termination-pattern '10101010'H]\n");

    performTest(QStringLiteral("MyNumericString"), expectedResult, std::move(type));
}

void AcnNodeReconstructingVisitorTests::test_enumerated()
{
    auto type = std::make_unique<Data::Types::Enumerated>();

    type->setAlignToNext(Data::Types::Type::mapAlignToNext(QString("word")));
    type->setSize(10);
    type->setEncoding(Data::Types::IntegerAcnParameters::mapEncoding(QStringLiteral("pos-int")));
    type->setEndianness(Data::Types::Type::mapEndianess(QStringLiteral("big")));
    type->setEncodeValues(true);

    auto expectedResult = QStringLiteral(
        "MyEnumerated [align-to-next word, size 10, encoding pos-int, endianness big]\n");

    performTest(QStringLiteral("MyEnumerated"), expectedResult, std::move(type));
}

void AcnNodeReconstructingVisitorTests::test_sequenceOf()
{
    auto type = std::make_unique<Data::Types::SequenceOf>();

    type->setAlignToNext(Data::Types::Type::mapAlignToNext(QString("dword")));
    type->setAcnSize(QStringLiteral("10"));

    auto expectedResult = QStringLiteral("MySequenceOf [align-to-next dword, size 10]\n");

    performTest(QStringLiteral("MySequenceOf"), expectedResult, std::move(type));
}

void AcnNodeReconstructingVisitorTests::test_choice()
{
    auto type = std::make_unique<Data::Types::Choice>();

    type->setAlignToNext(Data::Types::Type::mapAlignToNext(QString("word")));

    auto expectedResult = QStringLiteral("MyChoice [align-to-next word]\n");

    performTest(QStringLiteral("MyChoice"), expectedResult, std::move(type));
}

void AcnNodeReconstructingVisitorTests::test_choiceNested()
{
    auto choiceType = std::make_unique<Data::Types::Choice>();
    auto sequenceType = std::make_unique<Data::Types::Sequence>();

    sequenceType->addComponent(
        std::make_unique<Data::AcnSequenceComponent>(QStringLiteral("id1"),
                                                     QStringLiteral("seqItem1"),
                                                     Data::Types::TypeFactory::createBuiltinType(
                                                         QStringLiteral("BOOLEAN"))));

    sequenceType->addComponent(
        std::make_unique<Data::AsnSequenceComponent>(QStringLiteral("seqItem2"),
                                                     true,
                                                     QStringLiteral("seqItem1"),
                                                     Data::SourceLocation(),
                                                     Data::Types::TypeFactory::createBuiltinType(
                                                         QStringLiteral("INTEGER"))));

    choiceType->addComponent(
        std::make_unique<Data::Types::ChoiceAlternative>(QStringLiteral("choiceAlt1"),
                                                         QStringLiteral(""),
                                                         QStringLiteral("ada-name-attr"),
                                                         QStringLiteral("c-name-attr"),
                                                         QStringLiteral("presWhen1"),
                                                         Data::SourceLocation(),
                                                         std::move(sequenceType)));

    choiceType->addComponent(
        std::make_unique<Data::Types::ChoiceAlternative>(QStringLiteral("choiceAlt2"),
                                                         QStringLiteral(""),
                                                         QStringLiteral("ada-name-attr"),
                                                         QStringLiteral("c-name-attr"),
                                                         QStringLiteral(""),
                                                         Data::SourceLocation(),
                                                         Data::Types::TypeFactory::createBuiltinType(
                                                             QStringLiteral("INTEGER"))));

    auto expectedResult = QStringLiteral("MyChoice []\n"
                                         "{\n"
                                         "    choiceAlt1 [present-when presWhen1]\n"
                                         "    {\n"
                                         "        seqItem1 BOOLEAN [],\n"
                                         "        seqItem2 [present-when seqItem1]\n"
                                         "    },\n"
                                         "    choiceAlt2 []\n"
                                         "}\n");

    performTest(QStringLiteral("MyChoice"), expectedResult, std::move(choiceType));
}

void AcnNodeReconstructingVisitorTests::test_choiceParametrized()
{
    auto choiceType = std::make_unique<Data::Types::Choice>();

    choiceType->addComponent(
        std::make_unique<Data::Types::ChoiceAlternative>(QStringLiteral("choiceItem1"),
                                                         QStringLiteral(""),
                                                         QStringLiteral("ada-name-attr"),
                                                         QStringLiteral("c-name-attr"),
                                                         QStringLiteral("type1 == 1 type2 == 2"),
                                                         Data::SourceLocation(),
                                                         Data::Types::TypeFactory::createBuiltinType(
                                                             QStringLiteral("INTEGER"))));
    choiceType->addComponent(
        std::make_unique<Data::Types::ChoiceAlternative>(QStringLiteral("choiceItem2"),
                                                         QStringLiteral(""),
                                                         QStringLiteral("ada-name-attr"),
                                                         QStringLiteral("c-name-attr"),
                                                         QStringLiteral("type1 == 3 type2 == 4"),
                                                         Data::SourceLocation(),
                                                         Data::Types::TypeFactory::createBuiltinType(
                                                             QStringLiteral("INTEGER"))));
    choiceType->addComponent(
        std::make_unique<Data::Types::ChoiceAlternative>(QStringLiteral("choiceItem3"),
                                                         QStringLiteral(""),
                                                         QStringLiteral("ada-name-attr"),
                                                         QStringLiteral("c-name-attr"),
                                                         QStringLiteral("type1 == 5 type2 == 6"),
                                                         Data::SourceLocation(),
                                                         Data::Types::TypeFactory::createBuiltinType(
                                                             QStringLiteral("INTEGER"))));

    choiceType->addParameter(std::make_unique<Data::AcnParameter>(QStringLiteral("type1"),
                                                                  QStringLiteral("type1"),
                                                                  QStringLiteral("INTEGER")));

    choiceType->addParameter(std::make_unique<Data::AcnParameter>(QStringLiteral("type2"),
                                                                  QStringLiteral("type2"),
                                                                  QStringLiteral("INTEGER")));

    auto expectedResult = QStringLiteral("MyChoice <INTEGER:type1, INTEGER:type2> []\n"
                                         "{\n"
                                         "    choiceItem1 [present-when type1 == 1 type2 == 2],\n"
                                         "    choiceItem2 [present-when type1 == 3 type2 == 4],\n"
                                         "    choiceItem3 [present-when type1 == 5 type2 == 6]\n"
                                         "}\n");

    performTest(QStringLiteral("MyChoice"), expectedResult, std::move(choiceType));
}

void AcnNodeReconstructingVisitorTests::test_sequence()
{
    auto type = std::make_unique<Data::Types::Sequence>();

    type->setAlignToNext(Data::Types::Type::mapAlignToNext(QString("word")));

    auto expectedResult = QStringLiteral("MySequence [align-to-next word]\n");

    performTest(QStringLiteral("MySequence"), expectedResult, std::move(type));
}

void AcnNodeReconstructingVisitorTests::test_sequenceNested()
{
    auto sequenceType = std::make_unique<Data::Types::Sequence>();
    auto choiceType = std::make_unique<Data::Types::Choice>();

    choiceType->addComponent(
        std::make_unique<Data::Types::ChoiceAlternative>(QStringLiteral("choiceAlt1"),
                                                         QStringLiteral(""),
                                                         QStringLiteral("ada-name-attr"),
                                                         QStringLiteral("c-name-attr"),
                                                         QStringLiteral(""),
                                                         Data::SourceLocation(),
                                                         Data::Types::TypeFactory::createBuiltinType(
                                                             QStringLiteral("INTEGER"))));

    choiceType->addComponent(
        std::make_unique<Data::Types::ChoiceAlternative>(QStringLiteral("choiceAlt2"),
                                                         QStringLiteral(""),
                                                         QStringLiteral("ada-name-attr"),
                                                         QStringLiteral("c-name-attr"),
                                                         QStringLiteral(""),
                                                         Data::SourceLocation(),
                                                         Data::Types::TypeFactory::createBuiltinType(
                                                             QStringLiteral("INTEGER"))));

    sequenceType->addComponent(
        std::make_unique<Data::AsnSequenceComponent>(QStringLiteral("seq1"),
                                                     true,
                                                     QStringLiteral("preWhenValue"),
                                                     Data::SourceLocation(),
                                                     Data::Types::TypeFactory::createBuiltinType(
                                                         QStringLiteral("INTEGER"))));

    sequenceType->addComponent(std::make_unique<Data::AsnSequenceComponent>(QStringLiteral("seq2"),
                                                                            false,
                                                                            QStringLiteral(""),
                                                                            Data::SourceLocation(),
                                                                            std::move(choiceType)));

    auto expectedResult = QStringLiteral("MySequence []\n"
                                         "{\n"
                                         "    seq1 [present-when preWhenValue],\n"
                                         "    seq2 []\n"
                                         "    {\n"
                                         "        choiceAlt1 [],\n"
                                         "        choiceAlt2 []\n"
                                         "    }\n"
                                         "}\n");

    performTest(QStringLiteral("MySequence"), expectedResult, std::move(sequenceType));
}

void AcnNodeReconstructingVisitorTests::test_acnParameter()
{
    auto type = std::make_unique<Data::Types::Sequence>();
    type->addParameter(std::make_unique<Data::AcnParameter>(QStringLiteral("id"),
                                                            QStringLiteral("ParamName"),
                                                            QStringLiteral("ParamType")));

    auto expectedResult = QStringLiteral("MySequence <ParamType:ParamName> []\n");

    performTest(QStringLiteral("MySequence"), expectedResult, std::move(type));
}

void AcnNodeReconstructingVisitorTests::test_acnArgument()
{
    auto type = std::make_unique<Data::Types::UserdefinedType>(QStringLiteral("MyType"),
                                                               QStringLiteral("MyModule"));
    type->addArgument(std::make_unique<Data::AcnArgument>(QStringLiteral("ArgumentName")));

    auto expectedResult = QStringLiteral("MySequence <ArgumentName> []\n");

    performTest(QStringLiteral("MySequence"), expectedResult, std::move(type));
}

void AcnNodeReconstructingVisitorTests::test_integerEncodingToString()
{
    using namespace Data::Types;
    QCOMPARE(IntegerAcnParameters::encodingToString(IntegerEncoding::pos_int),
             QStringLiteral("pos-int"));
    QCOMPARE(IntegerAcnParameters::encodingToString(IntegerEncoding::twos_complement),
             QStringLiteral("twos-complement"));
    QCOMPARE(IntegerAcnParameters::encodingToString(IntegerEncoding::ASCII),
             QStringLiteral("ASCII"));
    QCOMPARE(IntegerAcnParameters::encodingToString(IntegerEncoding::BCD), QStringLiteral("BCD"));
    QCOMPARE(IntegerAcnParameters::encodingToString(IntegerEncoding::unspecified),
             QStringLiteral(""));
}

void AcnNodeReconstructingVisitorTests::test_realEncodingToString()
{
    using namespace Data::Types;
    QCOMPARE(Real::encodingToString(RealEncoding::IEEE754_1985_32),
             QStringLiteral("IEEE754-1985-32"));
    QCOMPARE(Real::encodingToString(RealEncoding::IEEE754_1985_64),
             QStringLiteral("IEEE754-1985-64"));
    QCOMPARE(IntegerAcnParameters::encodingToString(IntegerEncoding::unspecified),
             QStringLiteral(""));
}

void AcnNodeReconstructingVisitorTests::test_asciiStringEncodingToString()
{
    using namespace Data::Types;
    QCOMPARE(AsciiStringAcnParameters::encodingToString(AsciiStringEncoding::ASCII),
             QStringLiteral("ASCII"));
    QCOMPARE(IntegerAcnParameters::encodingToString(IntegerEncoding::unspecified),
             QStringLiteral(""));
}

void AcnNodeReconstructingVisitorTests::performTest(const QString &name,
                                                    const QString &expectedResult,
                                                    std::unique_ptr<Data::Types::Type> type) const
{
    auto assignment = std::make_unique<Data::TypeAssignment>(name,
                                                             Data::SourceLocation(),
                                                             std::move(type));

    auto actualResult = restoreNode(*assignment);
    QCOMPARE(actualResult, expectedResult);
}

QString AcnNodeReconstructingVisitorTests::restoreNode(const Data::Node &node) const
{
    QString ret;
    QTextStream outStream(&ret);

    AcnNodeReconstructingVisitor visitor(outStream);
    node.accept(visitor);

    return ret;
}
