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

#include <data/types/type.h>

namespace MalTester {
namespace Data {
namespace Types {

enum class Endianness { big, little, unspecified };

enum class IntegerEncoding {
    pos_int,
    twos_complement,
    ASCII,
    BCD,
    unspecified,
};

class Integer : public Type
{
public:
    Integer();
    Integer(const Integer &other) = default;

    QString name() const override;
    void accept(TypeVisitor &visitor) override;
    std::unique_ptr<Type> clone() const override;

    void setSize(const int size) { m_size = size; }
    int size() const { return m_size; }

    void setEncoding(const IntegerEncoding encoding) { m_encoding = encoding; }
    IntegerEncoding encoding() const { return m_encoding; }

    void setEndianness(const Endianness endianness) { m_endianness = endianness; }
    Endianness endianness() const { return m_endianness; }

    static IntegerEncoding mapEncoding(const QStringRef &in);
    static Endianness mapEndianess(const QStringRef &in);

private:
    IntegerEncoding m_encoding;
    Endianness m_endianness;
    int m_size;
};

} // namespace Types
} // namespace Data
} // namespace MalTester
