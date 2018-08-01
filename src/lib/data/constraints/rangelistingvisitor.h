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

#include <algorithm>
#include <functional>
#include <stdexcept>

#include <data/rangelist.h>

#include "constraintvisitor.h"

#include "constraintlist.h"
#include "fromconstraint.h"
#include "logicoperators.h"
#include "rangeconstraint.h"
#include "sizeconstraint.h"

namespace MalTester {
namespace Data {
namespace Constraints {

template<typename ValueType, typename ResultType = typename ValueType::Type>
class RangeListingVisitorBase : public ConstraintVisitor<ValueType>
{
protected:
    RangeListingVisitorBase() = default;

public:
    const RangeList<ResultType> &result() const { return m_result; }

    void visit(const AndConstraint<ValueType> &constraint) override;
    void visit(const OrConstraint<ValueType> &constraint) override;
    void visit(const FromConstraint<ValueType> &constraint) override;
    void visit(const SizeConstraint<ValueType> &constraint) override;
    void visit(const ConstraintList<ValueType> &constraint) override;
    void visit(const RangeConstraint<ValueType> &constraint) override = 0;

protected:
    virtual RangeList<ResultType> toRangeList(const Constraint<ValueType> &c) const = 0;

    RangeList<ResultType> m_result;
};

template<typename ValueType>
class RangeListingVisitor : public RangeListingVisitorBase<ValueType, typename ValueType::Type>
{
public:
    void visit(const RangeConstraint<ValueType> &constraint) override;

private:
    RangeList<typename ValueType::Type> toRangeList(const Constraint<ValueType> &c) const override;
};

template<typename ValueType>
inline RangeList<typename ValueType::Type> toRangeList(const Constraint<ValueType> &c)
{
    RangeListingVisitor<ValueType> v;
    c.accept(v);
    return v.result();
}

template<typename ValueType>
void RangeListingVisitor<ValueType>::visit(const RangeConstraint<ValueType> &constraint)
{
    this->m_result = {constraint.range()};
}

template<typename ValueType>
RangeList<typename ValueType::Type> RangeListingVisitor<ValueType>::toRangeList(
    const Constraint<ValueType> &c) const
{
    return MalTester::Data::Constraints ::toRangeList(c);
}

template<typename ValueType, typename ResultType>
void RangeListingVisitorBase<ValueType, ResultType>::visit(const AndConstraint<ValueType> &constraint)
{
    m_result = this->toRangeList(constraint.leftChild());
    m_result.intersect(this->toRangeList(constraint.rightChild()));
}

template<typename ValueType, typename ResultType>
void RangeListingVisitorBase<ValueType, ResultType>::visit(const OrConstraint<ValueType> &constraint)
{
    m_result = this->toRangeList(constraint.leftChild());
    m_result.merge(this->toRangeList(constraint.rightChild()));
}

template<typename ValueType, typename ResultType>
void RangeListingVisitorBase<ValueType, ResultType>::visit(const FromConstraint<ValueType> &)
{
    throw std::runtime_error("Unable to flatten FROM constaint");
}

template<typename ValueType, typename ResultType>
void RangeListingVisitorBase<ValueType, ResultType>::visit(const SizeConstraint<ValueType> &)
{
    throw std::runtime_error("Unable to flatten SIZE constaint");
}

template<typename ValueType, typename ResultType>
void RangeListingVisitorBase<ValueType, ResultType>::visit(
    const ConstraintList<ValueType> &constraint)
{
    const auto &constraints = constraint.constraints();
    if (constraints.empty())
        return;
    m_result = toRangeList(*constraints.front());
    std::for_each(std::begin(constraints) + 1,
                  std::end(constraints),
                  [this](const std::unique_ptr<Constraint<ValueType>> &c) {
                      m_result.intersect(toRangeList(*c));
                  });
    m_result.compact();
}

} // namespace Constraints
} // namespace Data
} // namespace MalTester
