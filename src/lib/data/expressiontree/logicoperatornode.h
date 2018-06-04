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
class LogicOperatorNode : public ExpressionNode<T>
{
public:
    LogicOperatorNode(const QString &type)
        : m_type(stringToOperatorType(type))
        , m_leftChild(nullptr)
        , m_rightChild(nullptr)
    {}

    LogicOperatorNode(const LogicOperatorNode &other)
    {
        m_leftChild = other.m_leftChild ? other.m_leftChild->clone() : nullptr;
        m_rightChild = other.m_rightChild ? other.m_rightChild->clone() : nullptr;
    }

    std::unique_ptr<ExpressionNode<T>> clone() const override
    {
        return std::make_unique<LogicOperatorNode<T>>(*this);
    }

    void appendChild(std::unique_ptr<ExpressionNode<T>> child) override;
    bool isFull() const override;
    QString asString() const override;

private:
    enum class NodeType { AND, OR, UNKNOWN };
    static NodeType stringToOperatorType(const QString &name);

    NodeType m_type;
    std::unique_ptr<ExpressionNode<T>> m_leftChild;
    std::unique_ptr<ExpressionNode<T>> m_rightChild;
};

template<typename T>
inline void LogicOperatorNode<T>::appendChild(std::unique_ptr<ExpressionNode<T>> child)
{
    if (!m_leftChild)
        m_leftChild = std::move(child);
    else
        m_rightChild = std::move(child);
}

template<typename T>
inline bool LogicOperatorNode<T>::isFull() const
{
    return m_leftChild && m_rightChild;
}

template<typename T>
inline QString LogicOperatorNode<T>::asString() const
{
    QString left;
    QString middle;
    QString right;

    if (m_leftChild != nullptr)
        left = m_leftChild->asString();

    if (m_type == NodeType::AND)
        middle = QStringLiteral(" ^ ");
    else if (m_type == NodeType::OR)
        middle = QStringLiteral(" | ");

    if (m_rightChild != nullptr)
        right = m_rightChild->asString();

    return QLatin1Char('(') + left + middle + right + QLatin1Char(')');
}

template<typename T>
inline typename LogicOperatorNode<T>::NodeType LogicOperatorNode<T>::stringToOperatorType(
    const QString &name)
{
    if (name == QStringLiteral("OR"))
        return LogicOperatorNode<T>::NodeType::OR;
    if (name == QStringLiteral("AND"))
        return LogicOperatorNode<T>::NodeType::AND;

    return LogicOperatorNode<T>::NodeType::UNKNOWN;
}

} // namespace ExpressionTree
} // namespace Data
} // namespace MalTester
