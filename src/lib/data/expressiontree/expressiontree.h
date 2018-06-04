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
#include <stack>

#include <QString>

#include <data/expressiontree/constrainingoperatornode.h>
#include <data/expressiontree/expressionnode.h>
#include <data/expressiontree/logicoperatornode.h>
#include <data/expressiontree/rangenode.h>
#include <data/expressiontree/rootnode.h>

namespace MalTester {
namespace Data {
namespace ExpressionTree {

template<typename T>
class ExpressionTree
{
public:
    ExpressionTree()
        : m_root(new RootNode<T>)
    {}

    ExpressionTree(const ExpressionTree &other) { m_root = other.m_root->clone(); }

    void addRange(const T &range);
    void addOperator(const QString &type);

    QString expression() const;

private:
    std::unique_ptr<ExpressionNode<T>> m_root;
    std::stack<ExpressionNode<T> *> m_stack;
};

template<typename T>
inline void ExpressionTree<T>::addRange(const T &range)
{
    auto parent = m_stack.empty() ? m_root.get() : m_stack.top();

    parent->appendChild(std::make_unique<RangeNode<T>>(range));

    if (parent->isFull())
        m_stack.pop();
}

template<typename T>
static std::unique_ptr<ExpressionNode<T>> creatOperatorNode(const QString &type)
{
    if (type == QStringLiteral("OR") || type == QStringLiteral("AND"))
        return std::make_unique<LogicOperatorNode<T>>(type);

    if (type == QStringLiteral("SIZE") || type == QStringLiteral("ALPHA"))
        return std::make_unique<ConstrainingOperatorNode<T>>(type);

    return nullptr;
}

template<typename T>
inline void ExpressionTree<T>::addOperator(const QString &type)
{
    auto parent = m_stack.empty() ? m_root.get() : m_stack.top();

    auto child = creatOperatorNode<T>(type);
    auto childRaw = child.get();

    parent->appendChild(std::move(child));

    if (parent->isFull())
        m_stack.pop();

    m_stack.push(childRaw);
}

template<typename T>
inline QString ExpressionTree<T>::expression() const
{
    return m_root->asString();
}

} // namespace ExpressionTree
} // namespace Data
} // namespace MalTester
