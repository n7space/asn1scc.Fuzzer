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

#include <data/constraints/constraint.h>
#include <data/definitions.h>
#include <data/file.h>
#include <data/project.h>
#include <data/valueassignment.h>

namespace Fuzzer {
namespace Tests {

class Asn1NodeReconstructingVisitorTests : public QObject
{
    Q_OBJECT
public:
    explicit Asn1NodeReconstructingVisitorTests(QObject *parent = 0);

private slots:
    void test_singleImportedType();
    void test_multipleImportedTypes();
    void test_singleImportedValue();
    void test_multipleImportedValues();

    void test_valueAssignmentSimpleValue();
    void test_valueAssignmentBitString();
    void test_valueAssignmentBoolean();
    void test_valueAssignmentOctetString();
    void test_valueAssignmentIA5String();
    void test_valueAssignmentChoice();
    void test_valueAssignmentMultipleValues();
    void test_valueAssignmentNamedValue();

    void test_typeAssignmentBoolean();
    void test_typeAssignmentNull();

    void test_typeAssignmentBitString();
    void test_typeAssignmentBitStringWithValue();

    void test_typeAssignmentOctetString();
    void test_typeAssignmentOctetStringWithValue();

    void test_typeAssignmentIA5String();
    void test_typeAssignmentIA5StringWithValue();

    void test_typeAssignmentNumericString();
    void test_typeAssignmentNumericStringWithValue();

    void test_typeAssignmentEnumerated();

    void test_typeAssignmentChoice();
    void test_typeAssignmentNestedChoice();

    void test_typeAssignmentSequence();
    void test_typeAssignmentNestedSequence();

    void test_typeAssignmentSequenceOf();
    void test_typeAssignmentSequenceOfWithValue();

    void test_typeAssignmentReal();
    void test_typeAssignmentRealWithValue();

    void test_typeAssignmentInteger();
    void test_typeAssignmentIntegerWithValue();

    void test_typeAssignmentUserDefined();
    void test_typeAssignmentUserDefinedWithValue();

    void test_typeAssignmentBooleanAcnParams();

private:
    std::unique_ptr<Data::Definitions> createDefinitions(const QString &name) const;

    QString createSingleValueValueAssignmentValue(
        const QString &typeName,
        const QString &typeValue,
        std::function<QString(const QString &)> printer = {}) const;

    void testSimpleTypeAssignment(const QString &astValue, const QString &asn1Value) const;

    std::unique_ptr<Data::TypeAssignment> createSimpleTypeAssignment(
        const QString &astTypeName) const;

    template<typename T>
    std::unique_ptr<Data::TypeAssignment> createTypeAssignmentWithConstraint(
        const QString &astTypeName, Data::Constraints::Constraint<T> *contraint) const;

    QString createMultipleValueValueAssignmentValue() const;
    QString createNamedValueAssignmentValue() const;
    QString createComponentialTypeAssignmentValue(std::unique_ptr<Data::Types::Type> type) const;

    QString createChoiceValueAssignmentValue() const;

    QString restoreNode(const Data::Node &assignment) const;
};

} // namespace Tests
} // namespace Fuzzer
