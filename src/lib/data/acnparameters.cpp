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

#include "acnparameters.h"

using namespace MalTester::Data;

Encoding AcnParameters::mapEncoding(const QStringRef &in)
{
    if (in == "pos-int")
        return Encoding::pos_int;
    else if (in == "twos-complement")
        return Encoding::twos_complement;
    else if (in == "ASCII")
        return Encoding::ASCII;
    else if (in == "BCD")
        return Encoding::BCD;
    else if (in == "IEEE754_1985_32")
        return Encoding::IEEE754_1985_32;
    else if (in == "IEEE754_1985_64")
        return Encoding::IEEE754_1985_64;

    return Encoding::undefined;
}

Endianness AcnParameters::mapEndianess(const QStringRef &in)
{
    if (in == "big")
        return Endianness::big;
    else if (in == "little")
        return Endianness::little;

    return Endianness::undefined;
}

AlignToNext AcnParameters::mapAlignToNext(const QStringRef &in)
{
    if (in == "byte")
        return AlignToNext::byte;
    else if (in == "word")
        return AlignToNext::word;
    else if (in == "dword")
        return AlignToNext::dword;

    return AlignToNext::undefined;
}