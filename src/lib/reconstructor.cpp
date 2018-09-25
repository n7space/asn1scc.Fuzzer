/****************************************************************************
**
** Copyright (C) 2018 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Fuzzer - Tool for generating test cases
** based on ASN.1/ACN models and simulating malformed or malicious data.
**
** Tool was developed under a m_processogramme and funded by
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
** along with this m_processogram.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "reconstructor.h"

#include <QTextStream>

#include <acnnodereconstructingvisitor.h>
#include <asn1nodereconstructingvisitor.h>

using namespace Fuzzer;

Reconstructor::Reconstructor(const StreamFactory &streamFactory)
    : m_streamFactory(streamFactory)
{}

bool Reconstructor::reconstruct(const Data::Project &project)
{
    bool result = true;
    for (const auto &file : project.files()) {
        result &= reconstructAsn1File(*file);
        result &= reconstructAcnFile(*file);
    }
    return result;
}

bool Reconstructor::reconstructAsn1File(const Data::File &file)
{
    auto outStream = m_streamFactory(file.name());
    if (outStream == nullptr)
        return false;

    Asn1NodeReconstructingVisitor visitor(*outStream);
    visitor.visit(file);
    return true;
}

namespace {
QString makeAcnFileName(const QString &name)
{
    if (name.endsWith(".asn1"))
        return name.left(name.size() - 3) + "cn";
    if (name.endsWith(".asn"))
        return name.left(name.size() - 2) + "cn";
    return name + ".acn";
}
} // namespace

bool Reconstructor::reconstructAcnFile(const Data::File &file)
{
    auto outStream = m_streamFactory(makeAcnFileName(file.name()));
    if (outStream == nullptr)
        return false;

    AcnNodeReconstructingVisitor visitor(*outStream);
    visitor.visit(file);
    return true;
}
