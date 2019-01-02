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

#include "namedvalue.h"

using namespace Fuzzer::Data;

NamedValue::NamedValue(const NamedValue &other)
{
    for (const auto &item : other.m_values) {
        // TODO condition shall be removed when proper SequenceValue will be used
        if (item.second != nullptr)
            addValue(item.first, item.second->clone());
        else
            addValue(item.first, nullptr);
    }
}

QString NamedValue::asString() const
{
    QString ret = QStringLiteral("{");

    for (auto it = m_values.begin(); it != m_values.end(); it++) {
        ret += it->first + QLatin1Char(' ') + it->second->asString();
        if (std::next(it, 1) != m_values.end())
            ret += QStringLiteral(", ");
    }
    ret += QStringLiteral("}");

    return ret;
}

ValuePtr NamedValue::clone() const
{
    return std::make_unique<NamedValue>(*this);
}

void NamedValue::addValue(const QString &name, ValuePtr value)
{
    m_values.push_back(std::pair<QString, ValuePtr>(name, std::move(value)));
}
