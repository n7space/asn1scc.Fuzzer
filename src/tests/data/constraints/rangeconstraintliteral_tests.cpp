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
#include "rangeconstraintliteral_tests.h"

#include <QtTest>

#include <data/constraints/rangeconstraintliteral.h>

#include <data/values.h>

using namespace Fuzzer::Data::Constraints::Tests;

RangeContraintLiteralTests::RangeContraintLiteralTests(QObject *parent)
    : QObject(parent)
{}

void RangeContraintLiteralTests::test_asString()
{
    QCOMPARE(RangeConstraintLiteral<Data::IntegerValue>(Range<int>(10)).asString(),
             QStringLiteral("10"));
    QCOMPARE(RangeConstraintLiteral<Data::IntegerValue>(Range<int>(10, 20)).asString(),
             QStringLiteral("10 .. 20"));

    QCOMPARE(RangeConstraintLiteral<Data::StringValue>(Range<QString>("abc")).asString(),
             QStringLiteral(R"("abc")"));
    QCOMPARE(RangeConstraintLiteral<Data::EnumValue>(Range<QString>("A", "Z")).asString(),
             QStringLiteral(R"(A .. Z)"));
}
