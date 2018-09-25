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

#include <memory>

#include <QString>

namespace Fuzzer {
namespace Data {
namespace Constraints {

template<typename ValueType>
class ConstraintVisitor;

template<typename ValueType>
class Constraint
{
public:
    virtual ~Constraint() = default;

    virtual std::unique_ptr<Constraint<ValueType>> clone() const = 0;

    virtual void accept(ConstraintVisitor<ValueType> &visitor) const = 0;
};

} // namespace Constraints
} // namespace Data
} // namespace Fuzzer
