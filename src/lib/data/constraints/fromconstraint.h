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

#include <data/values.h>

#include "constraint.h"
#include "constraintvisitor.h"

namespace MalTester {
namespace Data {
namespace Constraints {

template<typename ValueType>
class FromConstraint : public Constraint<ValueType>
{
public:
    FromConstraint(std::unique_ptr<Constraint<Data::StringValue>> innerConstraints)
        : m_innerContraints(std::move(innerConstraints))
    {}

    const Constraint<Data::StringValue> &innerConstraints() const { return *m_innerContraints; }

    void accept(ConstraintVisitor<ValueType> &visitor) const { visitor.visit(*this); }

    std::unique_ptr<Constraint<ValueType>> clone() const override;

    /*RangeList<typename RangeConstraint<T>::ValueType> asRangeList() const override
    {
        throw std::runtime_error("Unable to flatten element constaint");
    }*/

private:
    std::unique_ptr<Constraint<Data::StringValue>> m_innerContraints;
};

template<typename ValueType>
std::unique_ptr<Constraint<ValueType>> FromConstraint<ValueType>::clone() const
{
    return std::make_unique<FromConstraint<ValueType>>(m_innerContraints->clone());
}

} // namespace Constraints
} // namespace Data
} // namespace MalTester
