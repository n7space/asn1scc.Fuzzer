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
#include "integerranges.h"

using namespace MalTester::Data;
using namespace MalTester::Data::Types;

namespace {

int nineRepeated(int times)
{
    if (times == 1)
        return 9;
    return nineRepeated(times - 1) * 10 + 9;
}

} // namespace

Range<int> MalTester::Data::maxValueRangeFor(const IntegerAcnParameters &type)
{
    switch (type.encoding()) {
    case IntegerEncoding::pos_int:
        return {0, (1 << type.size()) - 1};
    case IntegerEncoding::twos_complement:
        return {-(1 << (type.size() - 1)), (1 << (type.size() - 1)) - 1};
    case IntegerEncoding::ASCII:
        return {-nineRepeated((type.size() / 8) - 1), nineRepeated((type.size() / 8) - 1)};
    case IntegerEncoding::BCD:
        return {0, nineRepeated(type.size() / 4)};
    case IntegerEncoding::unspecified:
        break;
    }
    return {0, 0};
}
