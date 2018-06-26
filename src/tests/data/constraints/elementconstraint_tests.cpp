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
#include "elementconstraint_tests.h"

#include <QtTest>

#include <data/values.h>

#include <data/constraints/elementconstraint.h>
#include <data/constraints/rangeconstraintliteral.h>

using namespace MalTester::Data::Constraints::Tests;
using namespace MalTester::Data::Constraints;
using namespace MalTester::Data;

ElementConstraintTests::ElementConstraintTests(QObject *parent)
    : QObject(parent)
{}

void ElementConstraintTests::test_asString()
{
    auto c = std::make_unique<RangeConstraintLiteral<IntegerValue>>(Range<int>{10, 20});

    ElementConstraint<IntegerValue> a(std::move(c));

    QCOMPARE(a.asString(), QLatin1Literal("FROM(10 .. 20)"));
}
