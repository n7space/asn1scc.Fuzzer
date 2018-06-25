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

#include "rangeconstraint.h"

namespace MalTester {
namespace Data {
namespace Constraints {

template<typename T>
class WithValueConstraints
{
public:
    WithValueConstraints() = default;
    WithValueConstraints(const WithValueConstraints &other);

    void setValueConstraints(std::unique_ptr<RangeConstraint<T>> c)
    {
        m_constraints = std::move(c);
    }
    const RangeConstraint<T> *valueConstraints() const { return m_constraints.get(); }

private:
    std::unique_ptr<RangeConstraint<T>> m_constraints;
};

class WithSizeConstraints
{
public:
    WithSizeConstraints() = default;
    WithSizeConstraints(const WithSizeConstraints &other) = default;

    void setSizeConstraints(std::unique_ptr<RangeConstraint<int>> c)
    {
        m_constraints.setValueConstraints(std::move(c));
    }
    const RangeConstraint<int> *sizeConstraints() const { return m_constraints.valueConstraints(); }

private:
    WithValueConstraints<int> m_constraints;
};

template<typename T>
WithValueConstraints<T>::WithValueConstraints(const WithValueConstraints &other)
    : m_constraints(other.m_constraints ? other.m_constraints->clone() : nullptr)
{}

} // namespace Constraints
} // namespace Data
} // namespace MalTester
