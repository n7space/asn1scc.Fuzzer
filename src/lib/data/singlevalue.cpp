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

#include "singlevalue.h"

using namespace Fuzzer::Data;

SingleValue::SingleValue(const QString &value, std::function<QString(const QString &)> printer)
    : m_value(value)
    , m_printer(printer)
{}

QString SingleValue::asString() const
{
    if (m_printer)
        return m_printer(m_value);
    return m_value;
}

ValuePtr SingleValue::clone() const
{
    return std::make_unique<SingleValue>(*this);
}
