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
#include "sequenceof.h"

#include "typevisitor.h"

using namespace MalTester::Data::Types;

void SequenceOf::accept(TypeVisitor &visitor)
{
    visitor.visit(*this);
}

SequenceOf::SequenceOf(const SequenceOf &other)
    : Type(other)
    , WithConstraints<IntegerConstraints>(other)
    , m_size(other.m_size)
    , m_itemsType(other.m_itemsType->clone())
{}

std::unique_ptr<Type> SequenceOf::clone() const
{
    return std::make_unique<SequenceOf>(*this);
}
