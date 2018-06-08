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

#include "typevisitor.h"

namespace MalTester {
namespace Data {
namespace Types {

template<typename Value>
class TypeVisitorWithValue : public TypeVisitor
{
public:
    using ValueType = Value;

    const Value &value() const { return m_value; }

    void visit(Boolean &type) override final { m_value = valueFor(type); }
    void visit(Null &type) override final { m_value = valueFor(type); }
    void visit(BitString &type) override final { m_value = valueFor(type); }
    void visit(OctetString &type) override final { m_value = valueFor(type); }
    void visit(IA5String &type) override final { m_value = valueFor(type); }
    void visit(NumericString &type) override final { m_value = valueFor(type); }
    void visit(Enumerated &type) override final { m_value = valueFor(type); }
    void visit(Choice &type) override final { m_value = valueFor(type); }
    void visit(Sequence &type) override final { m_value = valueFor(type); }
    void visit(SequenceOf &type) override final { m_value = valueFor(type); }
    void visit(Real &type) override final { m_value = valueFor(type); }
    void visit(LabelType &type) override final { m_value = valueFor(type); }
    void visit(Integer &type) override final { m_value = valueFor(type); }
    void visit(UserdefinedType &type) override final { m_value = valueFor(type); }

protected:
    virtual Value valueFor(Boolean &type) const = 0;
    virtual Value valueFor(Null &type) const = 0;
    virtual Value valueFor(BitString &type) const = 0;
    virtual Value valueFor(OctetString &type) const = 0;
    virtual Value valueFor(IA5String &type) const = 0;
    virtual Value valueFor(NumericString &type) const = 0;
    virtual Value valueFor(Enumerated &type) const = 0;
    virtual Value valueFor(Choice &type) const = 0;
    virtual Value valueFor(Sequence &type) const = 0;
    virtual Value valueFor(SequenceOf &type) const = 0;
    virtual Value valueFor(Real &type) const = 0;
    virtual Value valueFor(LabelType &type) const = 0;
    virtual Value valueFor(Integer &type) const = 0;
    virtual Value valueFor(UserdefinedType &type) const = 0;

private:
    Value m_value;
};

} // namespace Types
} // namespace Data
} // namespace MalTester
