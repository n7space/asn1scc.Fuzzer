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

#include <data/sourcelocation.h>

#include <data/types/type.h>

namespace MalTester {
namespace Data {
namespace Types {

class ChoiceAlternative
{
public:
    ChoiceAlternative() = default;
    ChoiceAlternative(const QString &name,
                      const QString &presentWhenName,
                      const QString &adaName,
                      const QString &cName,
                      const QString &presentWhen,
                      const SourceLocation &location,
                      std::unique_ptr<Type> type);

    ChoiceAlternative(const ChoiceAlternative &other);

    const QString &name() const { return m_name; }
    const QString &presentWhenName() const { return m_presentWhenName; }
    const QString &adaName() const { return m_adaName; }
    const QString &cName() const { return m_cName; }
    const QString &presentWhen() const { return m_presentWhen; }
    const SourceLocation &location() const { return m_location; }
    const Type &type() const { return *m_type; }

private:
    QString m_name;
    QString m_presentWhenName;
    QString m_adaName;
    QString m_cName;
    QString m_presentWhen;

    SourceLocation m_location;
    std::unique_ptr<Type> m_type;
};

class Choice : public Type
{
public:
    Choice() = default;
    Choice(const Choice &other) = default;

    QString name() const override { return QLatin1String("CHOICE"); }
    void accept(TypeVisitor &visitor) override;
    std::unique_ptr<Type> clone() const override;

    using Alternatives = std::map<QString, ChoiceAlternative>;

    const Alternatives &alternatives() const { return m_alternatives; }
    void addAlternative(const QString &key, const ChoiceAlternative &alt);

private:
    Alternatives m_alternatives;
};

} // namespace Types
} // namespace Data
} // namespace MalTester
