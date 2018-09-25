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

#include "acntypecomponentreconstructingvisitor.h"

#include "typeconstraintsreconstructingvisitor.h"

using namespace Fuzzer;

namespace {
const int INDENT_SIZE = 4;
} // namespace

AcnTypeComponentReconstructingVisitor::AcnTypeComponentReconstructingVisitor(
    QTextStream &outStream, int indent, QString presentWhenValue)
    : m_outStream(outStream)
    , m_indent(indent)
    , m_presentWhenValue(presentWhenValue)
{}

void AcnTypeComponentReconstructingVisitor::visit(const Data::Types::Boolean &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);

    tryAppendTrueValue(type, params);
    tryAppendFalseValue(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Data::Types::Null &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendPattern(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Data::Types::BitString &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendSize(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Data::Types::OctetString &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendSize(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Data::Types::IA5String &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendSize(type, params);
    tryAppendAsciiStringParams(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Data::Types::NumericString &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendSize(type, params);
    tryAppendAsciiStringParams(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Data::Types::Enumerated &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendIntegerAcnParams(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Data::Types::Choice &type)
{
    // TODO: append determinant, when it will be fixed
    reconstructComplexType(type, m_indent);
}

void AcnTypeComponentReconstructingVisitor::visit(const Data::Types::Sequence &type)
{
    reconstructComplexType(type, m_indent);
}

void AcnTypeComponentReconstructingVisitor::visit(const Data::Types::SequenceOf &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendSize(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Data::Types::Real &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendEncoding(type, params);
    tryAppendEndianness(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Data::Types::LabelType &type)
{
    Q_UNUSED(type);
}

void AcnTypeComponentReconstructingVisitor::visit(const Data::Types::Integer &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendIntegerAcnParams(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Data::Types::UserdefinedType &type)
{
    reconstructComplexTypeArguments(type);

    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::tryAppendIntegerAcnParams(
    const Data::Types::IntegerAcnParameters &type, QStringList &params) const
{
    tryAppendSize(type, params);
    tryAppendEncoding(type, params);
    tryAppendEndianness(type, params);
}

void AcnTypeComponentReconstructingVisitor::tryAppendAsciiStringParams(
    const Data::Types::AsciiStringAcnParameters &type, QStringList &params) const
{
    tryAppendEncoding(type, params);
    tryAppendTerminationPattern(type, params);
}

void AcnTypeComponentReconstructingVisitor::tryAppendAlignToNext(const Data::Types::Type &type,
                                                                 QStringList &params) const
{
    auto alignToNext = type.alignToNext();
    if (alignToNext != Data::Types::AlignToNext::unspecified)
        params << QStringLiteral("align-to-next ")
                      + Data::Types::Type::alignToNextToString(alignToNext);
}

void AcnTypeComponentReconstructingVisitor::tryAppendTrueValue(const Data::Types::Boolean &type,
                                                               QStringList &params) const
{
    auto trueValue = type.trueValue();
    if (!trueValue.isEmpty())
        params << QStringLiteral("true-value ") + Data::BitStringValue::asString(trueValue);
}

void AcnTypeComponentReconstructingVisitor::tryAppendFalseValue(const Data::Types::Boolean &type,
                                                                QStringList &params) const
{
    auto falseValue = type.falseValue();
    if (!falseValue.isEmpty())
        params << QStringLiteral("false-value ") + Data::BitStringValue::asString(falseValue);
}

void AcnTypeComponentReconstructingVisitor::tryAppendPattern(const Data::Types::Null &type,
                                                             QStringList &params) const
{
    auto pattern = type.pattern();
    if (!pattern.isEmpty())
        params << QStringLiteral("pattern ") + Data::BitStringValue::asString(pattern);
}

void AcnTypeComponentReconstructingVisitor::tryAppendTerminationPattern(
    const Data::Types::AsciiStringAcnParameters &type, QStringList &params) const
{
    auto terminationPattern = type.terminationPattern();
    if (!terminationPattern.isEmpty())
        params << QStringLiteral("termination-pattern ")
                      + Data::OctetStringValue::asString(terminationPattern);
}

void AcnTypeComponentReconstructingVisitor::tryAppendPresentWhen(QStringList &params) const
{
    auto val = m_presentWhenValue;
    if (!val.isEmpty())
        params << QStringLiteral("present-when ")
                      + val.replace(QStringLiteral(" = "), QStringLiteral(" == "));
}

template<typename T>
void AcnTypeComponentReconstructingVisitor::tryAppendSize(const T &type, QStringList &params) const
{
    const auto size = type.acnSize();
    if (!size.isEmpty())
        params << QStringLiteral("size ") + size;
}

void AcnTypeComponentReconstructingVisitor::tryAppendSize(
    const Data::Types::IntegerAcnParameters &type, QStringList &params) const
{
    const auto size = type.size();
    if (size != 0)
        params << QStringLiteral("size ") + QString::number(size);
}

template<typename T>
void AcnTypeComponentReconstructingVisitor::tryAppendEncoding(const T &type,
                                                              QStringList &params) const
{
    const auto encodingString = type.encodingToString(type.encoding());
    if (encodingString.isEmpty())
        return;

    params << QStringLiteral("encoding ") + encodingString;
}

template<typename T>
void AcnTypeComponentReconstructingVisitor::tryAppendEndianness(const T &type,
                                                                QStringList &params) const
{
    auto endianness = type.endianness();
    if (endianness != Data::Types::Endianness::unspecified)
        params.append(QStringLiteral("endianness ")
                      + Data::Types::Type::endiannessToString(endianness));
}

template<typename T>
void AcnTypeComponentReconstructingVisitor::reconstructComplexType(const T &type, const int indent)
{
    reconstructComplexTypeParameters(type);

    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);

    endParamsList(params);

    reconstructComplexTypeComponents(type, indent);
}

template<typename T>
void AcnTypeComponentReconstructingVisitor::reconstructComplexTypeComponents(const T &type,
                                                                             const int indent)
{
    if (type.components().empty())
        return;

    m_outStream << QStringLiteral("\n") << addIndent(indent) << QStringLiteral("{")
                << QStringLiteral("\n");

    const auto &components = type.components();
    for (auto it = components.begin(); it != components.end(); it++) {
        m_outStream << addIndent(indent + INDENT_SIZE) << (*it)->definitionAsString()
                    << QStringLiteral(" ");

        AcnTypeComponentReconstructingVisitor visitor(m_outStream,
                                                      indent + INDENT_SIZE,
                                                      (*it)->presentWhen());
        (*it)->type().accept(visitor);

        if (std::next(it, 1) != components.end())
            m_outStream << QStringLiteral(",");
        m_outStream << QStringLiteral("\n");
    }

    m_outStream << addIndent(indent) << QStringLiteral("}");
}

template<typename T>
void AcnTypeComponentReconstructingVisitor::reconstructComplexTypeParameters(const T &type)
{
    const auto &acnParameters = type.acnParameters();
    if (acnParameters.empty())
        return;

    m_outStream << QStringLiteral("<");
    for (auto it = acnParameters.begin(); it != acnParameters.end(); it++) {
        m_outStream << (*it)->type() << QStringLiteral(":") << (*it)->name();

        if (std::next(it, 1) != acnParameters.end())
            m_outStream << QStringLiteral(", ");
    }
    m_outStream << QStringLiteral("> ");
}

void AcnTypeComponentReconstructingVisitor::reconstructComplexTypeArguments(
    const Data::Types::UserdefinedType &type)
{
    const auto &acnArguments = type.acnArguments();
    if (acnArguments.empty())
        return;

    m_outStream << QStringLiteral("<");
    for (auto it = acnArguments.begin(); it != acnArguments.end(); it++) {
        m_outStream << (*it)->id();

        if (std::next(it, 1) != acnArguments.end())
            m_outStream << QStringLiteral(", ");
    }

    m_outStream << QStringLiteral("> ");
}

QStringList AcnTypeComponentReconstructingVisitor::beginParamsList()
{
    m_outStream << QStringLiteral("[");
    return {};
}

void AcnTypeComponentReconstructingVisitor::endParamsList(QStringList &params)
{
    tryAppendPresentWhen(params);

    if (!params.isEmpty())
        m_outStream << params.join(QStringLiteral(", "));

    m_outStream << QStringLiteral("]");
}

QString AcnTypeComponentReconstructingVisitor::addIndent(int size) const
{
    return QString(size, QChar(' '));
}
