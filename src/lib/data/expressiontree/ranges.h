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

#include <functional>

#include <QPair>
#include <QString>

#include <data/valueprinters.h>

#include <data/expressiontree/expressionnode.h>

namespace MalTester {
namespace Data {
namespace ExpressionTree {

class Range : public ExpressionTree::ExpressionNode
{
public:
    virtual ~Range() = default;
};

template<typename T>
class TypedRange : public Range
{
public:
    TypedRange(const T &begin, const T &end)
        : m_data(begin, end)
    {}

    ~TypedRange() override = default;

protected:
    QPair<T, T> m_data;
};

class IntegerRange : public TypedRange<int>
{
public:
    IntegerRange(const int &begin, const int &end);

    std::unique_ptr<ExpressionNode> clone() const override;
    QString asString() const override;
};

class RealRange : public TypedRange<double>
{
public:
    RealRange(const double &begin, const double &end);

    std::unique_ptr<ExpressionNode> clone() const override;
    QString asString() const override;
};

class EnumeratedRange : public TypedRange<int>
{
public:
    EnumeratedRange(const int &begin, const int &end);

    std::unique_ptr<ExpressionNode> clone() const override;
    QString asString() const override;
};

class StringRange : public TypedRange<QString>
{
public:
    StringRange(const QString &begin,
                const QString &end,
                std::function<QString(const QString &)> printer = printAsASCIIString);

    std::unique_ptr<ExpressionNode> clone() const override;
    QString asString() const override;

private:
    std::function<QString(const QString &)> m_printer;
};

} // namespace ExpressionTree
} // namespace Data
} // namespace MalTester
