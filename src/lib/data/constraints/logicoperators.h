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

#include "constraint.h"
#include "constraintvisitor.h"

namespace MalTester {
namespace Data {
namespace Constraints {

template<typename ValueType>
class AndConstraint : public Constraint<ValueType>
{
public:
    AndConstraint(std::unique_ptr<Constraint<ValueType>> leftChild,
                  std::unique_ptr<Constraint<ValueType>> rightChild)
        : m_leftChild(std::move(leftChild))
        , m_rightChild(std::move(rightChild))
    {}
    ~AndConstraint() override = default;

    const Constraint<ValueType> &leftChild() const { return *m_leftChild; }
    const Constraint<ValueType> &rightChild() const { return *m_rightChild; }

    void accept(ConstraintVisitor<ValueType> &visitor) const { visitor.visit(*this); }

    /*    RangeList<typename RangeConstraint<T>::ValueType> asRangeList() const override
    {
        auto list = m_leftChild->asRangeList();
        list.intersect(m_rightChild->asRangeList());
        return list;
    }*/

    std::unique_ptr<Constraint<ValueType>> clone() const override
    {
        return std::make_unique<AndConstraint>(m_leftChild->clone(), m_rightChild->clone());
    }

private:
    std::unique_ptr<Constraint<ValueType>> m_leftChild;
    std::unique_ptr<Constraint<ValueType>> m_rightChild;
};

template<typename ValueType>
class OrConstraint : public Constraint<ValueType>
{
public:
    OrConstraint(std::unique_ptr<Constraint<ValueType>> leftChild,
                 std::unique_ptr<Constraint<ValueType>> rightChild)
        : m_leftChild(std::move(leftChild))
        , m_rightChild(std::move(rightChild))
    {}
    ~OrConstraint() override = default;

    const Constraint<ValueType> &leftChild() const { return *m_leftChild; }
    const Constraint<ValueType> &rightChild() const { return *m_rightChild; }

    void accept(ConstraintVisitor<ValueType> &visitor) const { visitor.visit(*this); }

    /*    RangeList<typename RangeConstraint<T>::ValueType> asRangeList() const override
    {
        auto list = m_leftChild->asRangeList();
        list.merge(m_rightChild->asRangeList());
        return list;
    }*/

    std::unique_ptr<Constraint<ValueType>> clone() const override
    {
        return std::make_unique<OrConstraint>(m_leftChild->clone(), m_rightChild->clone());
    }

private:
    std::unique_ptr<Constraint<ValueType>> m_leftChild;
    std::unique_ptr<Constraint<ValueType>> m_rightChild;
};

} // namespace Constraints
} // namespace Data
} // namespace MalTester
