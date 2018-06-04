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
#include <vector>

#include <QString>

#include <data/expressiontree/expressionnode.h>

namespace MalTester {
namespace Data {
namespace ExpressionTree {

template<typename T>
class RootNode : public ExpressionNode<T>
{
public:
    RootNode() = default;
    RootNode(const RootNode &other)
    {
        for (const auto &child : other.m_children)
            appendChild(child->clone());
    }

    std::unique_ptr<ExpressionNode<T>> clone() const override
    {
        return std::make_unique<RootNode<T>>(*this);
    }

    void appendChild(std::unique_ptr<ExpressionNode<T>> child) override;
    bool isFull() const override;
    QString asString() const override;

private:
    std::vector<std::unique_ptr<ExpressionNode<T>>> m_children;
};

template<typename T>
inline QString RootNode<T>::asString() const
{
    QString res;
    for (const auto &child : m_children)
        res += '(' + child->asString() + ')';

    return res;
}

template<typename T>
inline void RootNode<T>::appendChild(std::unique_ptr<ExpressionNode<T>> child)
{
    m_children.push_back(std::move(child));
}

template<typename T>
inline bool RootNode<T>::isFull() const
{
    return false;
}

} // namespace ExpressionTree
} // namespace Data
} // namespace MalTester
