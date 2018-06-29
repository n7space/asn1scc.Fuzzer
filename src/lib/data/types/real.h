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

#include "constraints.h"
#include "type.h"

namespace MalTester {
namespace Data {
namespace Types {

enum class RealEncoding { IEEE754_1985_32, IEEE754_1985_64, unspecified };

class Real : public Type, public WithConstraints
{
public:
    Real();
    Real(const Real &other) = default;

    QString name() const override { return QLatin1String("REAL"); }
    void accept(TypeMutatingVisitor &visitor) override;
    void accept(TypeReadingVisitor &visitor) const override;
    std::unique_ptr<Type> clone() const override;

    void setEncoding(const RealEncoding encoding) { m_encoding = encoding; }
    RealEncoding encoding() const { return m_encoding; }

    void setEndianness(const Endianness endianness) { m_endianness = endianness; }
    Endianness endianness() const { return m_endianness; }

    static RealEncoding mapEncoding(const QStringRef &ref);

private:
    RealEncoding m_encoding;
    Endianness m_endianness;
};

} // namespace Types
} // namespace Data
} // namespace MalTester
