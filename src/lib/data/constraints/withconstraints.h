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

namespace MalTester {
namespace Data {
namespace Constraints {

template<typename ValueType>
class WithConstraints
{
public:
    using Constraints = Constraint<ValueType>;

    WithConstraints() = default;
    WithConstraints(const WithConstraints &other);

    void setXConstraints(std::unique_ptr<Constraints> c) { m_constraints = std::move(c); }
    bool hasXConstraints() const { return m_constraints != nullptr; }
    const Constraints &Xconstraints() const { return *m_constraints; }

private:
    std::unique_ptr<Constraints> m_constraints;
};

template<typename ValueType>
WithConstraints<ValueType>::WithConstraints(const WithConstraints &other)
    : m_constraints(other.m_constraints ? other.m_constraints->clone() : nullptr)
{}

} // namespace Constraints
} // namespace Data
} // namespace MalTester
