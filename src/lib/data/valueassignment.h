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

#include "value.h"

namespace Fuzzer {
namespace Data {

class ValueAssignment : public Node
{
public:
    ValueAssignment(const QString &name,
                    const SourceLocation &location,
                    std::unique_ptr<Types::Type> type,
                    ValuePtr value);
    ValueAssignment(const ValueAssignment &other);
    ~ValueAssignment() override;

    void accept(Visitor &visitor) const override;
    void accept(MutatingVisitor &visitor) override;

    const Types::Type *type() const { return m_type.get(); }
    const ValuePtr &value() const { return m_value; }

private:
    std::unique_ptr<Types::Type> m_type;
    ValuePtr m_value;
};

} // namespace Data
} // namespace Fuzzer
