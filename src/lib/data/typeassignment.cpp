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
#include "typeassignment.h"

#include "mutatingvisitor.h"
#include "visitor.h"

using namespace MalTester::Data;

TypeAssignment::TypeAssignment(const QString &name,
                               const QString &cName,
                               const SourceLocation &location,
                               std::unique_ptr<Types::Type> type)
    : Node(name, location)
    , m_cName(cName)
    , m_type(std::move(type))
{}

TypeAssignment::TypeAssignment(const TypeAssignment &other)
    : TypeAssignment(other.name(), other.cName(), other.location(), other.type()->clone())
{}

TypeAssignment::~TypeAssignment() {}

void TypeAssignment::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

void TypeAssignment::accept(MutatingVisitor &visitor)
{
    visitor.visit(*this);
}
