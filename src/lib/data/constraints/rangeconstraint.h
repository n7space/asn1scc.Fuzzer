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
#pragma once

#include <data/range.h>

#include "constraint.h"
#include "constraintvisitor.h"

namespace Fuzzer {
namespace Data {
namespace Constraints {

template<typename ValueType>
class RangeConstraint : public Constraint<ValueType>
{
public:
    static std::unique_ptr<RangeConstraint> create(const Range<typename ValueType::Type> &range)
    {
        return std::make_unique<RangeConstraint>(range);
    }

    explicit RangeConstraint(const Range<typename ValueType::Type> &range)
        : m_range(range)
    {}
    ~RangeConstraint() override = default;

    const Range<typename ValueType::Type> &range() const { return m_range; }

    void accept(ConstraintVisitor<ValueType> &visitor) const override { visitor.visit(*this); }

    std::unique_ptr<Constraint<ValueType>> clone() const override
    {
        return std::make_unique<RangeConstraint>(m_range);
    }

private:
    Range<typename ValueType::Type> m_range;
};

} // namespace Constraints
} // namespace Data
} // namespace Fuzzer
