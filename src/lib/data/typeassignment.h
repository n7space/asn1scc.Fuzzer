/****************************************************************************
**
** Copyright (C) 2018 N7 Space sp. z o. o.
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

#include <memory>

#include <QString>

#include "node.h"
#include "sourcelocation.h"
#include "types/type.h"

namespace Fuzzer {
namespace Data {

class TypeAssignment : public Node
{
public:
    TypeAssignment(const QString &name,
                   const QString &cName,
                   const SourceLocation &location,
                   std::unique_ptr<Types::Type> type);
    TypeAssignment(const TypeAssignment &other);
    ~TypeAssignment() override;

    const QString &cName() const { return m_cName; }

    void accept(Visitor &visitor) const override;
    void accept(MutatingVisitor &visitor) override;

    const Types::Type *type() const { return m_type.get(); }
    Types::Type *type() { return m_type.get(); }

private:
    QString m_cName;
    std::unique_ptr<Types::Type> m_type;
};

} // namespace Data
} // namespace Fuzzer
