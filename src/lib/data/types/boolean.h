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
#pragma once

#include <QString>

#include <data/types/type.h>

#include <data/constraints/withconstraints.h>

namespace Fuzzer {
namespace Data {
namespace Types {

class Boolean : public Type, public Constraints::WithConstraints<Data::BooleanValue>
{
public:
    Boolean() = default;
    Boolean(const Boolean &other) = default;

    QString name() const override { return QLatin1String("BOOLEAN"); }

    void accept(TypeMutatingVisitor &visitor) override;
    void accept(TypeReadingVisitor &visitor) const override;

    std::unique_ptr<Type> clone() const override;

    void setTrueValue(const QString &trueValue) { m_trueValue = trueValue; }
    const QString &trueValue() const { return m_trueValue; }

    void setFalseValue(const QString &falseValue) { m_falseValue = falseValue; }
    const QString &falseValue() const { return m_falseValue; }

private:
    QString m_trueValue;
    QString m_falseValue;
};

} // namespace Types
} // namespace Data
} // namespace Fuzzer
