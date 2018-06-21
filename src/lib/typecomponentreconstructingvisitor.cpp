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

#include "typecomponentreconstructingvisitor.h"

#include "typeconstraintsreconstructingvisitor.h"

using namespace MalTester;

namespace {

const int INDENT_SIZE = 4;

void addIndent(QString &in, int indent)
{
    if (indent > 0)
        in.append(QString(indent, QChar(' ')));
}

void addWord(QString &in, QString word)
{
    in.append(word);
    in.append(QStringLiteral(" "));
}

void finishLine(QString &in)
{
    in.append(QStringLiteral("\n"));
}

void addLine(QString &in, QString line, int indent = 0)
{
    addIndent(in, indent);
    addWord(in, line);
    finishLine(in);
}

QString valueForStraightType(Data::Types::Type &type)
{
    TypeConstraintsReconstructingVisitor visitor;
    type.accept(visitor);

    return type.name() + visitor.value();
}

template<typename T>
QString valueForComplexType(const T &type, const int indent)
{
    QString out;
    addLine(out, type.name());
    addLine(out, QStringLiteral("{"), indent);

    const auto &components = type.components();
    for (auto it = components.begin(); it != components.end(); it++) {
        addIndent(out, indent + INDENT_SIZE);
        addWord(out, (*it)->name());

        TypeComponentReconstructingVisitor visitor(indent + INDENT_SIZE);
        (*it)->type().accept(visitor);
        addWord(out, visitor.value());

        if (std::next(it) != components.end())
            addWord(out, QStringLiteral(","));

        finishLine(out);
    }

    addIndent(out, indent);
    addWord(out, QStringLiteral("}"));

    return out;
}

} // namespace

TypeComponentReconstructingVisitor::TypeComponentReconstructingVisitor(int indent)
    : m_indent(indent)
{}

QString TypeComponentReconstructingVisitor::valueFor(Data::Types::Boolean &type) const
{
    return valueForStraightType(type);
}
QString TypeComponentReconstructingVisitor::valueFor(Data::Types::Null &type) const
{
    return valueForStraightType(type);
}

QString TypeComponentReconstructingVisitor::valueFor(Data::Types::BitString &type) const
{
    return valueForStraightType(type);
}

QString TypeComponentReconstructingVisitor::valueFor(Data::Types::OctetString &type) const
{
    return valueForStraightType(type);
}

QString TypeComponentReconstructingVisitor::valueFor(Data::Types::IA5String &type) const
{
    return valueForStraightType(type);
}

QString TypeComponentReconstructingVisitor::valueFor(Data::Types::NumericString &type) const
{
    return valueForStraightType(type);
}

QString TypeComponentReconstructingVisitor::valueFor(Data::Types::Enumerated &type) const
{
    QString out;
    addLine(out, type.name());
    addLine(out, QStringLiteral("{"), m_indent);

    const auto &items = type.items();
    for (auto it = items.begin(); it != items.end(); it++) {
        addIndent(out, m_indent + INDENT_SIZE);
        addWord(out, (*it).name());
        addWord(out, QStringLiteral("("));
        addWord(out, QString::number((*it).value()));
        addWord(out, QStringLiteral(")"));

        if (std::next(it, 1) != items.end())
            addWord(out, QStringLiteral(","));

        finishLine(out);
    }

    addIndent(out, m_indent);
    addWord(out, QStringLiteral("}"));

    return out;
}

QString TypeComponentReconstructingVisitor::valueFor(Data::Types::Choice &type) const
{
    return valueForComplexType<Data::Types::Choice>(type, m_indent);
}

QString TypeComponentReconstructingVisitor::valueFor(Data::Types::Sequence &type) const
{
    return valueForComplexType<Data::Types::Sequence>(type, m_indent);
}

QString TypeComponentReconstructingVisitor::valueFor(Data::Types::SequenceOf &type) const
{
    TypeComponentReconstructingVisitor visitor(m_indent);
    type.itemsType().accept(visitor);

    return QStringLiteral("SEQUENCE ") + type.constraints().rangesTree().expression()
           + QStringLiteral(" OF ") + visitor.value();
}

QString TypeComponentReconstructingVisitor::valueFor(Data::Types::Real &type) const
{
    return valueForStraightType(type);
}

QString TypeComponentReconstructingVisitor::valueFor(Data::Types::LabelType &type) const
{
    Q_UNUSED(type);
    return {};
}

QString TypeComponentReconstructingVisitor::valueFor(Data::Types::Integer &type) const
{
    return valueForStraightType(type);
}

QString TypeComponentReconstructingVisitor::valueFor(Data::Types::UserdefinedType &type) const
{
    return valueForStraightType(type);
}
