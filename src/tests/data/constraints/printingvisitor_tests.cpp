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
#include "printingvisitor_tests.h"

#include <QtTest>

#include <data/values.h>

#include <data/constraints/printingvisitor.h>

using namespace MalTester::Data::Constraints::Tests;
using namespace MalTester::Data::Constraints;
using namespace MalTester::Data;

PrintingVisitorTests::PrintingVisitorTests(QObject *parent)
    : QObject(parent)
{}

void PrintingVisitorTests::test_fromConstraintToString()
{
    auto c = std::make_unique<RangeConstraint<StringValue>>(Range<QString>{"a", "z"});

    FromConstraint<IntegerValue> a(std::move(c));

    QCOMPARE(toString(a), QLatin1Literal(R"(FROM("a" .. "z"))"));
}

void PrintingVisitorTests::test_sizeConstraintToString()
{
    auto c = std::make_unique<RangeConstraint<IntegerValue>>(Range<int>{1, 10});

    SizeConstraint<IntegerValue> a(std::move(c));

    QCOMPARE(toString(a), QLatin1Literal(R"(SIZE(1 .. 10))"));
}

void PrintingVisitorTests::test_rangeConstraintToString()
{
    QCOMPARE(toString(RangeConstraint<Data::IntegerValue>(Range<int>(10))), QStringLiteral("10"));
    QCOMPARE(toString(RangeConstraint<Data::IntegerValue>(Range<int>(10, 20))),
             QStringLiteral("10 .. 20"));

    QCOMPARE(toString(RangeConstraint<Data::StringValue>(Range<QString>("abc"))),
             QStringLiteral(R"("abc")"));
    QCOMPARE(toString(RangeConstraint<Data::EnumValue>(Range<QString>("A", "Z"))),
             QStringLiteral(R"(A .. Z)"));
}

void PrintingVisitorTests::test_andConstraintToString()
{
    auto l = std::make_unique<RangeConstraint<IntegerValue>>(Range<int>{10, 20});
    auto r = std::make_unique<RangeConstraint<IntegerValue>>(Range<int>{50, 60});

    AndConstraint<IntegerValue> a(std::move(l), std::move(r));

    QCOMPARE(toString(a), QLatin1Literal("(10 .. 20 ^ 50 .. 60)"));
}

void PrintingVisitorTests::test_orConstraintToString()
{
    auto l = std::make_unique<RangeConstraint<IntegerValue>>(Range<int>{10, 20});
    auto r = std::make_unique<RangeConstraint<IntegerValue>>(Range<int>{50, 60});

    OrConstraint<IntegerValue> o(std::move(l), std::move(r));

    QCOMPARE(toString(o), QLatin1Literal("(10 .. 20 | 50 .. 60)"));
}

void PrintingVisitorTests::test_listToString()
{
    ConstraintList<IntegerValue> l;

    l.append(std::make_unique<RangeConstraint<IntegerValue>>(Range<int>{10, 20}));
    l.append(std::make_unique<RangeConstraint<IntegerValue>>(Range<int>{100, 200}));

    QCOMPARE(toString(l), QLatin1Literal("(10 .. 20) (100 .. 200)"));
}
