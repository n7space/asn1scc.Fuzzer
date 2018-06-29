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

#include "typereadingvisitor.h"

namespace MalTester {
namespace Data {
namespace Types {

template<typename Value>
class TypeVisitorWithValue : public TypeReadingVisitor
{
public:
    using ValueType = Value;

    const Value &value() const { return m_value; }

    void visit(const Boolean &type) override final { m_value = valueFor(type); }
    void visit(const Null &type) override final { m_value = valueFor(type); }
    void visit(const BitString &type) override final { m_value = valueFor(type); }
    void visit(const OctetString &type) override final { m_value = valueFor(type); }
    void visit(const IA5String &type) override final { m_value = valueFor(type); }
    void visit(const NumericString &type) override final { m_value = valueFor(type); }
    void visit(const Enumerated &type) override final { m_value = valueFor(type); }
    void visit(const Choice &type) override final { m_value = valueFor(type); }
    void visit(const Sequence &type) override final { m_value = valueFor(type); }
    void visit(const SequenceOf &type) override final { m_value = valueFor(type); }
    void visit(const Real &type) override final { m_value = valueFor(type); }
    void visit(const LabelType &type) override final { m_value = valueFor(type); }
    void visit(const Integer &type) override final { m_value = valueFor(type); }
    void visit(const UserdefinedType &type) override final { m_value = valueFor(type); }

protected:
    virtual Value valueFor(const Boolean &type) const = 0;
    virtual Value valueFor(const Null &type) const = 0;
    virtual Value valueFor(const BitString &type) const = 0;
    virtual Value valueFor(const OctetString &type) const = 0;
    virtual Value valueFor(const IA5String &type) const = 0;
    virtual Value valueFor(const NumericString &type) const = 0;
    virtual Value valueFor(const Enumerated &type) const = 0;
    virtual Value valueFor(const Choice &type) const = 0;
    virtual Value valueFor(const Sequence &type) const = 0;
    virtual Value valueFor(const SequenceOf &type) const = 0;
    virtual Value valueFor(const Real &type) const = 0;
    virtual Value valueFor(const LabelType &type) const = 0;
    virtual Value valueFor(const Integer &type) const = 0;
    virtual Value valueFor(const UserdefinedType &type) const = 0;

private:
    Value m_value;
};

} // namespace Types
} // namespace Data
} // namespace MalTester
