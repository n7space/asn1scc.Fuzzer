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

#include "logicoperator.h"

using namespace MalTester::Data::ExpressionTree;

LogicOperator::LogicOperator(const QString &type,
                             const ExpressionNode *leftChild,
                             const ExpressionNode *rightChild)
    : m_type(stringToOperatorType(type))
    , m_leftChild(leftChild)
    , m_rightChild(rightChild)
{}

LogicOperator::LogicOperator(const LogicOperator &other)
    : m_type(other.m_type)
{
    m_leftChild = other.m_leftChild ? other.m_leftChild->clone() : nullptr;
    m_rightChild = other.m_rightChild ? other.m_rightChild->clone() : nullptr;
}

std::unique_ptr<ExpressionNode> LogicOperator::clone() const
{
    return std::make_unique<LogicOperator>(*this);
}

QString LogicOperator::LogicOperator::asString() const
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

LogicOperator::NodeType LogicOperator::LogicOperator::stringToOperatorType(const QString &name)
{
    if (name == QStringLiteral("OR"))
        return LogicOperator::NodeType::OR;
    if (name == QStringLiteral("AND"))
        return LogicOperator::NodeType::AND;

    return LogicOperator::NodeType::UNKNOWN;
}
