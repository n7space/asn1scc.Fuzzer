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

#include <data/types/type.h>

namespace MalTester {
namespace Data {
namespace Types {

class BuiltinType : public Type
{
protected:
    BuiltinType(std::unique_ptr<Constraints> constraints = nullptr)
        : Type(std::move(constraints))
    {}

public:
    static std::unique_ptr<Type> createBuiltinType(const QString &name);
};

class Boolean : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("BOOLEAN"); }

    void accept(TypeVisitor &visitor) override;
};

class Null : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("NULL"); }

    void accept(TypeVisitor &visitor) override;
};

class BitString : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("BIT STRING"); }

    void accept(TypeVisitor &visitor) override;
};

class OctetString : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("OCTET STRING"); }

    void accept(TypeVisitor &visitor) override;
};

class IA5String : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("IA5String"); }

    void accept(TypeVisitor &visitor) override;
};

class NumericString : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("NumericString"); }

    void accept(TypeVisitor &visitor) override;
};

class Enumarated : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("ENUMERATED"); }

    void accept(TypeVisitor &visitor) override;
};

class Choice : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("CHOICE"); }

    void accept(TypeVisitor &visitor) override;
};

class Sequence : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("SEQUENCE"); }

    void accept(TypeVisitor &visitor) override;
};

class SequenceOf : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("SEQUENCE OF"); }

    void accept(TypeVisitor &visitor) override;
};

enum class Endianness { big, little, unspecified };

enum class IntegerEncoding {
    pos_int,
    twos_complement,
    ASCII,
    BCD,
    unspecified,
};

class Integer : public BuiltinType
{
public:
    Integer()
        : BuiltinType(std::make_unique<Constraints>(&Integer::toVariantPair))
        , m_encoding(IntegerEncoding::unspecified)
        , m_endianness(Endianness::unspecified)
        , m_size(0) // TODO?
    {}

    QString name() const override { return QLatin1String("INTEGER"); }

    void accept(TypeVisitor &visitor) override;

    void setSize(const int size) { m_size = size; }
    int size() const { return m_size; }

    void setEncoding(const IntegerEncoding encoding) { m_encoding = encoding; }
    IntegerEncoding encoding() const { return m_encoding; }

    void setEndianness(const Endianness endianness) { m_endianness = endianness; }
    Endianness endianness() const { return m_endianness; }

    static IntegerEncoding mapEncoding(const QStringRef &in);
    static Endianness mapEndianess(const QStringRef &in);

private:
    static Constraints::VariantPair toVariantPair(const Constraints::StringPair &range)
    {
        return {range.first.toInt(), range.second.toInt()};
    }

    IntegerEncoding m_encoding;
    Endianness m_endianness;
    int m_size;
};

enum class RealEncoding { IEEE754_1985_32, IEEE754_1985_64, unspecified };

class Real : public BuiltinType
{
public:
    Real()
        : BuiltinType(std::make_unique<Constraints>(&Real::toVariantPair))
    {}

    QString name() const override { return QLatin1String("REAL"); }

    void accept(TypeVisitor &visitor) override;

    static RealEncoding mapEncoding(const QStringRef &ref);

private:
    static Constraints::VariantPair toVariantPair(const Constraints::StringPair &range)
    {
        return {range.first.toDouble(), range.second.toDouble()};
    }
};

} // namespace Types
} // namespace Data
} // namespace MalTester
