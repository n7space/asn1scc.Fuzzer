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

#include "reconstructor.h"

#include <QTextStream>

#include <acnnodereconstructingvisitor.h>
#include <nodereconstructingvisitor.h>

#include <iostream>
#include <QDebug>

using namespace MalTester;

Reconstructor::Reconstructor(std::unique_ptr<Data::Project> &project)
    : m_project(std::move(project))
{}

void Reconstructor::reconstruct()
{
    for (const auto &file : m_project->files()) {
        reconstructAsn1File(*file);
        reconstructAcnFile(*file);
    }
}

void Reconstructor::reconstructAsn1File(const Data::File &file)
{
    QTextStream outStream(&m_reconstructedFiles[file.name()].first);
    NodeReconstructingVisitor visitor(outStream);
    visitor.visit(file);
}

void Reconstructor::reconstructAcnFile(const Data::File &file)
{
    QTextStream outStream(&m_reconstructedFiles[file.name()].second);
    AcnNodeReconstructingVisitor visitor(outStream);
    visitor.visit(file);
}
