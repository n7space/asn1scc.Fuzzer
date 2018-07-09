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
#include "integerincorrectvalues_tests.h"

#include <QtTest>

#include <cases/integerincorrectvalues.h>

#include <data/constraints/rangeconstraint.h>

using namespace MalTester::Cases::Tests;
using namespace MalTester::Data::Types;
using namespace MalTester::Data;

using RangeConstraint = Constraints::RangeConstraint<IntegerValue>;
namespace {
std::unique_ptr<RangeConstraint> makeRange(int b, int e)
{
    return std::make_unique<RangeConstraint>(Range<int>(b, e));
}
} // namespace

IntegerIncorrectValuesTests::IntegerIncorrectValuesTests(QObject *parent)
    : QObject(parent)
{}

void IntegerIncorrectValuesTests::test_empty()
{
    Integer i;
    i.setSize(8);
    i.setEncoding(IntegerEncoding::pos_int);
    i.constraints().append(makeRange(0, 255));

    const auto v = IntegerIncorrectValues(i).items();

    QVERIFY(v.isEmpty());
}
void IntegerIncorrectValuesTests::test_values()
{
    Integer i;
    i.setSize(8);
    i.setEncoding(IntegerEncoding::pos_int);
    i.constraints().append(makeRange(1, 10));
    i.constraints().append(makeRange(100, 180));

    const auto v = IntegerIncorrectValues(i).items();

    QCOMPARE(v, (QList<int>{0, 11, 55, 99, 181, 218, 255}));
}
