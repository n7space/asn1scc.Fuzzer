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

namespace MalTester {
namespace Data {
namespace Constraints {

template<typename ValueType>
class ConstraintList;

template<typename ValueType>
class RangeConstraint;

template<typename ValueType>
class AndConstraint;

template<typename ValueType>
class OrConstraint;

template<typename ValueType>
class FromConstraint;

template<typename ValueType>
class SizeConstraint;

template<typename ValueType>
class ConstraintVisitor
{
public:
    virtual ~ConstraintVisitor() = default;

    virtual void visit(const RangeConstraint<ValueType> &constraint) = 0;
    virtual void visit(const AndConstraint<ValueType> &constraint) = 0;
    virtual void visit(const OrConstraint<ValueType> &constraint) = 0;
    virtual void visit(const FromConstraint<ValueType> &constraint) = 0;
    virtual void visit(const SizeConstraint<ValueType> &constraint) = 0;
    virtual void visit(const ConstraintList<ValueType> &constraint) = 0;
};

} // namespace Constraints
} // namespace Data
} // namespace MalTester
