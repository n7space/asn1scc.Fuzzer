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

#include <QStringList>

namespace MalTester {
namespace Cases {

// TODO rename file
// TODO rename class (MalformedValueTestCase?)

using FieldPath = QStringList;

struct FieldAssignment
{
    FieldPath m_path;
    QString m_value;
};

class TestCase
{
public:
    explicit TestCase(const QString &name)
        : m_name(name)
    {}

    const QString &name() const { return m_name; }
    const QString &typeUnderTest() const { return m_typeUnderTest; }
    const FieldAssignment &fieldAssignment() const { return m_fieldAssignment; }

    void setAssignment(const FieldAssignment &a) { m_fieldAssignment = a; }
    void setTypeUnderTest(const QString &t) { m_typeUnderTest = t; }

private:
    QString m_name;
    QString m_typeUnderTest;
    FieldAssignment m_fieldAssignment;
};

} // namespace Cases
} // namespace MalTester
