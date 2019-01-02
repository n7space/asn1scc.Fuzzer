/****************************************************************************
**
** Copyright (C) 2018-2019 N7 Space sp. z o. o.
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

#include "choicevalue.h"

using namespace Fuzzer::Data;

ChoiceValue::ChoiceValue(const QString &name, ValuePtr value)
    : m_name(name)
    , m_value(std::move(value))
{}

ChoiceValue::ChoiceValue(const ChoiceValue &other)
    : m_name(other.m_name)
    , m_value(other.m_value->clone())
{}

QString ChoiceValue::asString() const
{
    return m_name + QLatin1Char(':') + m_value->asString();
}
ValuePtr ChoiceValue::clone() const
{
    return std::make_unique<ChoiceValue>(*this);
}
