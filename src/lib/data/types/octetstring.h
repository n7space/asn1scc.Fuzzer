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

#include "constraints.h"

#include <data/types/type.h>

namespace MalTester {
namespace Data {
namespace Types {

class OctetString : public Type,
                    public WithConstraints<IntegerConstraints>,
                    public WithConstraints<StringConstraints>
{
public:
    OctetString() = default;
    OctetString(const OctetString &other) = default;

    QString name() const override { return QLatin1String("OCTET STRING"); }
    void accept(TypeVisitor &visitor) override;
    std::unique_ptr<Type> clone() const override;

    IntegerConstraints &integerConstraints()
    {
        return WithConstraints<IntegerConstraints>::constraints();
    }

    const IntegerConstraints &integerConstraints() const
    {
        return WithConstraints<IntegerConstraints>::constraints();
    }

    StringConstraints &stringConstraints()
    {
        return WithConstraints<StringConstraints>::constraints();
    }

    const StringConstraints &stringConstraints() const
    {
        return WithConstraints<StringConstraints>::constraints();
    }

}; // namespace Types

} // namespace Types
} // namespace Data
} // namespace MalTester
