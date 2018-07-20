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
#include "testcaseprinter_tests.h"

#include <QtTest>

#include <cases/testcaseprinter.h>

using namespace MalTester::Cases::Tests;
using namespace MalTester::Cases;

TestCasePrinterTests::TestCasePrinterTests(QObject *parent)
    : QObject(parent)
{}

static QString print(const TestCase &c)
{
    QString s;
    QTextStream str(&s);

    TestCasePrinter p(str);
    p.print("MySeq", c);

    return s;
}

void TestCasePrinterTests::test_emptyPath()
{
    TestCase c{"Name"};
    c.setAssignment({{}, {"1410"}});

    const auto result = print(c);

    QCOMPARE(result,
             QStringLiteral("static int test_Name(MySeq *v, BitStream *stream)\n"
                            "{\n"
                            "  MySeq_Initialize(v);\n"
                            "\n"
                            "  *v = 1410;\n"
                            "\n"
                            "  MySeq_encode(v, stream);\n"
                            "  return (validate(stream) == TRUE) ? 0 : 1;\n"
                            "}\n"));
}

void TestCasePrinterTests::test_longPath()
{
    TestCase c{"Name"};
    c.setAssignment({{QStringLiteral("a"), QStringLiteral("b")}, {"1410"}});

    const auto result = print(c);

    QCOMPARE(result,
             QStringLiteral("static int test_Name(MySeq *v, BitStream *stream)\n"
                            "{\n"
                            "  MySeq_Initialize(v);\n"
                            "\n"
                            "  v->a.b = 1410;\n"
                            "\n"
                            "  MySeq_encode(v, stream);\n"
                            "  return (validate(stream) == TRUE) ? 0 : 1;\n"
                            "}\n"));
}
