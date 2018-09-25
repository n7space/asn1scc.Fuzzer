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
#include "integerranges_tests.h"

#include <cstdint>
#include <limits>

#include <QtTest>

#include <cases/integerranges.h>

using namespace Fuzzer::Cases::Tests;
using namespace Fuzzer::Data::Types;

IntegerRangesTests::IntegerRangesTests(QObject *parent)
    : QObject(parent)
{}

void IntegerRangesTests::test_uint8()
{
    IntegerAcnParameters p;
    p.setSize(8);
    p.setEncoding(IntegerEncoding::pos_int);

    const auto r = maxValueRangeFor(p);

    QCOMPARE(r.begin(), static_cast<int>(std::numeric_limits<std::uint8_t>::min()));
    QCOMPARE(r.end(), static_cast<int>(std::numeric_limits<std::uint8_t>::max()));
}

void IntegerRangesTests::test_int16()
{
    IntegerAcnParameters p;
    p.setSize(16);
    p.setEncoding(IntegerEncoding::twos_complement);

    const auto r = maxValueRangeFor(p);

    QCOMPARE(r.begin(), static_cast<int>(std::numeric_limits<std::int16_t>::min()));
    QCOMPARE(r.end(), static_cast<int>(std::numeric_limits<std::int16_t>::max()));
}

void IntegerRangesTests::test_ascii()
{
    IntegerAcnParameters p;
    p.setSize(80);
    p.setEncoding(IntegerEncoding::ASCII);

    const auto r = maxValueRangeFor(p);

    QCOMPARE(r.begin(), -999999999);
    QCOMPARE(r.end(), +999999999);
}

void IntegerRangesTests::test_bcd()
{
    IntegerAcnParameters p;
    p.setSize(20);
    p.setEncoding(IntegerEncoding::BCD);

    const auto r = maxValueRangeFor(p);

    QCOMPARE(r.begin(), 0);
    QCOMPARE(r.end(), 99999);
}

void IntegerRangesTests::test_unspecified()
{
    IntegerAcnParameters p;
    p.setAcnMaxSizeInBits(4);
    p.setEncoding(IntegerEncoding::unspecified);

    const auto r = maxValueRangeFor(p);

    QCOMPARE(r.begin(), 0);
    QCOMPARE(r.end(), 15);
}
