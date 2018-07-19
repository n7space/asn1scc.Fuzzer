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
#include "testcaseprinter.h"

using namespace MalTester::Cases;

TestCasePrinter::TestCasePrinter(QTextStream &stream)
    : m_stream(stream)
{}

static QString asString(const FieldPath &p)
{
    if (p.isEmpty())
        return "*v";
    return "v->" + p.join('.');
}

void TestCasePrinter::print(const TestCase &test)
{
    m_stream << QStringLiteral("static void test_%1(%2 *v, BitStream *stream)\n"
                               "{\n"
                               "  BitStream_Init(stream, stream->buf, stream->count);\n"
                               "  %2_Initialize(v);\n"
                               "\n"
                               "  %3 = %4;\n"
                               "\n"
                               "  %2_encode(v, stream);\n"
                               "  validate(stream);\n"
                               "}\n")
                    .arg(test.name())
                    .arg(test.typeUnderTest())
                    .arg(asString(test.fieldAssignment().m_path))
                    .arg(test.fieldAssignment().m_value);
}

void TestCasePrinter::printAll(const QString &mainStructure, const QList<TestCase> &tests)
{
    printFileHeader(mainStructure);
    printBodies(tests);
    printMain(mainStructure, tests);
}

void TestCasePrinter::printFileHeader(const QString &mainStructure)
{
    m_stream << "#include \"AllModels.h\"";
    m_stream << endl;
    m_stream << QStringLiteral("static void %1_encode(const %1 *v, BitStream *stream)\n"
                               "{\n"
                               "  BitStream_Init(stream, stream->buf, stream->count);\n"
                               "  int errCode = 0;\n"
                               "  %1_ACN_Encode(v, stream, &errCode, FALSE);\n"
                               // TODO CCSDS ?
                               "}\n")
                    .arg(mainStructure);
    m_stream << endl;
    m_stream << QStringLiteral("static void validate(BitStream *stream)\n"
                               "{\n"
                               "  // TODO - fill to perform malformed tests on target\n"
                               "}\n")
             << endl;
}

void TestCasePrinter::printBodies(const QList<TestCase> &tests)
{
    for (const auto &t : tests) {
        print(t);
        m_stream << endl;
    }
}

void TestCasePrinter::printMain(const QString &mainStructure, const QList<TestCase> &tests)
{
    m_stream << QStringLiteral("int main()\n"
                               "{\n"
                               "  %1 v;\n"
                               "  byte buf[%1_REQUIRED_BYTES_FOR_ACN_ENCODING];\n"
                               "  BitStream stream;\n"
                               "  BitStream_Init(&stream, buf, sizeof(buf));\n"
                               "\n")
                    .arg(mainStructure);

    for (const auto &t : tests)
        m_stream << QStringLiteral("  test_%1(&v, &stream);\n").arg(t.name());

    m_stream << QStringLiteral("\n"
                               "  return 0;\n" // TODO
                               "}\n");
}
