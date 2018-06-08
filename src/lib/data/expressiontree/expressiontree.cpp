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

#include "expressiontree.h"

#include <vector>

using namespace MalTester::Data::ExpressionTree;

class ExpressionTree::Root : public ExpressionNode
{
public:
    Root() = default;
    Root(const Root &other)
    {
        for (const auto &child : other.m_children)
            appendChild(child->clone());
    }

    std::unique_ptr<ExpressionNode> clone() const override { return std::make_unique<Root>(*this); }

    QString asString() const override
    {
        QString res;
        for (const auto &child : m_children)
            res += '(' + child->asString() + ')';

        return res;
    }

    void appendChild(std::unique_ptr<const ExpressionNode> child)
    {
        m_children.push_back(std::move(child));
    }

private:
    std::vector<std::unique_ptr<const ExpressionNode>> m_children;
};

ExpressionTree::ExpressionTree()
    : m_root(new Root)
{}

ExpressionTree::~ExpressionTree() {}

ExpressionTree::ExpressionTree(const ExpressionTree &other)
{
    m_root = std::make_unique<Root>(*other.m_root);
}

void ExpressionTree::appendSubtree(const ExpressionNode *node)
{
    m_root->appendChild(std::unique_ptr<const ExpressionNode>(node));
}

QString ExpressionTree::expression() const
{
    return m_root->asString();
}
