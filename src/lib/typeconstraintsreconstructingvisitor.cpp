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

#include "typeconstraintsreconstructingvisitor.h"

using namespace MalTester;

QString TypeConstraintsReconstructingVisitor::valueFor(Data::Types::Boolean &type) const
{
    Q_UNUSED(type);
    return {};
}
QString TypeConstraintsReconstructingVisitor::valueFor(Data::Types::Null &type) const
{
    Q_UNUSED(type);
    return {};
}

QString TypeConstraintsReconstructingVisitor::valueFor(Data::Types::BitString &type) const
{
    return type.constraints().rangesTree().expression();
}

QString TypeConstraintsReconstructingVisitor::valueFor(Data::Types::OctetString &type) const
{
    return type.constraints().rangesTree().expression();
}

QString TypeConstraintsReconstructingVisitor::valueFor(Data::Types::IA5String &type) const
{
    return type.constraints().rangesTree().expression();
}

QString TypeConstraintsReconstructingVisitor::valueFor(Data::Types::NumericString &type) const
{
    return type.constraints().rangesTree().expression();
}

QString TypeConstraintsReconstructingVisitor::valueFor(Data::Types::Enumerated &type) const
{
    return type.constraints().rangesTree().expression();
}

QString TypeConstraintsReconstructingVisitor::valueFor(Data::Types::Choice &type) const
{
    Q_UNUSED(type);
    return {};
}

QString TypeConstraintsReconstructingVisitor::valueFor(Data::Types::Sequence &type) const
{
    Q_UNUSED(type);
    return {};
}

QString TypeConstraintsReconstructingVisitor::valueFor(Data::Types::SequenceOf &type) const
{
    return type.constraints().rangesTree().expression();
}

QString TypeConstraintsReconstructingVisitor::valueFor(Data::Types::Real &type) const
{
    return type.constraints().rangesTree().expression();
}

QString TypeConstraintsReconstructingVisitor::valueFor(Data::Types::LabelType &type) const
{
    Q_UNUSED(type);
    return {};
}

QString TypeConstraintsReconstructingVisitor::valueFor(Data::Types::Integer &type) const
{
    return type.constraints().rangesTree().expression();
}

QString TypeConstraintsReconstructingVisitor::valueFor(Data::Types::UserdefinedType &type) const
{
    TypeConstraintsReconstructingVisitor visitor;
    type.type().accept(visitor);

    return visitor.value();
}
