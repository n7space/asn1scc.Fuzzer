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

#include <memory>

#include <QString>

#include <data/typereference.h>
#include <data/types/typereadingvisitor.h>
#include <data/visitor.h>

#include "testcase.h"
#include "testcasesink.h"

namespace MalTester {
namespace Cases {

class TestCaseBuilder : public Data::Visitor
{
public:
    explicit TestCaseBuilder(const Data::TypeReference &rootType);
    ~TestCaseBuilder() override;

    std::unique_ptr<TestCaseSink> takeResult() { return std::move(m_result); }

    void visit(const Data::Root &root) override;
    void visit(const Data::Definitions &defs) override;
    void visit(const Data::File &file) override;
    void visit(const Data::TypeAssignment &type) override;
    void visit(const Data::ValueAssignment &value) override;
    void visit(const Data::Project &project) override;

private:
    void buildCasesForAssignment(const Data::TypeAssignment &type);

    Data::TypeReference m_rootType;
    std::unique_ptr<TestCaseSink> m_result;
};

class TypeTestCaseBuilder : public Data::Types::TypeReadingVisitor
{
public:
    explicit TypeTestCaseBuilder(TestCaseSink &sink, const FieldPath &currentPath = {});
    ~TypeTestCaseBuilder() override;

    void visit(const Data::Types::Boolean &type) override;
    void visit(const Data::Types::Null &type) override;
    void visit(const Data::Types::BitString &type) override;
    void visit(const Data::Types::OctetString &type) override;
    void visit(const Data::Types::IA5String &type) override;
    void visit(const Data::Types::NumericString &type) override;
    void visit(const Data::Types::Enumerated &type) override;
    void visit(const Data::Types::Choice &type) override;
    void visit(const Data::Types::Sequence &type) override;
    void visit(const Data::Types::SequenceOf &type) override;
    void visit(const Data::Types::Real &type) override;
    void visit(const Data::Types::LabelType &type) override;
    void visit(const Data::Types::Integer &type) override;
    void visit(const Data::Types::UserdefinedType &type) override;

private:
    QString caseNameFor(int value) const;
    void addCaseForValue(int value);

    TestCaseSink &m_sink;
    const FieldPath m_path;
};

} // namespace Cases
} // namespace MalTester
