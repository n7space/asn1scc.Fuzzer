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
#include "valueassignment.h"

#include "mutatingvisitor.h"
#include "visitor.h"

using namespace Fuzzer::Data;

ValueAssignment::ValueAssignment(const QString &name,
                                 const SourceLocation &location,
                                 std::unique_ptr<Types::Type> type,
                                 ValuePtr value)
    : Node(name, location)
    , m_type(std::move(type))
    , m_value(std::move(value))
{}

ValueAssignment::ValueAssignment(const ValueAssignment &other)
    : Node(other.name(), other.location())
    , m_type(other.type()->clone())
{
    if (other.m_value != nullptr)
        m_value = other.m_value->clone();
}

ValueAssignment::~ValueAssignment() {}

void ValueAssignment::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

void ValueAssignment::accept(MutatingVisitor &visitor)
{
    visitor.visit(*this);
}
