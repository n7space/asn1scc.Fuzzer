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
#include "rangelist_tests.h"

#include <QtTest>

#include <data/rangelist.h>

using namespace MalTester::Data::Tests;
using namespace MalTester::Data;

RangeListTests::RangeListTests(QObject *parent)
    : QObject(parent)
{}

void RangeListTests::test_asString()
{
    QCOMPARE(RangeList<int>().asString(), QLatin1Literal(""));
    QCOMPARE(RangeList<int>({Range<int>(10)}).asString(), QLatin1Literal("10"));
    QCOMPARE(RangeList<int>({Range<int>(10), Range<int>(20)}).asString(), QLatin1Literal("10 | 20"));
    QCOMPARE(RangeList<int>({Range<int>(10), Range<int>(20, 30)}).asString(),
             QLatin1Literal("10 | 20 .. 30"));
}

namespace {

QString compact(RangeList<int> l)
{
    l.compact();
    return l.asString();
}

QString sort(RangeList<int> l)
{
    l.sort();
    return l.asString();
}

QString intersection(RangeList<int> l, Range<int> r)
{
    return l.intersection(r).asString();
}

QString intersect(RangeList<int> a, RangeList<int> b)
{
    a.intersect(b);
    return a.asString();
}

QString diff(Range<int> r, RangeList<int> l)
{
    return difference(r, l).asString();
}

} // namespace

void RangeListTests::test_sort()
{
    QCOMPARE(sort({}), QLatin1Literal(""));
    QCOMPARE(sort({Range<int>(1), Range<int>(3)}), QLatin1Literal("1 | 3"));
    QCOMPARE(sort({Range<int>(3), Range<int>(1), Range<int>(2)}), QLatin1Literal("1 | 2 | 3"));
}

void RangeListTests::test_compact()
{
    QCOMPARE(compact({}), QLatin1Literal(""));
    QCOMPARE(compact({Range<int>(1), Range<int>(3)}), QLatin1Literal("1 | 3"));
    QCOMPARE(compact({Range<int>(1), Range<int>(2)}), QLatin1Literal("1 .. 2"));
    QCOMPARE(compact({Range<int>(2), Range<int>(1), Range<int>(3)}), QLatin1Literal("1 .. 3"));
    QCOMPARE(compact({Range<int>(2), Range<int>(5), Range<int>(3)}), QLatin1Literal("2 .. 3 | 5"));
}

void RangeListTests::test_intersection()
{
    QCOMPARE(intersection({}, {10, 20}), QLatin1Literal(""));
    QCOMPARE(intersection({{10, 20}}, {10, 20}), QLatin1Literal("10 .. 20"));
    QCOMPARE(intersection({{10, 20}}, {12, 18}), QLatin1Literal("12 .. 18"));
    QCOMPARE(intersection({{10, 15}, {17, 20}}, {12, 18}), QLatin1Literal("12 .. 15 | 17 .. 18"));
}

void RangeListTests::test_intersect()
{
    QCOMPARE(intersect({}, {{10, 20}}), QLatin1Literal(""));
    QCOMPARE(intersect({{10, 20}}, {{10, 20}}), QLatin1Literal("10 .. 20"));
    QCOMPARE(intersect({{10, 20}}, {{12, 18}}), QLatin1Literal("12 .. 18"));
    QCOMPARE(intersect({{10, 15}, {17, 20}}, {{12, 18}}), QLatin1Literal("12 .. 15 | 17 .. 18"));
    QCOMPARE(intersect({{10, 15}, {17, 20}}, {{1, 2}}), QLatin1Literal(""));
    QCOMPARE(intersect({{10, 15}, {17, 20}}, {{12, 17}, {19, 20}}),
             QLatin1Literal("12 .. 15 | 17 | 19 .. 20"));
}

void RangeListTests::test_difference()
{
    QCOMPARE(diff({10, 15}, {{10, 15}}), QLatin1Literal(""));
    QCOMPARE(diff({10, 15}, {{100, 150}}), QLatin1Literal("10 .. 15"));
    QCOMPARE(diff({10, 15}, {{11, 20}, {25, 30}}), QLatin1Literal("10"));
    QCOMPARE(diff({10, 40}, {{15, 20}, {25, 30}}), QLatin1Literal("10 .. 14 | 21 .. 24 | 31 .. 40"));
    QCOMPARE(diff({0, 255}, {{1, 10}}), QLatin1Literal("0 | 11 .. 255"));
}
