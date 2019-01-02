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

namespace Fuzzer {
namespace Data {
namespace Types {

class Boolean;
class Null;
class BitString;
class OctetString;
class IA5String;
class NumericString;
class Enumerated;
class Choice;
class Sequence;
class SequenceOf;
class Real;
class LabelType;
class Integer;
class UserdefinedType;

class TypeMutatingVisitor
{
public:
    virtual ~TypeMutatingVisitor();

    virtual void visit(Boolean &type) = 0;
    virtual void visit(Null &type) = 0;
    virtual void visit(BitString &type) = 0;
    virtual void visit(OctetString &type) = 0;
    virtual void visit(IA5String &type) = 0;
    virtual void visit(NumericString &type) = 0;
    virtual void visit(Enumerated &type) = 0;
    virtual void visit(Choice &type) = 0;
    virtual void visit(Sequence &type) = 0;
    virtual void visit(SequenceOf &type) = 0;
    virtual void visit(Real &type) = 0;
    virtual void visit(LabelType &type) = 0;
    virtual void visit(Integer &type) = 0;
    virtual void visit(UserdefinedType &type) = 0;
};

} // namespace Types
} // namespace Data
} // namespace Fuzzer
