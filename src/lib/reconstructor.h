/****************************************************************************
**
** Copyright (C) 2018 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN MalTester - Tool for generating test cases
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
#pragma once

#include <map>
#include <memory>

#include <QString>

#include <data/project.h>

namespace MalTester {

class Reconstructor
{
public:
    using AcnComponent = QString;
    using Asn1Component = QString;
    using Asn1AcnPair = std::pair<Asn1Component, AcnComponent>;

    Reconstructor(std::unique_ptr<Data::Project> &project);

    void reconstruct();
    const std::map<QString, Asn1AcnPair> &reconstructedFiles() const
    {
        return m_reconstructedFiles;
    }

private:
    void reconstructAsn1File(const Data::File &file);
    void reconstructAcnFile(const Data::File &file);

    std::map<QString, Asn1AcnPair> m_reconstructedFiles;
    std::unique_ptr<Data::Project> m_project;
};

} // namespace MalTester
