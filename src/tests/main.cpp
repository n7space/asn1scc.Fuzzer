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

#include <QObject>
#include <QTest>

#include "acnnodereconstructingvisitor_tests.h"
#include "asn1nodereconstructingvisitor_tests.h"
#include "astxmlconstraintparser_tests.h"
#include "astxmlparser_tests.h"
#include "reconstructor_tests.h"

#include "data/constraints/printingvisitor_tests.h"

#include "data/range_tests.h"
#include "data/rangelist_tests.h"

#include "cases/enumeratedincorrectitems_tests.h"
#include "cases/integerincorrectvalues_tests.h"
#include "cases/integerranges_tests.h"
#include "cases/testcaseprinter_tests.h"

int main(int argc, char *argv[])
{
    int ret = 0;
    const auto runTest = [&ret, argc, argv](QObject *obj) {
        ret += QTest::qExec(obj, argc, argv);
        delete obj;
    };

    runTest(new Fuzzer::Tests::AstXmlParserTests);
    runTest(new Fuzzer::Tests::Asn1NodeReconstructingVisitorTests);
    runTest(new Fuzzer::Tests::AcnNodeReconstructingVisitorTests);
    runTest(new Fuzzer::Tests::ReconstructorTests);
    runTest(new Fuzzer::Tests::AstXmlConstraintParserTests);
    runTest(new Fuzzer::Data::Tests::RangeTests);
    runTest(new Fuzzer::Data::Tests::RangeListTests);
    runTest(new Fuzzer::Data::Constraints::Tests::PrintingVisitorTests);
    runTest(new Fuzzer::Cases::Tests::IntegerIncorrectValuesTests);
    runTest(new Fuzzer::Cases::Tests::EnumeratedIncorrectItemsTests);
    runTest(new Fuzzer::Cases::Tests::IntegerRangesTests);
    runTest(new Fuzzer::Cases::Tests::TestCasePrinterTests);

    return ret;
}
