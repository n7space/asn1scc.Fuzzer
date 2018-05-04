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

#include <data/acnparameter.h>
#include <data/sourcelocation.h>

#include <data/types/type.h>

namespace MalTester {
namespace Data {
namespace Types {

class SequenceComponent
{
public:
    SequenceComponent() = default;
    SequenceComponent(const QString &name,
                      const QString &presentWhen,
                      const SourceLocation &location,
                      std::unique_ptr<Type> type);

    SequenceComponent(const SequenceComponent &other);

    const QString &name() const { return m_name; }
    const SourceLocation &location() const { return m_location; }
    const Type &type() const { return *m_type; }
    const QString &presentWhen() const { return m_presentWhen; }

private:
    QString m_name;
    QString m_presentWhen;
    SourceLocation m_location;
    std::unique_ptr<Type> m_type;
};

class Sequence : public Type
{
public:
    Sequence() = default;
    Sequence(const Sequence &other);

    QString name() const override { return QLatin1String("SEQUENCE"); }
    void accept(TypeVisitor &visitor) override;
    std::unique_ptr<Type> clone() const override;

    using Components = std::map<QString, SequenceComponent>;

    const Components &components() const { return m_components; }
    void addComponent(const QString &key, const SequenceComponent &component);

    const AcnParameter::AcnParameterPtrs &acnParameters() const { return m_parameters; }
    void addParameter(std::unique_ptr<AcnParameter> parameter);

private:
    Components m_components;
    AcnParameter::AcnParameterPtrs m_parameters;
};

} // namespace Types
} // namespace Data
} // namespace MalTester
