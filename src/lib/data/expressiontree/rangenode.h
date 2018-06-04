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

#include <QString>

#include <data/expressiontree/expressionnode.h>

namespace MalTester {
namespace Data {
namespace ExpressionTree {

template<typename T>
class RangeNode : public ExpressionNode<T>
{
public:
    RangeNode(const T &range)
        : m_range(range)
    {}

    ~RangeNode() override = default;

    std::unique_ptr<ExpressionNode<T>> clone() const override
    {
        return std::make_unique<RangeNode<T>>(*this);
    }

    void appendChild(std::unique_ptr<ExpressionNode<T>> child) override { Q_UNUSED(child); }
    bool isFull() const override { return true; }
    QString asString() const override { return m_range->asString(); }

private:
    const T m_range;
};

} // namespace ExpressionTree
} // namespace Data
} // namespace MalTester
