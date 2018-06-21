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

#include <nodereconstructingvisitor.h>

#include <iostream>

using namespace MalTester;

Reconstructor::Reconstructor(std::unique_ptr<Data::Project> &project)
    : m_project(std::move(project))
{}

void printToStdOut(const QString &name, const QString &content)
{
    std::cout << std::endl;
    std::cout << "=========" + name.toStdString() + "=========" << std::endl;
    std::cout << content.toStdString() << std::endl;
    std::cout << "=========" + name.toStdString() + "=========" << std::endl;
    std::cout << std::endl;
}

void Reconstructor::reconstruct()
{
    for (const auto &file : m_project->files()) {
        NodeReconstructingVisitor visitor;
        visitor.visit(*file);

        // TODO: remove when not needed any more
        if (0)
            printToStdOut(file->name(), visitor.value());

        m_reconstructedFiles[file->name()] = visitor.value();
    }
}
