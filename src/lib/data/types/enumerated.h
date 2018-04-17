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

#include <QHash>
#include <QString>

#include <data/sourcelocation.h>

#include <data/types/constraints.h>
#include <data/types/type.h>

namespace MalTester {
namespace Data {
namespace Types {

class EnumeratedItem
{
public:
    EnumeratedItem() = default;

    EnumeratedItem(const QString &name, int value, const SourceLocation &location)
        : m_name(name)
        , m_value(value)
        , m_location(location)
    {}

    EnumeratedItem(const EnumeratedItem &other) = default;

    const QString &name() const { return m_name; }
    int value() const { return m_value; }
    const SourceLocation &location() const { return m_location; }

private:
    QString m_name;
    int m_value;
    SourceLocation m_location;
};

class Enumerated : public Type, public WithConstraints<EnumeratedConstraints>
{
public:
    Enumerated() = default;
    Enumerated(const Enumerated &other) = default;

    QString name() const override;
    void accept(TypeVisitor &visitor) override;
    std::unique_ptr<Type> clone() const override;

    using Items = QHash<QString, EnumeratedItem>;

    const Items &items() const { return m_items; }
    void addItem(const QString &key, const EnumeratedItem &item);

private:
    Items m_items;
};

} // namespace Types
} // namespace Data
} // namespace MalTester
