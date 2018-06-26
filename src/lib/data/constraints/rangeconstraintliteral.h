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

#include "range.h"
#include "rangeconstraint.h"

namespace MalTester {
namespace Data {
namespace Constraints {

template<typename T>
class RangeConstraintLiteral : public RangeConstraint<T>
{
public:
    explicit RangeConstraintLiteral(const Range<typename RangeConstraint<T>::ValueType> &range)
        : m_range(range)
    {}
    ~RangeConstraintLiteral() override = default;

    QString asString() const override { return m_range.asString(); }
    RangeList<typename RangeConstraint<T>::ValueType> asRangeList() const override
    {
        return {m_range};
    }
    std::unique_ptr<RangeConstraint<T>> clone() const override
    {
        return std::make_unique<RangeConstraintLiteral>(m_range);
    }

private:
    Range<typename RangeConstraint<T>::ValueType> m_range;
};

} // namespace Constraints
} // namespace Data
} // namespace MalTester
