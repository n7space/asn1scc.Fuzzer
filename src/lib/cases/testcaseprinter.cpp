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
    m_stream << QStringLiteral("void test_%1(%2 *v, BitStream *stream)\n"
                               "{\n"
                               "  BitStream_Init(stream, stream->buf, stream->count);\n"
                               "  %2_Init(v);\n"
                               "\n"
                               "  %3 = %4;\n"
                               "\n"
                               "  %2_EncodeACN(stream, v);\n"
                               "  validate(stream);\n"
                               "}\n")
                    .arg(test.name())
                    .arg(test.typeUnderTest())
                    .arg(asString(test.fieldAssignment().m_path))
                    .arg(test.fieldAssignment().m_value);
}
