/****************************************************************************
**
** Copyright (C) 2018-2019 N7 Space sp. z o. o.
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
#pragma once

#include <functional>
#include <memory>

#include <QObject>

#include <data/definitions.h>
#include <data/file.h>
#include <data/project.h>
#include <data/valueassignment.h>

namespace Fuzzer {
namespace Tests {

class AcnNodeReconstructingVisitorTests : public QObject
{
    Q_OBJECT
public:
    explicit AcnNodeReconstructingVisitorTests(QObject *parent = 0);

private slots:
    void test_emptyModule();
    void test_boolean();
    void test_null();
    void test_integer();
    void test_real();
    void test_bitString();
    void test_octetString();
    void test_IA5String();
    void test_numericString();
    void test_enumerated();
    void test_sequenceOf();

    void test_choice();
    void test_choiceNested();
    void test_choiceParametrized();

    void test_sequenceNested();
    void test_sequence();

    void test_acnParameter();
    void test_acnArgument();

    void test_integerEncodingToString();
    void test_realEncodingToString();
    void test_asciiStringEncodingToString();

private:
    void performTest(const QString &name,
                     const QString &expectedResult,
                     std::unique_ptr<Data::Types::Type> type) const;
    QString restoreNode(const Data::Node &node) const;
};

} // namespace Tests
} // namespace Fuzzer
