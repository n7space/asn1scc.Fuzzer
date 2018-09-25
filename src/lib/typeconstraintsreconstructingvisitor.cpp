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
#include "typeconstraintsreconstructingvisitor.h"

#include <data/constraints/printingvisitor.h>

#include <data/types/acnparameterizablecomposite.h>
#include <data/types/bitstring.h>
#include <data/types/boolean.h>
#include <data/types/choice.h>
#include <data/types/enumerated.h>
#include <data/types/ia5string.h>
#include <data/types/integer.h>
#include <data/types/integeracnparams.h>
#include <data/types/null.h>
#include <data/types/numericstring.h>
#include <data/types/octetstring.h>
#include <data/types/real.h>
#include <data/types/sequence.h>
#include <data/types/sequenceof.h>
#include <data/types/userdefinedtype.h>

using namespace Fuzzer;

QString TypeConstraintsReconstructingVisitor::valueFor(const Data::Types::Boolean &type) const
{
    Q_UNUSED(type);
    return {};
}
QString TypeConstraintsReconstructingVisitor::valueFor(const Data::Types::Null &type) const
{
    Q_UNUSED(type);
    return {};
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Data::Types::BitString &type) const
{
    return toString(type.constraints());
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Data::Types::OctetString &type) const
{
    return toString(type.constraints());
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Data::Types::IA5String &type) const
{
    return toString(type.constraints());
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Data::Types::NumericString &type) const
{
    return toString(type.constraints());
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Data::Types::Enumerated &type) const
{
    return toString(type.constraints());
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Data::Types::Choice &type) const
{
    Q_UNUSED(type);
    return {};
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Data::Types::Sequence &type) const
{
    Q_UNUSED(type);
    return {};
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Data::Types::SequenceOf &type) const
{
    return toString(type.constraints());
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Data::Types::Real &type) const
{
    return toString(type.constraints());
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Data::Types::LabelType &type) const
{
    Q_UNUSED(type);
    return {};
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Data::Types::Integer &type) const
{
    return toString(type.constraints());
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Data::Types::UserdefinedType &type) const
{
    TypeConstraintsReconstructingVisitor visitor;
    type.type().accept(visitor);

    return visitor.value();
}
