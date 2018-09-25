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

#include <memory>

#include <QString>

#include <QXmlStreamReader>

#include <data/constraints/constraintlist.h>
#include <data/constraints/fromconstraint.h>
#include <data/constraints/logicoperators.h>
#include <data/constraints/rangeconstraint.h>
#include <data/constraints/sizeconstraint.h>

#include <data/values.h>

namespace Fuzzer {

template<typename T>
class AstXmlConstraintParser
{
public:
    using ConstraintList = Data::Constraints::ConstraintList<T>;

    explicit AstXmlConstraintParser(QXmlStreamReader &xmlReader, ConstraintList &targetList)
        : m_xmlReader(xmlReader)
        , m_list(targetList)
    {}

    bool parse();

private:
    QXmlStreamReader &m_xmlReader;
    ConstraintList &m_list;
};

template<typename T>
class AstXmlConstraintNodeParser
{
public:
    using Constraints = std::unique_ptr<Data::Constraints::Constraint<T>>;

    explicit AstXmlConstraintNodeParser(QXmlStreamReader &xmlReader)
        : m_xmlReader(xmlReader)
    {}

    bool parseSingleSubNode();
    bool parseNextSubNode();

    Constraints takeConstraints() { return std::move(m_constraints); }

private:
    Constraints readNode();

    Constraints readValueNode();
    Constraints readRange();
    Constraints readOr();
    Constraints readAnd();
    Constraints readAlpha();
    Constraints readSize();

    Constraints readNextNode();

    QString findValue();

    Constraints buildRange(const QString &val) const;
    Constraints buildRange(const QString &min, const QString &max) const;

    QXmlStreamReader &m_xmlReader;
    Constraints m_constraints;
};

template<typename T>
bool AstXmlConstraintParser<T>::parse()
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QLatin1Literal("Constraints"))
            return parse();
        AstXmlConstraintNodeParser<T> nodeParser(m_xmlReader);
        if (!nodeParser.parseNextSubNode())
            return false;
        m_list.append(nodeParser.takeConstraints());
    }

    return true;
}

template<typename T>
bool AstXmlConstraintNodeParser<T>::parseSingleSubNode()
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_constraints != nullptr) {
            m_xmlReader.raiseError("Unexpected element");
            return false;
        }
        m_constraints = readNode();
    }
    return m_constraints != nullptr;
}

template<typename T>
bool AstXmlConstraintNodeParser<T>::parseNextSubNode()
{
    m_constraints = readNode();
    return m_constraints != nullptr;
}

template<typename T>
typename AstXmlConstraintNodeParser<T>::Constraints AstXmlConstraintNodeParser<T>::readNode()
{
    if (m_xmlReader.name() == T::astNodeName())
        return readValueNode();
    else if (m_xmlReader.name() == QLatin1Literal("Range"))
        return readRange();
    else if (m_xmlReader.name() == QLatin1Literal("OR"))
        return readOr();
    else if (m_xmlReader.name() == QLatin1Literal("AND"))
        return readAnd();
    else if (m_xmlReader.name() == QLatin1Literal("ALPHA"))
        return readAlpha();
    else if (m_xmlReader.name() == QLatin1Literal("SIZE"))
        return readSize();

    m_xmlReader.raiseError("Unknown node: " + m_xmlReader.name());
    return nullptr;
}

template<typename T>
typename AstXmlConstraintNodeParser<T>::Constraints AstXmlConstraintNodeParser<T>::readValueNode()
{
    return buildRange(m_xmlReader.readElementText());
}

template<typename T>
typename AstXmlConstraintNodeParser<T>::Constraints AstXmlConstraintNodeParser<T>::readRange()
{
    QString min;
    QString max;
    while (m_xmlReader.readNextStartElement())
        if (m_xmlReader.name() == QLatin1Literal("Min"))
            min = findValue();
        else if (m_xmlReader.name() == QLatin1Literal("Max"))
            max = findValue();

    if (min.isEmpty() || max.isEmpty()) {
        m_xmlReader.raiseError("Malformed range in constraints");
        return nullptr;
    }

    return buildRange(min, max);
}

template<typename T>
typename AstXmlConstraintNodeParser<T>::Constraints AstXmlConstraintNodeParser<T>::readOr()
{
    auto left = readNextNode();
    auto right = readNextNode();
    m_xmlReader.skipCurrentElement();
    return std::make_unique<Data::Constraints::OrConstraint<T>>(std::move(left), std::move(right));
}

template<typename T>
typename AstXmlConstraintNodeParser<T>::Constraints AstXmlConstraintNodeParser<T>::readAnd()
{
    auto left = readNextNode();
    auto right = readNextNode();
    m_xmlReader.skipCurrentElement();
    return std::make_unique<Data::Constraints::AndConstraint<T>>(std::move(left), std::move(right));
}

template<typename T>
typename AstXmlConstraintNodeParser<T>::Constraints AstXmlConstraintNodeParser<T>::readAlpha()
{
    AstXmlConstraintNodeParser<Data::StringValue> innerParser(m_xmlReader);
    if (!innerParser.parseSingleSubNode())
        return nullptr;
    return std::make_unique<Data::Constraints::FromConstraint<T>>(innerParser.takeConstraints());
}

template<typename T>
typename AstXmlConstraintNodeParser<T>::Constraints AstXmlConstraintNodeParser<T>::readSize()
{
    AstXmlConstraintNodeParser<Data::IntegerValue> innerParser(m_xmlReader);
    if (!innerParser.parseSingleSubNode())
        return nullptr;
    return std::make_unique<Data::Constraints::SizeConstraint<T>>(innerParser.takeConstraints());
}

template<typename T>
typename AstXmlConstraintNodeParser<T>::Constraints AstXmlConstraintNodeParser<T>::readNextNode()
{
    if (!m_xmlReader.readNextStartElement()) {
        m_xmlReader.raiseError("Missing required sub-node");
        return nullptr;
    }
    return readNode();
}

template<typename T>
QString AstXmlConstraintNodeParser<T>::findValue()
{
    QString val;

    while (m_xmlReader.readNextStartElement())
        if (m_xmlReader.name() == T::astNodeName())
            val = m_xmlReader.readElementText();
        else
            m_xmlReader.skipCurrentElement();

    return val;
}

template<typename T>
typename AstXmlConstraintNodeParser<T>::Constraints AstXmlConstraintNodeParser<T>::buildRange(
    const QString &val) const
{
    return Data::Constraints::RangeConstraint<T>::create({T::fromAstValue(val)});
}

template<typename T>
typename AstXmlConstraintNodeParser<T>::Constraints AstXmlConstraintNodeParser<T>::buildRange(
    const QString &min, const QString &max) const
{
    return Data::Constraints::RangeConstraint<T>::create(
        {T::fromAstValue(min), T::fromAstValue(max)});
}

} // namespace Fuzzer
