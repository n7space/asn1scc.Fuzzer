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
#include "constraintsrelaxingvisitor.h"

#include <QtGlobal>

#include <data/definitions.h>
#include <data/file.h>
#include <data/project.h>
#include <data/root.h>

#include <data/types/integer.h>
#include <data/types/sequence.h>
#include <data/types/sequenceof.h>

#include <data/constraints/rangeconstraint.h>

#include "integerranges.h"

using namespace MalTester::Cases;
using namespace MalTester::Data::Types;
using namespace MalTester::Data;

ConstraintsRelaxingVisitor::~ConstraintsRelaxingVisitor() {}

void ConstraintsRelaxingVisitor::visit(Boolean &type)
{
    Q_UNUSED(type);
}

void ConstraintsRelaxingVisitor::visit(Null &type)
{
    Q_UNUSED(type);
}

void ConstraintsRelaxingVisitor::visit(BitString &type)
{
    Q_UNUSED(type);
}

void ConstraintsRelaxingVisitor::visit(OctetString &type)
{
    Q_UNUSED(type);
}

void ConstraintsRelaxingVisitor::visit(IA5String &type)
{
    Q_UNUSED(type);
}

void ConstraintsRelaxingVisitor::visit(NumericString &type)
{
    Q_UNUSED(type);
}

void ConstraintsRelaxingVisitor::visit(Enumerated &type)
{
    Q_UNUSED(type);
}

void ConstraintsRelaxingVisitor::visit(Choice &type)
{
    Q_UNUSED(type);
}

void ConstraintsRelaxingVisitor::visit(Sequence &type)
{
    for (const auto &c : type.components())
        c->type().accept(*this);
}

void ConstraintsRelaxingVisitor::visit(SequenceOf &type)
{
    type.itemsType().accept(*this);
}

void ConstraintsRelaxingVisitor::visit(Real &type)
{
    Q_UNUSED(type);
}

void ConstraintsRelaxingVisitor::visit(LabelType &type)
{
    Q_UNUSED(type);
}

void ConstraintsRelaxingVisitor::visit(Integer &type)
{
    type.constraints().clear();
    type.constraints().append(maxValueRangeFor(type));
}

void ConstraintsRelaxingVisitor::visit(UserdefinedType &type)
{
    Q_UNUSED(type);
}

void ConstraintsRelaxingVisitor::visit(Root &root)
{
    for (const auto &p : root.projects())
        p->accept(*this);
}

void ConstraintsRelaxingVisitor::visit(Definitions &defs)
{
    for (const auto &t : defs.types())
        t->accept(*this);
}

void ConstraintsRelaxingVisitor::visit(File &file)
{
    for (const auto &d : file.definitionsList())
        d->accept(*this);
}

void ConstraintsRelaxingVisitor::visit(TypeAssignment &type)
{
    if (type.type() != nullptr)
        type.type()->accept(*this);
}

void ConstraintsRelaxingVisitor::visit(ValueAssignment &value)
{
    Q_UNUSED(value);
}

void ConstraintsRelaxingVisitor::visit(Project &project)
{
    for (const auto &f : project.files())
        f->accept(*this);
}
