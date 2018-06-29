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
#include "acnnodereconstructingvisitor.h"

#include <QString>

#include <data/definitions.h>
#include <data/file.h>
#include <data/project.h>
#include <data/root.h>

#include "typecomponentreconstructingvisitor.h"

using namespace MalTester;

AcnNodeReconstructingVisitor::AcnNodeReconstructingVisitor(QTextStream &outStream)
    : m_outStream(outStream)
{}

void AcnNodeReconstructingVisitor::visit(const Data::Definitions &defs)
{
    m_outStream << defs.name() << QStringLiteral(" DEFINITIONS ::= BEGIN") << QStringLiteral("\n");
    m_outStream << QStringLiteral("END\n\n");
}

void AcnNodeReconstructingVisitor::visit(const Data::File &file)
{
    reconstructCollection<Data::File::DefinitionsList>(file.definitionsList());
}

void AcnNodeReconstructingVisitor::visit(const Data::TypeAssignment &type)
{
    Q_UNUSED(type);
}

void AcnNodeReconstructingVisitor::visit(const Data::ValueAssignment &assignment)
{
    Q_UNUSED(assignment);
}

void AcnNodeReconstructingVisitor::visit(const Data::Project &project)
{
    Q_UNUSED(project);
}

void AcnNodeReconstructingVisitor::visit(const Data::Root &root)
{
    Q_UNUSED(root);
}

void AcnNodeReconstructingVisitor::reconstructImports(const Data::Definitions &defs) const
{
    Q_UNUSED(defs);
}

template<typename T>
void AcnNodeReconstructingVisitor::reconstructImportedCollection(const T &types) const
{
    Q_UNUSED(types);
}

template<typename T>
void AcnNodeReconstructingVisitor::reconstructCollection(const T &collection) const
{
    for (const auto &item : collection) {
        AcnNodeReconstructingVisitor visitor(m_outStream);
        item->accept(visitor);
    }
}
