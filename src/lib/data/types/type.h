/****************************************************************************
**
** Copyright (C) 2018-2019 N7 Space sp. z o. o.
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

#include "typereadingvisitor.h"

namespace Fuzzer {
namespace Data {
namespace Types {

class TypeMutatingVisitor;

enum class AlignToNext { byte, word, dword, unspecified };
enum class Endianness { big, little, unspecified };

class Type
{
protected:
    Type()
        : m_alignment(AlignToNext::unspecified)
    {}

    Type(const Type &other) = default;

public:
    virtual ~Type();

    virtual QString name() const = 0;

    virtual void accept(TypeMutatingVisitor &visitor) = 0;
    virtual void accept(TypeReadingVisitor &visitor) const = 0;

    virtual std::unique_ptr<Type> clone() const = 0;

    void setAlignToNext(const AlignToNext alignToNext) { m_alignment = alignToNext; }
    AlignToNext alignToNext() const { return m_alignment; }

    static AlignToNext mapAlignToNext(QStringRef in);
    static AlignToNext mapAlignToNext(const QString &in) { return mapAlignToNext(&in); }

    static Endianness mapEndianess(QStringRef in);
    static Endianness mapEndianess(const QString &in) { return mapEndianess(&in); }

    static QString alignToNextToString(AlignToNext param);
    static QString endiannessToString(Endianness param);

private:
    AlignToNext m_alignment;
};

} // namespace Types
} // namespace Data
} // namespace Fuzzer
