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

#include <data/constraints.h>

namespace MalTester {
namespace Data {
namespace Types {

class TypeVisitor;

enum class AlignToNext { byte, word, dword, unspecified };

class Type
{
protected:
    Type(std::unique_ptr<Constraints> constraints = nullptr)
        : m_constraints(std::move(constraints))
        , m_alignment(AlignToNext::unspecified)
    {}

    Type(const Type &other);

public:
    virtual ~Type();

    virtual QString name() const = 0;
    virtual void accept(TypeVisitor &visitor) = 0;
    virtual std::unique_ptr<Type> clone() const = 0;

    const Constraints *constraint() const { return m_constraints.get(); }
    Constraints *constraint() { return m_constraints.get(); }

    void setAlignToNext(const AlignToNext alignToNext) { m_alignment = alignToNext; }
    AlignToNext alignToNext() const { return m_alignment; }

    static AlignToNext mapAlignToNext(const QStringRef &in);

private:
    std::unique_ptr<Constraints> m_constraints;
    AlignToNext m_alignment;
};

} // namespace Types
} // namespace Data
} // namespace MalTester
