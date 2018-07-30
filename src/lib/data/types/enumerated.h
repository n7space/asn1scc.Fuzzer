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

#include <QMap>
#include <QString>

#include <data/sourcelocation.h>

#include <data/types/integeracnparams.h>
#include <data/types/type.h>

#include <data/constraints/withconstraints.h>

#include <data/range.h>

namespace MalTester {
namespace Data {
namespace Types {

class EnumeratedItem
{
public:
    EnumeratedItem() = default;

    EnumeratedItem(int index, const QString &name, int value, const SourceLocation &location = {})
        : m_index(index)
        , m_name(name)
        , m_value(value)
        , m_location(location)
    {}

    const QString &name() const { return m_name; }
    int value() const { return m_value; }
    int index() const { return m_index; }
    const SourceLocation &location() const { return m_location; }

private:
    int m_index;
    QString m_name;
    int m_value;
    SourceLocation m_location;
};

class Enumerated : public Type,
                   public IntegerAcnParameters,
                   public Constraints::WithConstraints<Data::EnumValue>
{
public:
    Enumerated();
    Enumerated(const Enumerated &other) = default;

    QString name() const override;
    void accept(TypeMutatingVisitor &visitor) override;
    void accept(TypeReadingVisitor &visitor) const override;
    std::unique_ptr<Type> clone() const override;

    using Items = QMap<QString, EnumeratedItem>;

    const Items &items() const { return m_items; }
    void addItem(const QString &key, const EnumeratedItem &item);

    void setEncodeValues(bool encodeValues) { m_encodeValues = encodeValues; }
    bool encodeValues() const { return m_encodeValues; }

    int mapToValue(const QString &key) const;
    Range<int64_t> mapToValue(const Range<QString> &r) const;

private:
    Items m_items;
    bool m_encodeValues;
};

} // namespace Types
} // namespace Data
} // namespace MalTester
