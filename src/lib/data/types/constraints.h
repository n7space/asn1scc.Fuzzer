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

#include <data/expressiontree/expressiontree.h>
#include <data/ranges.h>

namespace MalTester {
namespace Data {
namespace Types {

class RangeConstraints
{
public:
    using RangesTree = ExpressionTree::ExpressionTree<const Range *>;

    const RangesTree &rangesTree() const { return m_rangesTree; }

    void addRange(const Range *range) { m_rangesTree.addRange(range); }
    void addOperator(const QString &type) { m_rangesTree.addOperator(type); }

private:
    RangesTree m_rangesTree;
};

class WithConstraints
{
public:
    WithConstraints() = default;
    WithConstraints(const WithConstraints &other) = default;

    RangeConstraints &constraints() { return m_constraints; }
    const RangeConstraints &constraints() const { return m_constraints; }

private:
    RangeConstraints m_constraints;
};

} // namespace Types
} // namespace Data
} // namespace MalTester
