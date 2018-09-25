/****************************************************************************
**
** Copyright (C) 2018 N7 Space sp. z o. o.
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

class TypeReadingVisitor
{
public:
    virtual ~TypeReadingVisitor() = default;

    virtual void visit(const Boolean &type) = 0;
    virtual void visit(const Null &type) = 0;
    virtual void visit(const BitString &type) = 0;
    virtual void visit(const OctetString &type) = 0;
    virtual void visit(const IA5String &type) = 0;
    virtual void visit(const NumericString &type) = 0;
    virtual void visit(const Enumerated &type) = 0;
    virtual void visit(const Choice &type) = 0;
    virtual void visit(const Sequence &type) = 0;
    virtual void visit(const SequenceOf &type) = 0;
    virtual void visit(const Real &type) = 0;
    virtual void visit(const LabelType &type) = 0;
    virtual void visit(const Integer &type) = 0;
    virtual void visit(const UserdefinedType &type) = 0;
};

} // namespace Types
} // namespace Data
} // namespace Fuzzer
