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

#include "ranges.h"

using namespace MalTester::Data;
using namespace MalTester::Data::ExpressionTree;

IntegerRange::IntegerRange(const int &begin, const int &end)
    : TypedRange(begin, end)
{}

QString IntegerRange::asString() const
{
    if (m_data.first != m_data.second)
        return QString::number(m_data.first) + " .. " + QString::number(m_data.second);
    else
        return QString::number(m_data.first);
}

std::unique_ptr<ExpressionNode> IntegerRange::clone() const
{
    return std::make_unique<IntegerRange>(*this);
}

RealRange::RealRange(const double &begin, const double &end)
    : TypedRange(begin, end)
{}

QString RealRange::asString() const
{
    if (m_data.first != m_data.second)
        return QString::number(m_data.first) + " .. " + QString::number(m_data.second);
    else
        return QString::number(m_data.first);
}

std::unique_ptr<ExpressionNode> RealRange::clone() const
{
    return std::make_unique<RealRange>(*this);
}

EnumeratedRange::EnumeratedRange(const int &begin, const int &end)
    : TypedRange(begin, end)
{}

QString EnumeratedRange::asString() const
{
    if (m_data.first != m_data.second)
        return QString::number(m_data.first) + " .. " + QString::number(m_data.second);
    else
        return QString::number(m_data.first);
}

std::unique_ptr<ExpressionNode> EnumeratedRange::clone() const
{
    return std::make_unique<EnumeratedRange>(*this);
}

StringRange::StringRange(const QString &begin, const QString &end)
    : TypedRange(begin, end)
{}

std::unique_ptr<ExpressionNode> StringRange::clone() const
{
    return std::make_unique<StringRange>(*this);
}

QString StringRange::asString() const
{
    if (m_data.first != m_data.second)
        return '"' + m_data.first + "\" .. \"" + m_data.second + '"';
    else
        return '"' + m_data.first + '"';
}
