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
#include "expressiontree_tests.h"

#include <QtTest>

#include <data/expressiontree/expressiontree.h>
#include <data/expressiontree/ranges.h>

using namespace MalTester;
using namespace MalTester::Tests;
using namespace MalTester::Data::ExpressionTree;

ExpressionTreeTests::ExpressionTreeTests(QObject *parent)
    : QObject(parent)
{}

void ExpressionTreeTests::test_emptyTree()
{
    ExpressionTree tree;

    QCOMPARE(tree.expression(), QString());
}

void ExpressionTreeTests::test_singleValue()
{
    ExpressionTree tree;

    tree.appendSubtree(new Data::ExpressionTree::IntegerRange(1, 1));

    QCOMPARE(tree.expression(), QStringLiteral("(1)"));
}

void ExpressionTreeTests::test_alternativesExpression()
{
    ExpressionTree tree;

    // clang-format off
    tree.appendSubtree(new LogicOperator(QStringLiteral("OR"),
                                         new LogicOperator(QStringLiteral("OR"),
                                                           new LogicOperator(QStringLiteral("OR"),
                                                                             new LogicOperator(QStringLiteral("OR"),
                                                                                               new IntegerRange(1, 1),
                                                                                               new IntegerRange(2, 3)),
                                                                             new IntegerRange(4, 5)),
                                                           new IntegerRange(6, 7)),
                                         new IntegerRange(8, 8)));
    // clang-format on

    QCOMPARE(tree.expression(), QStringLiteral("(((((1 | 2 .. 3) | 4 .. 5) | 6 .. 7) | 8))"));
}

void ExpressionTreeTests::test_sumsExpression()
{
    ExpressionTree tree;

    // clang-format off
    tree.appendSubtree(new LogicOperator(QStringLiteral("AND"),
                                         new LogicOperator(QStringLiteral("AND"),
                                                           new LogicOperator(QStringLiteral("AND"),
                                                                             new LogicOperator(QStringLiteral("AND"),
                                                                                               new IntegerRange(1, 100),
                                                                                               new IntegerRange(1, 90)),
                                                                             new IntegerRange(1, 80)),
                                                           new IntegerRange(1, 70)),
                                         new IntegerRange(1, 1)));
    // clang-format on

    QCOMPARE(tree.expression(),
             QStringLiteral("(((((1 .. 100 ^ 1 .. 90) ^ 1 .. 80) ^ 1 .. 70) ^ 1))"));
}

void ExpressionTreeTests::test_alternativesAndSumsExpression()
{
    ExpressionTree tree;

    // clang-format off
    tree.appendSubtree(new LogicOperator(QStringLiteral("OR"),
                                         new LogicOperator(QStringLiteral("AND"),
                                                           new LogicOperator(QStringLiteral("OR"),
                                                                             new LogicOperator(QStringLiteral("AND"),
                                                                                               new IntegerRange(1, 100),
                                                                                               new IntegerRange(1, 90)),
                                                                             new LogicOperator(QStringLiteral("AND"),
                                                                                               new IntegerRange(1, 80),
                                                                                               new IntegerRange(1, 70))),
                                                           new LogicOperator(QStringLiteral("AND"),
                                                                             new IntegerRange(1, 60),
                                                                             new IntegerRange(1, 50))),
                                         new IntegerRange(100, 100)));
    // clang-format on

    QCOMPARE(tree.expression(),
             QStringLiteral(
                 "(((((1 .. 100 ^ 1 .. 90) | (1 .. 80 ^ 1 .. 70)) ^ (1 .. 60 ^ 1 .. 50)) | 100))"));
}

void ExpressionTreeTests::test_sizeExpression()
{
    ExpressionTree tree;

    // clang-format off
    tree.appendSubtree(new ConstrainingOperator(QStringLiteral("SIZE"),
                                                new IntegerRange(1, 30)));
    // clang-format on

    QCOMPARE(tree.expression(), QStringLiteral("((SIZE (1 .. 30)))"));
}

void ExpressionTreeTests::test_sizeAndAlphabetExpression()
{
    ExpressionTree tree;

    // clang-format off
    tree.appendSubtree(new ConstrainingOperator(QStringLiteral("SIZE"),
                                                new IntegerRange(20, 20)));

    tree.appendSubtree(new ConstrainingOperator(QStringLiteral("ALPHA"),
                                                new LogicOperator(QStringLiteral("OR"),
                                                                  new StringRange("A", "Z"),
                                                                  new StringRange("1", "5"))));
    // clang-format on

    QCOMPARE(tree.expression(),
             QStringLiteral("((SIZE (20)))((FROM ((\"A\" .. \"Z\" | \"1\" .. \"5\"))))"));
}

void ExpressionTreeTests::test_logicalAndConstrainingOperators()
{
    ExpressionTree tree;

    // clang-format off
    tree.appendSubtree(new LogicOperator(QStringLiteral("OR"),
                                         new ConstrainingOperator(QStringLiteral("SIZE"),
                                                                  new LogicOperator(QStringLiteral("OR"),
                                                                                    new LogicOperator(QStringLiteral("OR"),
                                                                                                      new IntegerRange(1, 2),
                                                                                                      new IntegerRange(4, 5)),
                                                                                    new IntegerRange(7, 8))),
                                         new LogicOperator(QStringLiteral("AND"),
                                                           new ConstrainingOperator(QStringLiteral("SIZE"),
                                                                                    new LogicOperator(QStringLiteral("OR"),
                                                                                                      new IntegerRange(30, 50),
                                                                                                      new IntegerRange(100, 150))),
                                                           new ConstrainingOperator(QStringLiteral("ALPHA"),
                                                                                    new LogicOperator(QStringLiteral("OR"),
                                                                                                      new LogicOperator(QStringLiteral("OR"),
                                                                                                                        new StringRange("A", "Z"),
                                                                                                                        new StringRange("a", "z")),
                                                                                                      new StringRange("1", "1"))))));
    // clang-format on

    QCOMPARE(
        tree.expression(),
        QStringLiteral(
            "(((SIZE (((1 .. 2 | 4 .. 5) | 7 .. 8))) | ((SIZE ((30 .. 50 | 100 .. 150))) ^ (FROM (((\"A\" .. \"Z\" | \"a\" .. \"z\") | \"1\"))))))"));
}
