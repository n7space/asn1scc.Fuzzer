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
#include "testcasebuilder.h"

#include <data/definitions.h>
#include <data/file.h>
#include <data/project.h>
#include <data/root.h>

#include <data/types/integer.h>
#include <data/types/sequence.h>
#include <data/types/userdefinedtype.h>

#include <data/constraints/rangeconstraint.h>

#include "enumeratedincorrectitems.h"
#include "integerincorrectvalues.h"

using namespace MalTester::Cases;
using namespace MalTester::Data::Types;
using namespace MalTester::Data;

TestCaseBuilder::TestCaseBuilder(const TypeReference &rootType)
    : m_rootType(rootType)
{}

TestCaseBuilder::~TestCaseBuilder() {}

TypeTestCaseBuilder::TypeTestCaseBuilder(TestCaseSink &sink, const FieldPath &currentPath)
    : m_sink(sink)
    , m_path(currentPath)
{}

TypeTestCaseBuilder::~TypeTestCaseBuilder() {}

void TypeTestCaseBuilder::visit(const Boolean &type)
{
    Q_UNUSED(type);
}

void TypeTestCaseBuilder::visit(const Null &type)
{
    Q_UNUSED(type);
}

void TypeTestCaseBuilder::visit(const BitString &type)
{
    Q_UNUSED(type);
}

void TypeTestCaseBuilder::visit(const OctetString &type)
{
    Q_UNUSED(type);
}

void TypeTestCaseBuilder::visit(const IA5String &type)
{
    Q_UNUSED(type);
}

void TypeTestCaseBuilder::visit(const NumericString &type)
{
    Q_UNUSED(type);
}

void TypeTestCaseBuilder::visit(const Enumerated &type)
{
    const EnumeratedIncorrectItems incorrect(type);
    for (const auto &item : incorrect.items())
        addCaseForValue(item.value());
}

void TypeTestCaseBuilder::addCaseForValue(long value)
{
    TestCase c{caseNameFor(value)};
    c.setAssignment({m_path, QString::number(value)});
    m_sink.append(c);
}

void TypeTestCaseBuilder::visit(const Choice &type)
{
    Q_UNUSED(type);
}

void TypeTestCaseBuilder::visit(const Sequence &type)
{
    for (const auto &c : type.components()) {
        TypeTestCaseBuilder b{m_sink, m_path + FieldPath(c->cName())};
        c->type().accept(b);
    }
}

void TypeTestCaseBuilder::visit(const SequenceOf &type)
{
    Q_UNUSED(type);
}

void TypeTestCaseBuilder::visit(const Real &type)
{
    Q_UNUSED(type);
}

void TypeTestCaseBuilder::visit(const LabelType &type)
{
    Q_UNUSED(type);
}

void TypeTestCaseBuilder::visit(const Integer &type)
{
    const IntegerIncorrectValues values(type);
    for (const auto value : values.items())
        addCaseForValue(value);
}

QString TypeTestCaseBuilder::caseNameFor(long value) const
{
    if (m_path.isEmpty())
        return QString::number(value);
    return m_path.join('_') + "_" + QString::number(value);
}

void TypeTestCaseBuilder::visit(const UserdefinedType &type)
{
    type.type().accept(*this);
}

void TestCaseBuilder::visit(const Root &root)
{
    for (const auto &p : root.projects())
        p->accept(*this);
}

void TestCaseBuilder::visit(const Definitions &defs)
{
    for (const auto &t : defs.types())
        if (m_rootType.module().isEmpty() || defs.name() == m_rootType.module())
            t->accept(*this);
}

void TestCaseBuilder::visit(const File &file)
{
    for (const auto &d : file.definitionsList())
        d->accept(*this);
}

void TestCaseBuilder::buildCasesForAssignment(const TypeAssignment &type)
{
    m_result = std::make_unique<TestCaseSink>(type.cName());
    TypeTestCaseBuilder b{*m_result};
    type.type()->accept(b);
}

void TestCaseBuilder::visit(const TypeAssignment &type)
{
    if (type.type() != nullptr && type.name() == m_rootType.name())
        buildCasesForAssignment(type);
}

void TestCaseBuilder::visit(const ValueAssignment &value)
{
    Q_UNUSED(value);
}

void TestCaseBuilder::visit(const Project &project)
{
    for (const auto &f : project.files())
        f->accept(*this);
}
