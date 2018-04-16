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

#include <data/acnparameters.h>
#include <data/types/type.h>

namespace MalTester {
namespace Data {
namespace Types {

class BuiltinType : public Type
{
protected:
    BuiltinType(std::unique_ptr<Constraints> constraints = nullptr,
                std::unique_ptr<AcnParameters> acnParams = nullptr)
        : Type(std::move(constraints), std::move(acnParams))
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

} // namespace Types
} // namespace Data
} // namespace MalTester
