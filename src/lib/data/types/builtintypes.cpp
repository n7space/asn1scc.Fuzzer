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

#include "builtintypes.h"
#include "integertype.h"

using namespace MalTester::Data::Types;

std::unique_ptr<Type> BuiltinType::createBuiltinType(const QString &name)
{
    if (name == QStringLiteral("BOOLEAN"))
        return std::make_unique<Boolean>();

    if (name == QStringLiteral("NULL"))
        return std::make_unique<Null>();

    if (name == QStringLiteral("INTEGER"))
        return std::make_unique<Integer>();

    if (name == QStringLiteral("REAL"))
        return std::make_unique<Real>();

    if (name == QStringLiteral("BIT_STRING"))
        return std::make_unique<BitString>();

    if (name == QStringLiteral("OCTET_STRING"))
        return std::make_unique<OctetString>();

    if (name == QStringLiteral("IA5String"))
        return std::make_unique<IA5String>();

    if (name == QStringLiteral("NumericString"))
        return std::make_unique<NumericString>();

    if (name == QStringLiteral("Enumerated"))
        return std::make_unique<Enumarated>();

    if (name == QStringLiteral("CHOICE"))
        return std::make_unique<Choice>();

    if (name == QStringLiteral("SEQUENCE"))
        return std::make_unique<Sequence>();

    if (name == QStringLiteral("SEQUENCE_OF"))
        return std::make_unique<SequenceOf>();

    return nullptr;
}
