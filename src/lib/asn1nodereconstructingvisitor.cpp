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
#include "asn1nodereconstructingvisitor.h"

#include <QString>

#include <data/definitions.h>
#include <data/file.h>
#include <data/project.h>
#include <data/root.h>

#include "asn1typecomponentreconstructingvisitor.h"

using namespace Fuzzer;

Asn1NodeReconstructingVisitor::Asn1NodeReconstructingVisitor(QTextStream &outStream)
    : m_outStream(outStream)
{}

void Asn1NodeReconstructingVisitor::visit(const Data::Definitions &defs)
{
    m_outStream << defs.name() << QStringLiteral(" DEFINITIONS ::= BEGIN") << QStringLiteral("\n");

    reconstructImports(defs);
    reconstructCollection<Data::Definitions::Values>(defs.values());
    reconstructCollection<Data::Definitions::Types>(defs.types());

    m_outStream << QStringLiteral("END\n\n");
}

void Asn1NodeReconstructingVisitor::visit(const Data::File &file)
{
    reconstructCollection<Data::File::DefinitionsList>(file.definitionsList());
}

void Asn1NodeReconstructingVisitor::visit(const Data::TypeAssignment &type)
{
    m_outStream << type.name() << QStringLiteral(" ::= ");

    Asn1TypeComponentReconstructingVisitor visitor(m_outStream);
    type.type()->accept(visitor);

    m_outStream << QStringLiteral("\n");
}

void Asn1NodeReconstructingVisitor::visit(const Data::ValueAssignment &assignment)
{
    if (assignment.value() == nullptr)
        return;

    m_outStream << assignment.name() << QStringLiteral(" ") << assignment.type()->name()
                << QStringLiteral(" ::= ") << assignment.value()->asString()
                << QStringLiteral("\n");
}

void Asn1NodeReconstructingVisitor::visit(const Data::Project &project)
{
    Q_UNUSED(project);
}

void Asn1NodeReconstructingVisitor::visit(const Data::Root &root)
{
    Q_UNUSED(root);
}

void Asn1NodeReconstructingVisitor::reconstructImports(const Data::Definitions &defs) const
{
    const auto &values = defs.importedValues();
    const auto &types = defs.importedTypes();

    if (values.empty() && types.empty())
        return;

    m_outStream << QStringLiteral("IMPORTS\n");

    reconstructImportedCollection<Data::Definitions::ImportedValues>(values);
    reconstructImportedCollection<Data::Definitions::ImportedTypes>(types);

    m_outStream << QStringLiteral(";\n");
}

template<typename T>
void Asn1NodeReconstructingVisitor::reconstructImportedCollection(const T &types) const
{
    for (const auto &type : types)
        m_outStream << type.name() << QStringLiteral(" FROM ") << type.module()
                    << QStringLiteral("\n");
}

template<typename T>
void Asn1NodeReconstructingVisitor::reconstructCollection(const T &collection) const
{
    for (const auto &item : collection) {
        Asn1NodeReconstructingVisitor visitor(m_outStream);
        item->accept(visitor);
    }
}
