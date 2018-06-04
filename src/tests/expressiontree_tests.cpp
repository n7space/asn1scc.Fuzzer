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
#include <data/ranges.h>

using namespace MalTester;
using namespace MalTester::Tests;

using Tree = Data::ExpressionTree::ExpressionTree<Data::Range *>;

ExpressionTreeTests::ExpressionTreeTests(QObject *parent)
    : QObject(parent)
{}

void ExpressionTreeTests::test_emptyTree()
{
    Tree tree;
    QCOMPARE(tree.expression(), QString());
}

void ExpressionTreeTests::test_singleValue()
{
    Tree tree;
    tree.addRange(new Data::IntegerRange(1, 1));

    QCOMPARE(tree.expression(), QStringLiteral("(1)"));
}

void ExpressionTreeTests::test_alternativesExpression()
{
    Tree tree;
    tree.addOperator(QStringLiteral("OR"));
    tree.addOperator(QStringLiteral("OR"));
    tree.addOperator(QStringLiteral("OR"));
    tree.addOperator(QStringLiteral("OR"));

    tree.addRange(new Data::IntegerRange(1, 1));
    tree.addRange(new Data::IntegerRange(2, 3));
    tree.addRange(new Data::IntegerRange(4, 5));
    tree.addRange(new Data::IntegerRange(6, 7));
    tree.addRange(new Data::IntegerRange(8, 8));

    QCOMPARE(tree.expression(), QStringLiteral("(((((1 | 2 .. 3) | 4 .. 5) | 6 .. 7) | 8))"));
}

void ExpressionTreeTests::test_sumsExpression()
{
    Tree tree;
    tree.addOperator(QStringLiteral("AND"));
    tree.addOperator(QStringLiteral("AND"));
    tree.addOperator(QStringLiteral("AND"));
    tree.addOperator(QStringLiteral("AND"));

    tree.addRange(new Data::IntegerRange(1, 100));
    tree.addRange(new Data::IntegerRange(1, 90));
    tree.addRange(new Data::IntegerRange(1, 80));
    tree.addRange(new Data::IntegerRange(1, 70));
    tree.addRange(new Data::IntegerRange(1, 1));

    QCOMPARE(tree.expression(),
             QStringLiteral("(((((1 .. 100 ^ 1 .. 90) ^ 1 .. 80) ^ 1 .. 70) ^ 1))"));
}

void ExpressionTreeTests::test_alternativesAndSumsExpression()
{
    Tree tree;
    tree.addOperator(QStringLiteral("OR"));
    tree.addOperator(QStringLiteral("AND"));
    tree.addOperator(QStringLiteral("OR"));
    tree.addOperator(QStringLiteral("AND"));

    tree.addRange(new Data::IntegerRange(1, 100));
    tree.addRange(new Data::IntegerRange(1, 90));

    tree.addOperator(QStringLiteral("AND"));
    tree.addRange(new Data::IntegerRange(1, 80));
    tree.addRange(new Data::IntegerRange(1, 70));

    tree.addOperator(QStringLiteral("AND"));
    tree.addRange(new Data::IntegerRange(1, 60));
    tree.addRange(new Data::IntegerRange(1, 50));

    tree.addRange(new Data::IntegerRange(100, 100));

    QCOMPARE(tree.expression(),
             QStringLiteral(
                 "(((((1 .. 100 ^ 1 .. 90) | (1 .. 80 ^ 1 .. 70)) ^ (1 .. 60 ^ 1 .. 50)) | 100))"));
}

void ExpressionTreeTests::test_sizeExpression()
{
    Tree tree;
    tree.addOperator(QStringLiteral("SIZE"));
    tree.addRange(new Data::IntegerRange(1, 30));

    QCOMPARE(tree.expression(), QStringLiteral("((SIZE (1 .. 30)))"));
}

void ExpressionTreeTests::test_sizeAndAlphabetExpression()
{
    Tree tree;
    tree.addOperator(QStringLiteral("SIZE"));
    tree.addRange(new Data::IntegerRange(20, 20));

    tree.addOperator(QStringLiteral("ALPHA"));
    tree.addOperator(QStringLiteral("OR"));
    tree.addRange(new Data::StringRange("A", "Z"));
    tree.addRange(new Data::StringRange("1", "5"));

    QCOMPARE(tree.expression(),
             QStringLiteral("((SIZE (20)))((FROM ((\"A\" .. \"Z\" | \"1\" .. \"5\"))))"));
}

void ExpressionTreeTests::test_logicalAndConstrainingOperators()
{
    Tree tree;

    tree.addOperator(QStringLiteral("OR"));
    tree.addOperator(QStringLiteral("SIZE"));
    tree.addOperator(QStringLiteral("OR"));
    tree.addOperator(QStringLiteral("OR"));

    tree.addRange(new Data::IntegerRange(1, 2));
    tree.addRange(new Data::IntegerRange(4, 5));
    tree.addRange(new Data::IntegerRange(7, 8));

    tree.addOperator(QStringLiteral("AND"));
    tree.addOperator(QStringLiteral("SIZE"));
    tree.addOperator(QStringLiteral("OR"));

    tree.addRange(new Data::IntegerRange(30, 50));
    tree.addRange(new Data::IntegerRange(100, 150));

    tree.addOperator(QStringLiteral("ALPHA"));
    tree.addOperator(QStringLiteral("OR"));
    tree.addOperator(QStringLiteral("OR"));

    tree.addRange(new Data::StringRange("A", "Z"));
    tree.addRange(new Data::StringRange("a", "z"));
    tree.addRange(new Data::StringRange("1", "1"));

    QCOMPARE(
        tree.expression(),
        QStringLiteral(
            "(((SIZE (((1 .. 2 | 4 .. 5) | 7 .. 8))) | ((SIZE ((30 .. 50 | 100 .. 150))) ^ (FROM (((\"A\" .. \"Z\" | \"a\" .. \"z\") | \"1\"))))))"));
}
