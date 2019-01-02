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
#include "definitions.h"

#include "mutatingvisitor.h"
#include "visitor.h"

using namespace Fuzzer::Data;

Definitions::Definitions(const QString &name, const SourceLocation &location)
    : Node(name, location)
{}

Definitions::Definitions(const Definitions &other)
    : Definitions(other.name(), other.location())
{
    for (const auto &type : other.types())
        addType(std::make_unique<TypeAssignment>(*type));
    for (const auto &value : other.values())
        addValue(std::make_unique<ValueAssignment>(*value));
    for (const auto &importedType : other.importedTypes())
        addImportedType(importedType);
    for (const auto &importedValue : other.importedValues())
        addImportedValue(importedValue);
}

Definitions::~Definitions() {}

void Definitions::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

void Definitions::accept(MutatingVisitor &visitor)
{
    visitor.visit(*this);
}

void Definitions::addType(std::unique_ptr<TypeAssignment> type)
{
    type->setParent(this);
    m_typeByNameMap[type->name()] = type.get();
    m_types.push_back(std::move(type));
}

void Definitions::addValue(std::unique_ptr<ValueAssignment> value)
{
    value->setParent(this);
    m_valueByNameMap[value->name()] = value.get();
    m_values.push_back(std::move(value));
}

void Definitions::addImportedType(const ImportedType &type)
{
    m_importedTypes.push_back(type);
}

void Definitions::addImportedValue(const ImportedValue &value)
{
    m_importedValues.push_back(value);
}

const TypeAssignment *Definitions::type(const QString &name) const
{
    const auto it = m_typeByNameMap.find(name);
    return it != m_typeByNameMap.end() ? it->second : nullptr;
}

const ValueAssignment *Definitions::value(const QString &name) const
{
    const auto it = m_valueByNameMap.find(name);
    return it != m_valueByNameMap.end() ? it->second : nullptr;
}
