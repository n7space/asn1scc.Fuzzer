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

#include <QString>

#include <data/constraints.h>

#include <data/types/builtintypes.h>

namespace MalTester {
namespace Data {
namespace Types {

class Real : public BuiltinType
{
public:
    Real()
        : BuiltinType(std::make_unique<Constraints>(&Real::toVariantPair))
    {}

    QString name() const override { return QLatin1String("REAL"); }

private:
    QString baseIconFile() const override
    {
        return QStringLiteral(":/asn1acn/images/outline/real.png");
    }

    static Constraints::VariantPair toVariantPair(const Constraints::StringPair &range)
    {
        return {range.first.toDouble(), range.second.toDouble()};
    }
};

} // namespace Types
} // namespace Data
} // namespace MalTester
