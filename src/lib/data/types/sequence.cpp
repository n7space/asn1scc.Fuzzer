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
                                     const QString &presentWhen,
                                     const SourceLocation &location,
                                     std::unique_ptr<Type> type)
    : m_name(name)
    , m_presentWhen(presentWhen)
    , m_location(location)
    , m_type(std::move(type))
{}

SequenceComponent::SequenceComponent(const SequenceComponent &other)
    : m_name(other.m_name)
    , m_presentWhen(other.m_presentWhen)
    , m_location(other.m_location)
    , m_type(other.m_type->clone())
{}

Sequence::Sequence(const Sequence &other)
    : Type(other)
{
    for (const auto &parameter : other.m_parameters)
        addParameter(std::make_unique<AcnParameter>(*parameter));

    for (const auto &component : other.m_acnComponents)
        addAcnComponent(std::make_unique<AcnComponent>(*component));
}

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

void Sequence::addParameter(AcnParameterPtr parameter)
{
    m_parameters.push_back(std::move(parameter));
}

void Sequence::addAcnComponent(AcnComponentPtr component)
{
    m_acnComponents.push_back(std::move(component));
}
