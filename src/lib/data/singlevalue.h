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

#include <functional>
#include <memory>

#include <data/value.h>
#include <data/valueprinters.h>

#include <QString>

namespace MalTester {
namespace Data {

class SingleValue : public Value
{
public:
    SingleValue(const QString &value, std::function<QString(const QString &)> printer = printAsSelf);
    SingleValue(const SingleValue &other) = default;

    QString asString() const override;
    ValuePtr clone() const override;

private:
    QString m_value;
    std::function<QString(const QString &)> m_printer;
};

} // namespace Data
} // namespace MalTester