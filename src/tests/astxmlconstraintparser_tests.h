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

#pragma once

#include <QObject>

#include <astxmlconstraintparser.h>

namespace MalTester {
namespace Tests {

class AstXmlConstraintParserTests : public QObject
{
    Q_OBJECT
public:
    explicit AstXmlConstraintParserTests(QObject *parent = 0);

private slots:
    void test_emptyXml();

    void test_simpleIntegerValue();
    void test_simpleRealValue();
    void test_simpleBooleanValue();
    void test_simpleEnumValue();
    void test_simpleStringValue();
    void test_simpleBitStringValue();
    void test_simpleOctetStringValue();

    void test_range();
    void test_rangeWithoutMinOrMax();

    void test_multipleValues();

    void test_or();
    void test_and();

    void test_nestedLogicalOperators();

    void test_from();

    void test_size();
    void test_mixed();
    void test_mixedInReverseOrder();

    void test_complex();

private:
    template<typename T>
    void parsingFails(const QString &xmlData);

    template<typename T>
    void parse(const QString &xmlData);

    QString m_dumpedConstraint;
};

} // namespace Tests
} // namespace MalTester
