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

#include <data/sourcelocation.h>
#include <data/types/type.h>

namespace MalTester {
namespace Data {

class SequenceComponent
{
public:
    SequenceComponent() = default;
    SequenceComponent(const QString &name, std::unique_ptr<Types::Type> type)
        : m_name(name)
        , m_type(std::move(type))
    {}

    SequenceComponent(const SequenceComponent &other)
        : m_name(other.m_name)
        , m_type(other.m_type->clone())
    {}

    virtual ~SequenceComponent() = default;

    const QString &name() const { return m_name; }
    const Types::Type &type() const { return *m_type; }

private:
    QString m_name;
    std::unique_ptr<Types::Type> m_type;
};

} // namespace Data
} // namespace MalTester
