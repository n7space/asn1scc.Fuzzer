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

#include <data/value.h>

namespace MalTester {
namespace Data {

inline QString printAsSelf(const QString &value)
{
    return value;
}

inline QString printAsASCIIString(const QString &value)
{
    return QLatin1Char('\"') + value + QLatin1Char('\"');
}

inline QString printAsBitString(const QString &value)
{
    // TODO: convert to bits here?
    return QLatin1Char('\'') + value + QLatin1String("\'B");
}

inline QString printAsHexString(const QString &value)
{
    // TODO: convert to hex here?
    return QLatin1Char('\'') + value + QLatin1String("\'H");
}

inline QString printInBooleanFormat(const QString &value)
{
    return value.toUpper();
}

} // namespace Data
} // namespace MalTester
