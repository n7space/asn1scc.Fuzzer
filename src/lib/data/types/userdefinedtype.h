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

#include <vector>

#include <QString>

#include <data/acnargument.h>

#include "type.h"

namespace MalTester {
namespace Data {
namespace Types {

class UserdefinedType : public Type
{
public:
    UserdefinedType(const QString &name, const QString &module);
    UserdefinedType(const UserdefinedType &other);

    QString name() const override;
    void accept(TypeVisitor &visitor) override;
    std::unique_ptr<Type> clone() const override;

    const QString &module() const { return m_module; }

    const Type &type() const { return *m_type; }
    Type &type() { return *m_type; }
    void setType(std::unique_ptr<Type> type);

    const AcnArgumentPtrs &acnArguments() const { return m_arguments; }
    void addArgument(AcnArgumentPtr argument);

private:
    QString m_name;
    QString m_module;

    std::unique_ptr<Type> m_type;
    AcnArgumentPtrs m_arguments;
};

} // namespace Types
} // namespace Data
} // namespace MalTester
