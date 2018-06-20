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

#include "rangeconstraint.h"

namespace MalTester {
namespace Data {
namespace Constraints {

template<typename T>
class AndConstraint : public RangeConstraint<T>
{
public:
    AndConstraint(std::unique_ptr<RangeConstraint<T>> leftChild,
                  std::unique_ptr<RangeConstraint<T>> rightChild)
        : m_leftChild(std::move(leftChild))
        , m_rightChild(std::move(rightChild))
    {}
    ~AndConstraint() override = default;

    QString asString() const override
    {
        return QStringLiteral("(%1 ^ %2)").arg(m_leftChild->asString()).arg(m_rightChild->asString());
    }

    RangeList<T> asRangeList() const override
    {
        auto list = m_leftChild->asRangeList();
        list.intersect(m_rightChild->asRangeList());
        return list;
    }

    std::unique_ptr<RangeConstraint<T>> clone() const override
    {
        return std::make_unique<AndConstraint>(m_leftChild->clone(), m_rightChild->clone());
    }

private:
    std::unique_ptr<RangeConstraint<T>> m_leftChild;
    std::unique_ptr<RangeConstraint<T>> m_rightChild;
};

template<typename T>
class OrConstraint : public RangeConstraint<T>
{
public:
    OrConstraint(std::unique_ptr<RangeConstraint<T>> leftChild,
                 std::unique_ptr<RangeConstraint<T>> rightChild)
        : m_leftChild(std::move(leftChild))
        , m_rightChild(std::move(rightChild))
    {}
    ~OrConstraint() override = default;

    QString asString() const override
    {
        return QStringLiteral("(%1 | %2)").arg(m_leftChild->asString()).arg(m_rightChild->asString());
    }

    RangeList<T> asRangeList() const override
    {
        auto list = m_leftChild->asRangeList();
        list.merge(m_rightChild->asRangeList());
        return list;
    }

    std::unique_ptr<RangeConstraint<T>> clone() const override
    {
        return std::make_unique<OrConstraint>(m_leftChild->clone(), m_rightChild->clone());
    }

private:
    std::unique_ptr<RangeConstraint<T>> m_leftChild;
    std::unique_ptr<RangeConstraint<T>> m_rightChild;
};

} // namespace Constraints
} // namespace Data
} // namespace MalTester
