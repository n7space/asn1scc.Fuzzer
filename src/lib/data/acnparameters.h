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

namespace MalTester {
namespace Data {

enum class Encoding {
    pos_int,
    twos_complement,
    ASCII,
    BCD,
    IEEE754_1985_32,
    IEEE754_1985_64,
    undefined
};

enum class Endianness { big, little, undefined };

enum class AlignToNext { byte, word, dword, undefined };

class AcnParameters
{
public:
    virtual ~AcnParameters() = 0;

    virtual void setSize(const int size) { Q_UNUSED(size); }
    virtual int size() const { return 0; }

    virtual void setEncoding(const Encoding encoding) { Q_UNUSED(encoding); }
    virtual Encoding encoding() const { return Encoding::undefined; }

    virtual void setEndianness(const Endianness endianness) { Q_UNUSED(endianness); }
    virtual Endianness endianness() const { return Endianness::undefined; }

    virtual void setAlignToNext(const AlignToNext alignToNext) { Q_UNUSED(alignToNext); }
    virtual AlignToNext alignToNext() const { return AlignToNext::undefined; }

    static Encoding mapEncoding(const QStringRef &in);
    static Endianness mapEndianess(const QStringRef &in);
    static AlignToNext mapAlignToNext(const QStringRef &in);
};

inline AcnParameters::~AcnParameters() {}

} // namespace Data
} // namespace MalTester
