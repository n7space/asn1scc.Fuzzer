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
#include "sequence.h"

#include "typevisitor.h"

using namespace MalTester::Data;
using namespace MalTester::Data::Types;

SequenceComponent::SequenceComponent(const QString &name,
                                     const SourceLocation &location,
                                     std::unique_ptr<Type> type)
    : m_name(name)
    , m_location(location)
    , m_type(std::move(type))
{}

SequenceComponent::SequenceComponent(const SequenceComponent &other)
    : m_name(other.m_name)
    , m_location(other.m_location)
    , m_type(other.m_type->clone())
{}

void Sequence::accept(TypeVisitor &visitor)
{
    visitor.visit(*this);
}

std::unique_ptr<Type> Sequence::clone() const
{
    return std::make_unique<Sequence>(*this);
}

void Sequence::addComponent(const QString &key, const SequenceComponent &component)
{
    m_components.insert(std::pair<QString, SequenceComponent>(key, component));
}
