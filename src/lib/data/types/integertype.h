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

#include <data/acnparameters.h>
#include <data/constraints.h>

#include <data/types/builtintypes.h>

namespace MalTester {
namespace Data {
namespace Types {

class IntegerAcnParameters : public AcnParameters
{
public:
    IntegerAcnParameters()
        : m_size(0)
        , m_encoding(Encoding::undefined)
        , m_endianness(Endianness::undefined)
    {}

    void setSize(const int size) override { m_size = size; }
    int size() const override { return m_size; }

    void setEncoding(const Encoding encoding) override { m_encoding = encoding; }
    Encoding encoding() const override { return m_encoding; }

    void setEndianness(const Endianness endianness) override { m_endianness = endianness; }
    Endianness endianness() const override { return m_endianness; }

private:
    int m_size;
    Encoding m_encoding;
    Endianness m_endianness;
};

class Integer : public BuiltinType
{
public:
    Integer()
        : BuiltinType(std::make_unique<Constraints>(&Integer::toVariantPair),
                      std::make_unique<IntegerAcnParameters>())
    {}

    QString name() const override { return QLatin1String("INTEGER"); }

private:
    QString baseIconFile() const override
    {
        return QStringLiteral(":/asn1acn/images/outline/integer.png");
    }

    static Constraints::VariantPair toVariantPair(const Constraints::StringPair &range)
    {
        return {range.first.toInt(), range.second.toInt()};
    }
};

} // namespace Types
} // namespace Data
} // namespace MalTester
