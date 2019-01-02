/****************************************************************************
**
** Copyright (C) 2018-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Fuzzer - Tool for generating test cases
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
#include "astxmlconstraintparser_tests.h"

#include <QtTest>

#include <data/constraints/printingvisitor.h>

using namespace Fuzzer::Tests;
using namespace Fuzzer;

AstXmlConstraintParserTests::AstXmlConstraintParserTests(QObject *parent)
    : QObject(parent)
{}

void AstXmlConstraintParserTests::test_emptyXml()
{
    parse<Data::IntegerValue>("");

    QVERIFY(m_dumpedConstraint.isEmpty());
}

void AstXmlConstraintParserTests::test_simpleIntegerValue()
{
    parse<Data::IntegerValue>("<IntegerValue>1410</IntegerValue>");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal("(1410)"));
}

void AstXmlConstraintParserTests::test_simpleRealValue()
{
    parse<Data::RealValue>("<RealValue>2.5</RealValue>");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal("(2.5)"));
}

void AstXmlConstraintParserTests::test_simpleBooleanValue()
{
    parse<Data::BooleanValue>("<BooleanValue>true</BooleanValue>");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal("(TRUE)"));
}

void AstXmlConstraintParserTests::test_simpleEnumValue()
{
    parse<Data::EnumValue>("<EnumValue>x</EnumValue>");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal("(x)"));
}

void AstXmlConstraintParserTests::test_simpleStringValue()
{
    parse<Data::StringValue>("<StringValue>abc</StringValue>");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal(R"(("abc"))"));
}

void AstXmlConstraintParserTests::test_simpleBitStringValue()
{
    parse<Data::BitStringValue>("<BitStringValue>0101</BitStringValue>");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal("('0101'B)"));
}

void AstXmlConstraintParserTests::test_simpleOctetStringValue()
{
    parse<Data::OctetStringValue>("<OctetStringValue>10</OctetStringValue>");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal("('10'H)"));
}

void AstXmlConstraintParserTests::test_range()
{
    parse<Data::IntegerValue>(R"(
                              <Range>
                                <Min>
                                  <IntegerValue>12</IntegerValue>
                                </Min>
                                <Max>
                                  <IntegerValue>34</IntegerValue>
                                </Max>
                              </Range>
                              )");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal("(12 .. 34)"));
}

void AstXmlConstraintParserTests::test_rangeWithoutMinOrMax()
{
    parsingFails<Data::IntegerValue>(R"(
                              <Range>
                                <Min>
                                  <IntegerValue>12</IntegerValue>
                                </Min>
                              </Range>
                              )");

    parsingFails<Data::IntegerValue>(R"(
                              <Range>
                                <Min>
                                </Min>
                                <Max>
                                  <IntegerValue>34</IntegerValue>
                                </Max>
                              </Range>
                              )");
}

void AstXmlConstraintParserTests::test_multipleValues()
{
    parse<Data::IntegerValue>(R"(<Constraints>
                                    <IntegerValue>12</IntegerValue>
                                    <IntegerValue>22</IntegerValue>
                                 </Constraints>)");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal("(12) (22)"));
}

void AstXmlConstraintParserTests::test_or()
{
    parse<Data::EnumValue>(R"(
                           <OR>
                                <EnumValue>a</EnumValue>
                                <EnumValue>b</EnumValue>
                           </OR>
                           )");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal("((a | b))"));
}

void AstXmlConstraintParserTests::test_and()
{
    parse<Data::EnumValue>(R"(
                           <AND>
                                <EnumValue>a</EnumValue>
                                <EnumValue>b</EnumValue>
                           </AND>
                           )");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal("((a ^ b))"));
}

void AstXmlConstraintParserTests::test_nestedLogicalOperators()
{
    parse<Data::EnumValue>(R"(
                           <AND>
                                <EnumValue>a</EnumValue>
                                <OR>
                                    <EnumValue>b</EnumValue>
                                    <EnumValue>c</EnumValue>
                                </OR>
                           </AND>
                           )");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal("((a ^ (b | c)))"));
}

void AstXmlConstraintParserTests::test_from()
{
    parse<Data::StringValue>(R"(
                             <ALPHA>
                                <Range>
                                    <Min><StringValue>A</StringValue></Min>
                                    <Max><StringValue>Z</StringValue></Max>
                                </Range>
                             </ALPHA>
                           )");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal(R"((FROM("A" .. "Z")))"));
}

void AstXmlConstraintParserTests::test_size()
{
    parse<Data::StringValue>(R"(
                             <SIZE>
                                <Range>
                                    <Min><IntegerValue>1</IntegerValue></Min>
                                    <Max><IntegerValue>5</IntegerValue></Max>
                                </Range>
                             </SIZE>
                           )");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal("(SIZE(1 .. 5))"));
}

void AstXmlConstraintParserTests::test_mixed()
{
    parse<Data::StringValue>(R"(<Constraints>
                             <ALPHA>
                                <Range>
                                    <Min><StringValue>A</StringValue></Min>
                                    <Max><StringValue>Z</StringValue></Max>
                                </Range>
                             </ALPHA>
                             <SIZE>
                                <Range>
                                    <Min><IntegerValue>1</IntegerValue></Min>
                                    <Max><IntegerValue>5</IntegerValue></Max>
                                </Range>
                             </SIZE>
                             </Constraints>
                           )");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal(R"((FROM("A" .. "Z")) (SIZE(1 .. 5)))"));
}

void AstXmlConstraintParserTests::test_mixedInReverseOrder()
{
    parse<Data::StringValue>(R"(<Constraints>
                             <SIZE>
                                <Range>
                                    <Min><IntegerValue>1</IntegerValue></Min>
                                    <Max><IntegerValue>5</IntegerValue></Max>
                                </Range>
                             </SIZE>
                             <ALPHA>
                                <Range>
                                    <Min><StringValue>A</StringValue></Min>
                                    <Max><StringValue>Z</StringValue></Max>
                                </Range>
                             </ALPHA>
                             </Constraints>
                           )");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal(R"((SIZE(1 .. 5)) (FROM("A" .. "Z")))"));
}

void AstXmlConstraintParserTests::test_complex()
{
    parse<Data::StringValue>(R"(
                             <OR>
                                <AND>
                                    <SIZE>
                                        <Range>
                                            <Min><IntegerValue>1</IntegerValue></Min>
                                            <Max><IntegerValue>5</IntegerValue></Max>
                                        </Range>
                                    </SIZE>
                                    <ALPHA>
                                        <Range>
                                            <Min><StringValue>A</StringValue></Min>
                                            <Max><StringValue>Z</StringValue></Max>
                                        </Range>
                                    </ALPHA>
                                </AND>
                                <StringValue>ZXY</StringValue>
                             </OR>
                           )");

    QCOMPARE(m_dumpedConstraint, QLatin1Literal(R"((((SIZE(1 .. 5) ^ FROM("A" .. "Z")) | "ZXY")))"));
}

template<typename T>
void AstXmlConstraintParserTests::parse(const QString &xmlData)
{
    QXmlStreamReader xmlReader(xmlData);
    Data::Constraints::ConstraintList<T> list;
    AstXmlConstraintParser<T> parser(xmlReader, list);
    QVERIFY(parser.parse());
    m_dumpedConstraint = toString(list);
}

template<typename T>
void AstXmlConstraintParserTests::parsingFails(const QString &xmlData)
{
    QXmlStreamReader xmlReader(xmlData);
    Data::Constraints::ConstraintList<T> list;
    AstXmlConstraintParser<T> parser(xmlReader, list);
    QVERIFY(!parser.parse());
}
