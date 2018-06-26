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
#include "logicoperators_tests.h"

#include <QtTest>

#include <data/values.h>

#include <data/constraints/logicoperators.h>
#include <data/constraints/rangeconstraintliteral.h>

using namespace MalTester::Data::Constraints::Tests;
using namespace MalTester::Data::Constraints;
using namespace MalTester::Data;

LogicOperatorsTests::LogicOperatorsTests(QObject *parent)
    : QObject(parent)
{}

void LogicOperatorsTests::test_andAsString()
{
    auto l = std::make_unique<RangeConstraintLiteral<IntegerValue>>(Range<int>{10, 20});
    auto r = std::make_unique<RangeConstraintLiteral<IntegerValue>>(Range<int>{50, 60});

    AndConstraint<IntegerValue> a(std::move(l), std::move(r));

    QCOMPARE(a.asString(), QLatin1Literal("(10 .. 20 ^ 50 .. 60)"));
}

void LogicOperatorsTests::test_orAsString()
{
    auto l = std::make_unique<RangeConstraintLiteral<IntegerValue>>(Range<int>{10, 20});
    auto r = std::make_unique<RangeConstraintLiteral<IntegerValue>>(Range<int>{50, 60});

    OrConstraint<IntegerValue> o(std::move(l), std::move(r));

    QCOMPARE(o.asString(), QLatin1Literal("(10 .. 20 | 50 .. 60)"));
}
