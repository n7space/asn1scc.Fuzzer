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

#include <functional>

#include <QList>
#include <QPair>

namespace MalTester {
namespace Data {
namespace Types {

template<typename T>
class RangeConstraints
{
public:
    using Range = QPair<T, T>;
    using Ranges = QList<Range>;

    const Ranges &ranges() const { return m_ranges; }
    void addRange(const T &begin, const T &end) { m_ranges.push_back({begin, end}); }

private:
    Ranges m_ranges;
};

template<typename T>
class WithConstraints
{
public:
    WithConstraints() = default;
    WithConstraints(const WithConstraints &other) = default;

    T &constraints() { return m_constraints; }
    const T &constraints() const { return m_constraints; }

private:
    T m_constraints;
};

using IntegerConstraints = RangeConstraints<int>;
using RealConstraints = RangeConstraints<double>;
using EnumeratedConstraints = RangeConstraints<int>;
using StringConstraints = RangeConstraints<QString>;

} // namespace Types
} // namespace Data
} // namespace MalTester
