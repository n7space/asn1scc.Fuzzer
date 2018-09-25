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

#include <QString>

#include "type.h"

namespace Fuzzer {
namespace Data {
namespace Types {

enum class IntegerEncoding {
    pos_int,
    twos_complement,
    ASCII,
    BCD,
    unspecified,
};

class IntegerAcnParameters
{
public:
    IntegerAcnParameters();
    IntegerAcnParameters(const IntegerAcnParameters &other) = default;

    void setSize(const int size) { m_size = size; }
    int size() const { return m_size; }

    int acnMinSizeInBits() const { return m_acnMinSizeInBits; }
    void setAcnMinSizeInBits(int s) { m_acnMinSizeInBits = s; }

    int acnMaxSizeInBits() const { return m_acnMaxSizeInBits; }
    void setAcnMaxSizeInBits(int s) { m_acnMaxSizeInBits = s; }

    void setEncoding(const IntegerEncoding encoding) { m_encoding = encoding; }
    IntegerEncoding encoding() const { return m_encoding; }

    void setEndianness(const Endianness endianness) { m_endianness = endianness; }
    Endianness endianness() const { return m_endianness; }

    static IntegerEncoding mapEncoding(QStringRef in);
    static IntegerEncoding mapEncoding(const QString &in) { return mapEncoding(&in); }
    static QString encodingToString(IntegerEncoding encoding);

private:
    IntegerEncoding m_encoding;
    Endianness m_endianness;
    int m_size;
    int m_acnMinSizeInBits;
    int m_acnMaxSizeInBits;
};

} // namespace Types
} // namespace Data
} // namespace Fuzzer
