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

#include <data/mutatingvisitor.h>
#include <data/types/typemutatingvisitor.h>

namespace Fuzzer {
namespace Cases {

class ConstraintsRelaxingVisitor : public Data::Types::TypeMutatingVisitor,
                                   public Data::MutatingVisitor
{
public:
    ~ConstraintsRelaxingVisitor() override;

    void visit(Data::Types::Boolean &type) override;
    void visit(Data::Types::Null &type) override;
    void visit(Data::Types::BitString &type) override;
    void visit(Data::Types::OctetString &type) override;
    void visit(Data::Types::IA5String &type) override;
    void visit(Data::Types::NumericString &type) override;
    void visit(Data::Types::Enumerated &type) override;
    void visit(Data::Types::Choice &type) override;
    void visit(Data::Types::Sequence &type) override;
    void visit(Data::Types::SequenceOf &type) override;
    void visit(Data::Types::Real &type) override;
    void visit(Data::Types::LabelType &type) override;
    void visit(Data::Types::Integer &type) override;
    void visit(Data::Types::UserdefinedType &type) override;

    void visit(Data::Root &root) override;
    void visit(Data::Definitions &defs) override;
    void visit(Data::File &file) override;
    void visit(Data::TypeAssignment &type) override;
    void visit(Data::ValueAssignment &value) override;
    void visit(Data::Project &project) override;
};

} // namespace Cases
} // namespace Fuzzer
