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
#include "testcaseprinter.h"

using namespace Fuzzer::Cases;

TestCasePrinter::TestCasePrinter(QTextStream &stream)
    : m_stream(stream)
{}

static QString asString(const FieldPath &p)
{
    if (p.isEmpty())
        return "*v";
    return "v->" + p.join('.');
}

static QString asDocString(const FieldPath &p)
{
    if (p.isEmpty())
        return {};
    return " field " + p.join('.');
}

void TestCasePrinter::print(const QString &rootType, const TestCase &test)
{
    m_stream << QStringLiteral("/// \\brief Test %1 validating incorrect message processing.\n"
                               "/// \\details Prepares %2 type with incorrect value (%4)\n"
                               "///          stored inside%5.\n"
                               "static bool test_%1(%2 *v, BitStream *stream)\n"
                               "{\n"
                               "  %2_Initialize(v);\n"
                               "\n"
                               "  %3 = %4;\n"
                               "\n"
                               "  %2_encode(v, stream);\n"
                               "  return validate(stream);\n"
                               "}\n")
                    .arg(test.name())
                    .arg(rootType)
                    .arg(asString(test.fieldAssignment().m_path))
                    .arg(test.fieldAssignment().m_value)
                    .arg(asDocString(test.fieldAssignment().m_path));
}

void TestCasePrinter::print(const TestCaseSink &sink)
{
    printFileHeader(sink.rootTypeCName());
    printBodies(sink);
    printMain(sink);
}

void TestCasePrinter::printFileHeader(const QString &rootType)
{
    m_stream << "#include <stdio.h>\n"
             << "#include <stdbool.h>\n"
             << endl
             << "#include \"AllModels.h\"\n"
             << "#include \"validate.h\"\n"
             << endl
             << QStringLiteral("#define RUN_TEST(T, ...) \\\n"
                               "  (printf(\"Executing \" #T \" ... \"), \\\n"
                               "   (T(__VA_ARGS__) \\\n"
                               "      ? (printf(\"PASSED\\n\"), 0) \\\n"
                               "      : (printf(\"FAILED\\n\"), 1)))\n")
             << endl
             << QStringLiteral("static void %1_encode(const %1 *v, BitStream *stream)\n"
                               "{\n"
                               "  BitStream_Init(stream, stream->buf, stream->count);\n"
                               "  int errCode = 0;\n"
                               "  %1_ACN_Encode(v, stream, &errCode, FALSE);\n"
                               "}\n")
                    .arg(rootType)
             << endl;
}

void TestCasePrinter::printBodies(const TestCaseSink &sink)
{
    for (const auto &t : sink.cases()) {
        print(sink.rootTypeCName(), t);
        m_stream << endl;
    }
}

void TestCasePrinter::printMain(const TestCaseSink &sink)
{
    m_stream << QStringLiteral("int main()\n"
                               "{\n"
                               "  %1 v;\n"
                               "  byte buf[%1_REQUIRED_BYTES_FOR_ACN_ENCODING];\n"
                               "  BitStream stream;\n"
                               "  BitStream_Init(&stream, buf, sizeof(buf));\n"
                               "  int result = 0;\n"
                               "\n")
                    .arg(sink.rootTypeCName());

    for (const auto &t : sink.cases())
        m_stream << QStringLiteral("  result += RUN_TEST(test_%1, &v, &stream);\n").arg(t.name());

    m_stream << QStringLiteral("\n"
                               "  if (result == 0)\n"
                               "    printf(\"OK - all (%1) tests passed\\n.\");\n"
                               "  else\n"
                               "    printf(\"ERROR - Failed %d out of %1 tests\\n.\", result);\n"
                               "  return result;\n"
                               "}\n")
                    .arg(sink.cases().count());
}
