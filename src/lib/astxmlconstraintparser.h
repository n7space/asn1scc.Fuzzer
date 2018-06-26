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

#include <memory>

#include <QString>

#include <QXmlStreamReader>

#include <data/constraints/constraintlist.h>
#include <data/constraints/fromconstraint.h>
#include <data/constraints/logicoperators.h>
#include <data/constraints/rangeconstraint.h>

#include <data/values.h>

namespace MalTester {

template<typename T>
class AstXmlConstraintParser
{
public:
    using Constraints = std::unique_ptr<Data::Constraints::Constraint<T>>;

    explicit AstXmlConstraintParser(QXmlStreamReader &xmlReader)
        : m_xmlReader(xmlReader)
    {}

    bool parse();
    bool parseSingleNode();

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
    auto list = std::make_unique<Data::Constraints::ConstraintList<T>>();

    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QLatin1Literal("Constraints"))
            return parse();
        auto node = readNode();
        if (node == nullptr)
            return false;
        list->append(std::move(node));
    }

    m_constraints = std::move(list);
    return true;
}

template<typename T>
bool AstXmlConstraintParser<T>::parseSingleNode()
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
typename AstXmlConstraintParser<T>::Constraints AstXmlConstraintParser<T>::readNode()
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
typename AstXmlConstraintParser<T>::Constraints AstXmlConstraintParser<T>::readValueNode()
{
    return buildRange(m_xmlReader.readElementText());
}

template<typename T>
typename AstXmlConstraintParser<T>::Constraints AstXmlConstraintParser<T>::readRange()
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
typename AstXmlConstraintParser<T>::Constraints AstXmlConstraintParser<T>::readOr()
{
    auto left = readNextNode();
    auto right = readNextNode();
    m_xmlReader.skipCurrentElement();
    return std::make_unique<Data::Constraints::OrConstraint<T>>(std::move(left), std::move(right));
}

template<typename T>
typename AstXmlConstraintParser<T>::Constraints AstXmlConstraintParser<T>::readAnd()
{
    auto left = readNextNode();
    auto right = readNextNode();
    m_xmlReader.skipCurrentElement();
    return std::make_unique<Data::Constraints::AndConstraint<T>>(std::move(left), std::move(right));
}

template<typename T>
typename AstXmlConstraintParser<T>::Constraints AstXmlConstraintParser<T>::readAlpha()
{
    AstXmlConstraintParser<Data::StringValue> innerParser(m_xmlReader);
    if (!innerParser.parseSingleNode())
        return nullptr;
    return std::make_unique<Data::Constraints::FromConstraint<T>>(innerParser.takeConstraints());
}

template<typename T>
typename AstXmlConstraintParser<T>::Constraints AstXmlConstraintParser<T>::readSize()
{
    AstXmlConstraintParser<Data::IntegerValue> innerParser(m_xmlReader);
    if (!innerParser.parseSingleNode())
        return nullptr;
    return std::make_unique<Data::Constraints::SizeConstraint<T>>(innerParser.takeConstraints());
}

template<typename T>
typename AstXmlConstraintParser<T>::Constraints AstXmlConstraintParser<T>::readNextNode()
{
    if (!m_xmlReader.readNextStartElement()) {
        m_xmlReader.raiseError("Missing required sub-node");
        return nullptr;
    }
    return readNode();
}

template<typename T>
QString AstXmlConstraintParser<T>::findValue()
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
typename AstXmlConstraintParser<T>::Constraints AstXmlConstraintParser<T>::buildRange(
    const QString &val) const
{
    const auto range = Data::Constraints::Range<typename T::Type>(T::fromAstValue(val));
    return std::make_unique<Data::Constraints::RangeConstraint<T>>(range);
}

template<typename T>
typename AstXmlConstraintParser<T>::Constraints AstXmlConstraintParser<T>::buildRange(
    const QString &min, const QString &max) const
{
    const auto range = Data::Constraints::Range<typename T::Type>(T::fromAstValue(min),
                                                                  T::fromAstValue(max));
    return std::make_unique<Data::Constraints::RangeConstraint<T>>(range);
}

} // namespace MalTester
