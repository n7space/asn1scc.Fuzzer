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

struct IntegerValue
{
    using Type = int;
    static QLatin1String astNodeName() { return QLatin1Literal("IntegerValue"); }
    static Type fromAstValue(const QString &value) { return value.toInt(); /* TODO error check?*/ }
    static QString asString(Type t) { return QString::number(t); }
};
struct RealValue
{
    using Type = double;
    static QLatin1String astNodeName() { return QLatin1Literal("RealValue"); }
    static Type fromAstValue(const QString &value) { return value.toDouble(); /* TODO error?*/ }
    static QString asString(Type t) { return QString::number(t); }
};

struct EnumValue
{
    using Type = QString;
    static QLatin1String astNodeName() { return QLatin1Literal("EnumValue"); }
    static Type fromAstValue(const QString &value) { return value; }
    static QString asString(Type t) { return t; }
};
struct BooleanValue
{
    using Type = bool;
    static QLatin1String astNodeName() { return QLatin1Literal("BooleanValue"); }
    static Type fromAstValue(const QString &value) { return value.toUpper() == "TRUE"; }
    static QString asString(Type t) { return t ? "TRUE" : "FALSE"; }
    static QString reformatString(const QString &value) { return asString(fromAstValue(value)); }
};

struct StringValue
{
    using Type = QString;
    static QLatin1String astNodeName() { return QLatin1Literal("StringValue"); }
    static Type fromAstValue(const QString &value) { return value; }
    static QString asString(Type t) { return '"' + t + '"'; }
};

struct BitStringValue
{
    using Type = QString;
    static QLatin1String astNodeName() { return QLatin1Literal("BitStringValue"); }
    static Type fromAstValue(const QString &value) { return value; }
    static QString asString(Type t) { return "'" + t + "'B"; }
};
struct OctetStringValue
{
    using Type = QString;
    static QLatin1String astNodeName() { return QLatin1Literal("OctetStringValue"); }
    static Type fromAstValue(const QString &value) { return value; }
    static QString asString(Type t) { return "'" + t + "'H"; }
};

} // namespace Data
} // namespace MalTester
