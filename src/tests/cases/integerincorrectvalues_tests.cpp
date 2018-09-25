/****************************************************************************
**
** Copyright (C) 2018 N7 Space sp. z o. o.
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
#include "integerincorrectvalues_tests.h"

#include <QtTest>

#include <cases/integerincorrectvalues.h>

#include <data/constraints/logicoperators.h>
#include <data/constraints/rangeconstraint.h>

using namespace Fuzzer::Cases::Tests;
using namespace Fuzzer::Data::Types;
using namespace Fuzzer::Data;

IntegerIncorrectValuesTests::IntegerIncorrectValuesTests(QObject *parent)
    : QObject(parent)
{}

void IntegerIncorrectValuesTests::test_noConstraints()
{
    Integer i;
    i.setSize(8);
    i.setEncoding(IntegerEncoding::pos_int);

    const auto v = IntegerIncorrectValues(i).items();

    QVERIFY(v.isEmpty());
}

void IntegerIncorrectValuesTests::test_rangeMatchingSize()
{
    Integer i;
    i.setSize(8);
    i.setEncoding(IntegerEncoding::pos_int);
    i.constraints().append({0, 255});

    const auto v = IntegerIncorrectValues(i).items();

    QVERIFY(v.isEmpty());
}

void IntegerIncorrectValuesTests::test_complexConstraint()
{
    Integer i;
    i.setSize(8);
    i.setEncoding(IntegerEncoding::pos_int);
    auto left = Constraints::RangeConstraint<IntegerValue>::create({1, 10});
    auto right = Constraints::RangeConstraint<IntegerValue>::create({100, 180});
    auto constraint = std::make_unique<Constraints::OrConstraint<IntegerValue>>(std::move(left),
                                                                                std::move(right));
    i.constraints().append(std::move(constraint));

    const auto v = IntegerIncorrectValues(i).items();

    QCOMPARE(v, (QList<int>{0, 11, 55, 99, 181, 218, 255}));
}

void IntegerIncorrectValuesTests::test_constraintList()
{
    Integer i;
    i.setSize(8);
    i.setEncoding(IntegerEncoding::pos_int);
    i.constraints().append({1, 100});
    i.constraints().append({1, 10});

    const auto v = IntegerIncorrectValues(i).items();

    QCOMPARE(v, (QList<int>{0, 11, 133, 255}));
}
