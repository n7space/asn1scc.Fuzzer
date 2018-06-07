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

class LogicOperatorNode : public ExpressionNode
{
public:
    LogicOperatorNode(const QString &type,
                      const ExpressionNode *leftChild,
                      const ExpressionNode *rightChild);

    LogicOperatorNode(const LogicOperatorNode &other);

    std::unique_ptr<ExpressionNode> clone() const override;
    QString asString() const override;

private:
    enum class NodeType { AND, OR, UNKNOWN };
    static NodeType stringToOperatorType(const QString &name);

    NodeType m_type;
    std::unique_ptr<const ExpressionNode> m_leftChild;
    std::unique_ptr<const ExpressionNode> m_rightChild;
};

} // namespace ExpressionTree
} // namespace Data
} // namespace MalTester
