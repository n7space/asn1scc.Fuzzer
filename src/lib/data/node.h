/****************************************************************************
**
** Copyright (C) 2018-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Fuzzer - Tool for generating test cases
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

#include <QString>

#include "sourcelocation.h"

namespace Fuzzer {
namespace Data {

class Visitor;
class MutatingVisitor;

class Node
{
protected:
    Node(const QString &name, const SourceLocation &location)
        : m_name(name)
        , m_location(location)
        , m_parent(nullptr)
    {}

    Node(const Node &other) = delete;

public:
    virtual ~Node();

    virtual void accept(Visitor &visitor) const = 0;
    virtual void accept(MutatingVisitor &visitor) = 0;

    template<typename VisitorType, typename... Args>
    typename VisitorType::ValueType valueFor(Args... args) const
    {
        VisitorType visitor(args...);
        this->accept(visitor);
        return visitor.value();
    }

    const QString &name() const { return m_name; }
    const SourceLocation &location() const { return m_location; }

    Node *parent() const { return m_parent; }
    void setParent(Node *parent) { m_parent = parent; }

private:
    QString m_name;
    SourceLocation m_location;
    Node *m_parent;
};

} // namespace Data
} // namespace Fuzzer
