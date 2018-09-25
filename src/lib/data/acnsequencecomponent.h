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
#include <vector>

#include <QString>

#include <data/sequencecomponent.h>

#include <data/types/type.h>

namespace Fuzzer {
namespace Data {

class AcnSequenceComponent : public SequenceComponent
{
public:
    AcnSequenceComponent() = default;
    ~AcnSequenceComponent() override = default;

    AcnSequenceComponent(const QString &id, const QString &name, std::unique_ptr<Types::Type> type);

    AcnSequenceComponent(const AcnSequenceComponent &other);

    std::unique_ptr<SequenceComponent> clone() const override;
    QString definitionAsString() const override;
    QString presentWhen() const override;

    const QString &id() const;

private:
    QString m_id;
};

using AcnSequenceComponentPtr = std::unique_ptr<AcnSequenceComponent>;

} // namespace Data
} // namespace Fuzzer
