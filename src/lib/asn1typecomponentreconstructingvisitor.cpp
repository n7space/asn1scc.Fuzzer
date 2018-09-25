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

#include "asn1typecomponentreconstructingvisitor.h"

#include <data/constraints/printingvisitor.h>

#include "typeconstraintsreconstructingvisitor.h"

using namespace Fuzzer;

namespace {
const int INDENT_SIZE = 4;
} // namespace

Asn1TypeComponentReconstructingVisitor::Asn1TypeComponentReconstructingVisitor(
    QTextStream &outStream, int indent)
    : m_outStream(outStream)
    , m_indent(indent)
{}

void Asn1TypeComponentReconstructingVisitor::visit(const Data::Types::Boolean &type)
{
    valueForStraightType(type);
}
void Asn1TypeComponentReconstructingVisitor::visit(const Data::Types::Null &type)
{
    valueForStraightType(type);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Data::Types::BitString &type)
{
    valueForStraightType(type);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Data::Types::OctetString &type)
{
    valueForStraightType(type);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Data::Types::IA5String &type)
{
    valueForStraightType(type);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Data::Types::NumericString &type)
{
    valueForStraightType(type);
}

namespace {
QList<Data::Types::EnumeratedItem> itemsSortedByIndex(const Data::Types::Enumerated &type)
{
    auto items = type.items().values();
    std::sort(items.begin(),
              items.end(),
              [](const Data::Types::EnumeratedItem &a, const Data::Types::EnumeratedItem &b) {
                  return a.index() < b.index();
              });
    return items;
}
} // namespace

void Asn1TypeComponentReconstructingVisitor::visit(const Data::Types::Enumerated &type)
{
    m_outStream << type.name() << QStringLiteral("\n");
    m_outStream << QString(m_indent, QChar(' ')) << QStringLiteral("{") << QStringLiteral("\n");

    const auto items = itemsSortedByIndex(type);
    for (auto it = items.begin(); it != items.end(); it++) {
        m_outStream << QString(m_indent + INDENT_SIZE, QChar(' '));
        m_outStream << (*it).name();
        m_outStream << QStringLiteral("(");
        m_outStream << QString::number((*it).value());
        m_outStream << QStringLiteral(")");

        if (std::next(it, 1) != items.end())
            m_outStream << QStringLiteral(",");

        m_outStream << QStringLiteral("\n");
    }

    m_outStream << QString(m_indent, QChar(' ')) << QStringLiteral("}");
}

void Asn1TypeComponentReconstructingVisitor::visit(const Data::Types::Choice &type)
{
    valueForComplexType<Data::Types::Choice>(type, m_indent);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Data::Types::Sequence &type)
{
    valueForComplexType<Data::Types::Sequence>(type, m_indent);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Data::Types::SequenceOf &type)
{
    m_outStream << QStringLiteral("SEQUENCE ") << toString(type.constraints())
                << QStringLiteral(" OF ");

    Asn1TypeComponentReconstructingVisitor visitor(m_outStream, m_indent);
    type.itemsType().accept(visitor);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Data::Types::Real &type)
{
    valueForStraightType(type);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Data::Types::LabelType &type)
{
    Q_UNUSED(type);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Data::Types::Integer &type)
{
    valueForStraightType(type);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Data::Types::UserdefinedType &type)
{
    valueForStraightType(type);
}

void Asn1TypeComponentReconstructingVisitor::valueForStraightType(const Data::Types::Type &type)
{
    TypeConstraintsReconstructingVisitor visitor;
    type.accept(visitor);

    m_outStream << type.name() << visitor.value();
}

template<typename T>
void Asn1TypeComponentReconstructingVisitor::valueForComplexType(const T &type, const int indent)
{
    addLine(type.name());
    addLine(QStringLiteral("{"), indent);

    const auto &components = type.components();
    for (auto it = components.begin(); it != components.end(); it++) {
        addIndent(indent + INDENT_SIZE);
        addWord((*it)->name());

        Asn1TypeComponentReconstructingVisitor visitor(m_outStream, indent + INDENT_SIZE);
        (*it)->type().accept(visitor);

        if (std::next(it) != components.end())
            addWord(QStringLiteral(","));

        finishLine();
    }

    addIndent(indent);
    addWord(QStringLiteral("}"));
}

void Asn1TypeComponentReconstructingVisitor::addIndent(int indent)
{
    m_outStream << QString(indent, QChar(' '));
}

void Asn1TypeComponentReconstructingVisitor::addWord(const QString &word)
{
    m_outStream << word << QStringLiteral(" ");
}

void Asn1TypeComponentReconstructingVisitor::finishLine()
{
    m_outStream << QStringLiteral("\n");
}

void Asn1TypeComponentReconstructingVisitor::addLine(QString line, int indent)
{
    addIndent(indent);
    addWord(line);
    finishLine();
}
