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
#include <stdexcept>

#include <QString>

#include "rangeconstraint.h"

namespace MalTester {
namespace Data {
namespace Constraints {

template<typename T>
class ElementConstraint : public RangeConstraint<T>
{
public:
    ElementConstraint(std::unique_ptr<RangeConstraint<T>> elementConstraints)
        : m_elementContraints(std::move(elementConstraints))
    {}

    QString asString() const override;
    std::unique_ptr<RangeConstraint<T>> clone() const override;

    RangeList<T> asRangeList() const override
    {
        throw std::runtime_error("Unable to flatten element constaint");
    }

private:
    std::unique_ptr<RangeConstraint<T>> m_elementContraints;
};

template<typename T>
QString ElementConstraint<T>::asString() const
{
    return "FROM(" + m_elementContraints->asString() + ")";
}

template<typename T>
std::unique_ptr<RangeConstraint<T>> ElementConstraint<T>::clone() const
{
    return std::make_unique<ElementConstraint<T>>(m_elementContraints->clone());
}

} // namespace Constraints
} // namespace Data
} // namespace MalTester
