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
#include "range_tests.h"

#include <QtTest>

#include <data/range.h>

using namespace MalTester::Data::Tests;

RangeTests::RangeTests(QObject *parent)
    : QObject(parent)
{}

void RangeTests::test_intersects()
{
    const int begin = 10;
    const int end = 15;

    const auto range = Range<int>{begin, end};

    QVERIFY(range.intersects(range));

    QVERIFY(range.intersects(Range<int>{begin}));
    QVERIFY(range.intersects(Range<int>{end}));

    QVERIFY(range.intersects({end, end + 1}));
    QVERIFY(range.intersects({begin - 1, begin}));

    QVERIFY(range.intersects({end - 1, end + 1}));
    QVERIFY(range.intersects({begin - 1, begin + 1}));

    QVERIFY(!range.intersects(Range<int>{begin - 1}));
    QVERIFY(!range.intersects(Range<int>{end + 1}));

    QVERIFY(!range.intersects({end + 1, end + 2}));
    QVERIFY(!range.intersects({begin - 2, begin - 1}));
}

void RangeTests::test_canMerge()
{
    const int begin = 10;
    const int end = 15;

    const auto range = Range<int>{begin, end};

    QVERIFY(range.canMerge(Range<int>{end + 1}));
    QVERIFY(!range.canMerge(Range<int>{end + 2}));

    QVERIFY(range.canMerge(Range<int>{begin - 1}));
    QVERIFY(!range.canMerge(Range<int>{begin - 2}));

    QVERIFY(range.canMerge(Range<int>(begin - 10, begin - 1)));
    QVERIFY(!range.canMerge(Range<int>(begin - 10, begin - 2)));

    QVERIFY(range.canMerge(Range<int>(end + 1, end + 10)));
    QVERIFY(!range.canMerge(Range<int>(end + 2, end + 10)));
}

void RangeTests::test_merge()
{
    const int begin = 10;
    const int end = 15;

    const auto range = Range<int>{begin, end};

    QCOMPARE(range.merge(Range<int>{begin + 1}), range);
    QCOMPARE(range.merge(Range<int>{end - 1}), range);
    QCOMPARE(range.merge(Range<int>(begin + 1, end - 1)), range);

    QCOMPARE(range.merge(Range<int>(begin - 1, end - 1)), Range<int>(begin - 1, end));
    QCOMPARE(range.merge(Range<int>(begin + 1, end + 1)), Range<int>(begin, end + 1));
    QCOMPARE(range.merge(Range<int>(begin - 1, end + 1)), Range<int>(begin - 1, end + 1));

    QCOMPARE(range.merge(Range<int>{begin - 1}), Range<int>(begin - 1, end));
    QCOMPARE(range.merge(Range<int>{end + 1}), Range<int>(begin, end + 1));
    QCOMPARE(range.merge(Range<int>(begin - 10, begin - 1)), Range<int>(begin - 10, end));
}

void RangeTests::test_intersection()
{
    const int begin = 10;
    const int end = 15;

    const auto range = Range<int>{begin, end};

    QCOMPARE(range.intersection(Range<int>{begin + 1}), Range<int>(begin + 1));
    QCOMPARE(range.intersection(Range<int>{end - 1}), Range<int>(end - 1));

    QCOMPARE(range.intersection(Range<int>(begin - 1, end + 1)), range);

    QCOMPARE(range.intersection(Range<int>(begin - 1, end - 1)), Range<int>(begin, end - 1));
    QCOMPARE(range.intersection(Range<int>(begin + 1, end + 1)), Range<int>(begin + 1, end));
    QCOMPARE(range.intersection(Range<int>(begin + 1, end - 1)), Range<int>(begin + 1, end - 1));
}

void RangeTests::test_difference()
{
    const int begin = 10;
    const int end = 15;

    const auto range = Range<int>{begin, end};

    QVERIFY(range.difference(range).isEmpty());

    QCOMPARE(range.difference(Range<int>{begin - 100, end - 100}), QList<Range<int>>{range});
    QCOMPARE(range.difference(Range<int>{begin + 100, end + 100}), QList<Range<int>>{range});

    QCOMPARE(range.difference(Range<int>{begin, begin + 1}), (QList<Range<int>>{{begin + 1, end}}));
    QCOMPARE(range.difference(Range<int>{end - 1, end}), (QList<Range<int>>{{begin, end - 1}}));

    QCOMPARE(range.difference(Range<int>{begin + 1, end - 1}),
             (QList<Range<int>>{{begin, begin + 1}, {end - 1, end}}));
}
