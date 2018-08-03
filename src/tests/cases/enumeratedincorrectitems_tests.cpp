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
#include "enumeratedincorrectitems_tests.h"

#include <QtTest>

#include <cases/enumeratedincorrectitems.h>

#include <data/constraints/logicoperators.h>
#include <data/constraints/rangeconstraint.h>

using namespace MalTester::Cases::Tests;
using namespace MalTester::Data::Types;
using namespace MalTester::Data;

EnumeratedIncorrectItemsTests::EnumeratedIncorrectItemsTests(QObject *parent)
    : QObject(parent)
{}

void EnumeratedIncorrectItemsTests::test_itemsMatchingSize()
{
    Enumerated e;
    e.setSize(1);
    e.setEncoding(IntegerEncoding::pos_int);
    e.setEncodeValues(false);
    e.addItem({0, "item1", 10});
    e.addItem({1, "item2", 11});

    const auto r = EnumeratedIncorrectItems(e).items();

    QVERIFY(r.isEmpty());
}

void EnumeratedIncorrectItemsTests::test_additionalItemsArePossible()
{
    Enumerated e;
    e.setSize(2);
    e.setEncoding(IntegerEncoding::pos_int);
    e.setEncodeValues(false);
    e.addItem({0, "item1", 10});
    e.addItem({1, "item2", 11});
    e.addItem({2, "item3", 12});

    const auto r = EnumeratedIncorrectItems(e).items();

    QCOMPARE(r.size(), 1);
    QCOMPARE(r.at(0).index(), 3);
}

void EnumeratedIncorrectItemsTests::test_encodeValues()
{
    Enumerated e;
    e.setSize(2);
    e.setEncoding(IntegerEncoding::pos_int);
    e.setEncodeValues(true);
    e.addItem({0, "item1", 3});
    e.addItem({1, "item2", 2});
    e.addItem({2, "item3", 1});

    const auto r = EnumeratedIncorrectItems(e).items();

    QCOMPARE(r.size(), 1);
    QCOMPARE(r.at(0).value(), 0LL);
}

void EnumeratedIncorrectItemsTests::test_incorrectValuesOverlapWithIndexes()
{
    Enumerated e;
    e.setSize(2);
    e.setEncoding(IntegerEncoding::pos_int);
    e.setEncodeValues(true);
    e.addItem({0, "item1", 1});
    e.addItem({1, "item2", 3});

    const auto r = EnumeratedIncorrectItems(e).items();

    QCOMPARE(r.size(), 2);
    QCOMPARE(r.at(0).value(), 0LL);
    QCOMPARE(r.at(1).value(), 2LL);
}

void EnumeratedIncorrectItemsTests::test_enumWithConstraints()
{
    Enumerated e;
    e.setSize(2);
    e.setEncoding(IntegerEncoding::pos_int);
    e.setEncodeValues(true);
    e.addItem({0, "item1", 1});
    e.addItem({1, "item2", 3});
    e.constraints().append(std::make_unique<Constraints::OrConstraint<EnumValue>>(
        Constraints::RangeConstraint<EnumValue>::create(QStringLiteral("item1")),
        Constraints::RangeConstraint<EnumValue>::create(QStringLiteral("item2"))));

    const auto r = EnumeratedIncorrectItems(e).items();

    QCOMPARE(r.size(), 2);
    QCOMPARE(r.at(0).value(), 0LL);
    QCOMPARE(r.at(1).value(), 2LL);
}
