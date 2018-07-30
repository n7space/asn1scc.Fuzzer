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
#include "enumerated.h"

#include "typemutatingvisitor.h"
#include "typereadingvisitor.h"

using namespace MalTester::Data::Types;
using namespace MalTester::Data;

Enumerated::Enumerated()
    : m_encodeValues(false)
{}

std::unique_ptr<Type> Enumerated::clone() const
{
    return std::make_unique<Enumerated>(*this);
}

QString Enumerated::name() const
{
    return QLatin1String("ENUMERATED");
}

void Enumerated::accept(TypeMutatingVisitor &visitor)
{
    visitor.visit(*this);
}

void Enumerated::accept(TypeReadingVisitor &visitor) const
{
    visitor.visit(*this);
}

void Enumerated::addItem(const EnumeratedItem &item)
{
    m_items.insert(item.name(), item);
}

int Enumerated::mapToValue(const QString &key) const
{
    const auto it = items().find(key);
    if (it == items().end())
        throw std::runtime_error("Unknown key in ENUMERATED");
    return encodeValues() ? it->value() : it->index();
}

Range<std::int64_t> Enumerated::mapToValue(const Range<QString> &r) const
{
    if (!r.isSingleItem())
        throw std::runtime_error("Multi-item range in ENUMERATED");
    return Range<std::int64_t>(mapToValue(r.begin()));
}
