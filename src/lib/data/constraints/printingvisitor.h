/****************************************************************************
**
** Copyright (C) 2018 N7 Space sp. z o. o.
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

#include <QTextStream>

#include "constraintvisitor.h"

#include "constraintlist.h"
#include "fromconstraint.h"
#include "logicoperators.h"
#include "rangeconstraint.h"
#include "sizeconstraint.h"

namespace Fuzzer {
namespace Data {
namespace Constraints {

template<typename ValueType>
class PrintingVisitor : public ConstraintVisitor<ValueType>
{
public:
    PrintingVisitor(QTextStream &stream)
        : m_stream(stream)
    {}

    void visit(const RangeConstraint<ValueType> &constraint) override;
    void visit(const AndConstraint<ValueType> &constraint) override;
    void visit(const OrConstraint<ValueType> &constraint) override;
    void visit(const FromConstraint<ValueType> &constraint) override;
    void visit(const SizeConstraint<ValueType> &constraint) override;
    void visit(const ConstraintList<ValueType> &constraint) override;

private:
    QTextStream &m_stream;
};

template<typename ValueType>
inline QString toString(const Constraint<ValueType> &c)
{
    QString str;
    QTextStream stream(&str);
    PrintingVisitor<ValueType> v(stream);
    c.accept(v);
    return str;
}

template<typename ValueType>
void PrintingVisitor<ValueType>::visit(const RangeConstraint<ValueType> &constraint)
{
    const auto &range = constraint.range();
    if (range.isSingleItem())
        m_stream << ValueType::asString(range.begin());
    else
        m_stream << ValueType::asString(range.begin()) << " .. "
                 << ValueType::asString(range.end());
}

template<typename ValueType>
void PrintingVisitor<ValueType>::visit(const AndConstraint<ValueType> &constraint)
{
    m_stream << "(";
    constraint.leftChild().accept(*this);
    m_stream << " ^ ";
    constraint.rightChild().accept(*this);
    m_stream << ")";
}

template<typename ValueType>
void PrintingVisitor<ValueType>::visit(const OrConstraint<ValueType> &constraint)
{
    m_stream << "(";
    constraint.leftChild().accept(*this);
    m_stream << " | ";
    constraint.rightChild().accept(*this);
    m_stream << ")";
}

template<typename ValueType>
void PrintingVisitor<ValueType>::visit(const FromConstraint<ValueType> &constraint)
{
    m_stream << "FROM(";
    PrintingVisitor<Data::StringValue> v(m_stream);
    constraint.innerConstraints().accept(v);
    m_stream << ")";
}

template<typename ValueType>
void PrintingVisitor<ValueType>::visit(const SizeConstraint<ValueType> &constraint)
{
    m_stream << "SIZE(";
    PrintingVisitor<Data::IntegerValue> v(m_stream);
    constraint.innerConstraints().accept(v);
    m_stream << ")";
}

template<typename ValueType>
void PrintingVisitor<ValueType>::visit(const ConstraintList<ValueType> &constraint)
{
    bool first = true;
    for (const auto &c : constraint.constraints()) {
        if (!first)
            m_stream << ' ';
        m_stream << '(';
        c->accept(*this);
        m_stream << ')';
        first = false;
    }
}

} // namespace Constraints
} // namespace Data
} // namespace Fuzzer
