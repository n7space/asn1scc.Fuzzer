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

#include <QString>

#include "constraints.h"
#include "type.h"

namespace MalTester {
namespace Data {
namespace Types {

class SequenceOf : public Type, public WithConstraints<IntegerConstraints>
{
public:
    SequenceOf() = default;
    SequenceOf(const SequenceOf &other);

    QString name() const override { return QLatin1String("SEQUENCE OF"); }
    void accept(TypeVisitor &visitor) override;
    std::unique_ptr<Type> clone() const override;

    QString size() const { return m_size; }
    void setSize(const QString &size) { m_size = size; }

    const Type &itemsType() const { return *m_itemsType; }
    void setItemsType(std::unique_ptr<Type> itemsType) { m_itemsType = std::move(itemsType); }

private:
    QString m_size;
    std::unique_ptr<Type> m_itemsType;
};

} // namespace Types
} // namespace Data
} // namespace MalTester
