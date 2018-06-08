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

#include "constrainingoperator.h"

using namespace MalTester::Data::ExpressionTree;

ConstrainingOperator::ConstrainingOperator(const QString &type, const ExpressionNode *child)
    : m_type(stringToOperatorType(type))
    , m_child(child)
{}

ConstrainingOperator::ConstrainingOperator(const ConstrainingOperator &other)
{
    m_child = other.m_child ? other.m_child->clone() : nullptr;
}

std::unique_ptr<ExpressionNode> ConstrainingOperator::clone() const
{
    return std::make_unique<ConstrainingOperator>(*this);
}

QString ConstrainingOperator::asString() const
{
    if (m_type == NodeType::SIZE)
        return QStringLiteral("(SIZE (") + m_child->asString() + QStringLiteral("))");

    if (m_type == NodeType::FROM)
        return QStringLiteral("(FROM (") + m_child->asString() + QStringLiteral("))");

    return {};
}

ConstrainingOperator::NodeType ConstrainingOperator::stringToOperatorType(const QString &name)
{
    if (name == QStringLiteral("SIZE"))
        return NodeType::SIZE;
    if (name == QStringLiteral("ALPHA"))
        return NodeType::FROM;

    return NodeType::UNKNOWN;
}
