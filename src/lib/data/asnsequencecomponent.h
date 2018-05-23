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

#include <data/sequencecomponent.h>

namespace MalTester {
namespace Data {

class AsnSequenceComponent : public SequenceComponent
{
public:
    AsnSequenceComponent() = default;
    ~AsnSequenceComponent() override = default;

    AsnSequenceComponent(const QString &name,
                         bool optional,
                         const QString &presentWhen,
                         const SourceLocation &location,
                         std::unique_ptr<Types::Type> type)
        : SequenceComponent(name, std::move(type))
        , m_optional(optional)
        , m_presentWhen(presentWhen)
        , m_location(location)
    {}

    AsnSequenceComponent(const AsnSequenceComponent &other)
        : SequenceComponent(other)
        , m_presentWhen(other.m_presentWhen)
        , m_location(other.m_location)
    {}

    const SourceLocation &location() const { return m_location; }
    const QString &presentWhen() const { return m_presentWhen; }
    bool isOptional() const { return m_optional; }

private:
    bool m_optional;
    QString m_presentWhen;
    SourceLocation m_location;
};

} // namespace Data
} // namespace MalTester
