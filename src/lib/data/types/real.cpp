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
#include "real.h"

#include "typevisitor.h"

using namespace MalTester::Data;
using namespace MalTester::Data::Types;

static Constraints::VariantPair toVariantPair(const Constraints::StringPair &range)
{
    return {range.first.toDouble(), range.second.toDouble()};
}

Real::Real()
    : Type(std::make_unique<Constraints>(toVariantPair))
{}

void Real::accept(TypeVisitor &visitor)
{
    visitor.visit(*this);
}

RealEncoding Real::mapEncoding(const QStringRef &in)
{
    if (in == "IEEE754_1985_32")
        return RealEncoding::IEEE754_1985_32;
    if (in == "IEEE754_1985_64")
        return RealEncoding::IEEE754_1985_64;
    return RealEncoding::unspecified;
}
