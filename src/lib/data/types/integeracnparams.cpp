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
#include "integeracnparams.h"

using namespace Fuzzer::Data;
using namespace Fuzzer::Data::Types;

IntegerAcnParameters::IntegerAcnParameters()
    : m_encoding(IntegerEncoding::unspecified)
    , m_endianness(Endianness::unspecified)
    , m_size(0)
    , m_acnMinSizeInBits(0)
    , m_acnMaxSizeInBits(0)
{}

IntegerEncoding IntegerAcnParameters::mapEncoding(QStringRef in)
{
    if (in == "pos-int")
        return IntegerEncoding::pos_int;
    if (in == "twos-complement")
        return IntegerEncoding::twos_complement;
    if (in == "ASCII")
        return IntegerEncoding::ASCII;
    if (in == "BCD")
        return IntegerEncoding::BCD;
    return IntegerEncoding::unspecified;
}

QString IntegerAcnParameters::encodingToString(IntegerEncoding encoding)
{
    switch (encoding) {
    case IntegerEncoding::pos_int:
        return QStringLiteral("pos-int");
    case IntegerEncoding::twos_complement:
        return QStringLiteral("twos-complement");
    case IntegerEncoding::ASCII:
        return QStringLiteral("ASCII");
    case IntegerEncoding::BCD:
        return QStringLiteral("BCD");
    case IntegerEncoding::unspecified:
        return {};

    default:
        return {};
    }
}
