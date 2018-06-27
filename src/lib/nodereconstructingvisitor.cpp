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
#include "nodereconstructingvisitor.h"

#include <QString>

#include <data/definitions.h>
#include <data/file.h>
#include <data/project.h>
#include <data/root.h>

#include "typecomponentreconstructingvisitor.h"

using namespace MalTester;

QString NodeReconstructingVisitor::valueFor(const Data::Definitions &defs) const
{
    QString start = defs.name() + QStringLiteral(" DEFINITIONS ::= BEGIN") + QStringLiteral("\n");

    QString imports = importsAsString(defs);
    QString values = valuesAsString(defs);
    QString types = typesAsString(defs);

    QString finish = QStringLiteral("END\n\n");

    return start + imports + types + values + finish;
}

QString NodeReconstructingVisitor::valueFor(const Data::File &file) const
{
    QString ret;
    for (const auto &def : file.definitionsList()) {
        NodeReconstructingVisitor visitor;
        ret += visitor.valueFor(*def);
    }

    return ret;
}

QString NodeReconstructingVisitor::valueFor(const Data::TypeAssignment &type) const
{
    TypeComponentReconstructingVisitor visitor;
    type.type()->accept(visitor);

    return type.name() + QStringLiteral(" ::= ") + visitor.value() + QStringLiteral("\n");
}

QString NodeReconstructingVisitor::valueFor(const Data::ValueAssignment &assignment) const
{
    if (assignment.value() == nullptr)
        return {};

    return assignment.name() + QStringLiteral(" ") + assignment.type()->name()
           + QStringLiteral(" ::= ") + assignment.value()->asString() + QStringLiteral("\n");
}

QString NodeReconstructingVisitor::valueFor(const Data::Project &project) const
{
    Q_UNUSED(project);
    return {};
}

QString NodeReconstructingVisitor::valueFor(const Data::Root &root) const
{
    Q_UNUSED(root);
    return {};
}

QString NodeReconstructingVisitor::importsAsString(const Data::Definitions &defs) const
{
    const auto &values = defs.importedValues();
    const auto &types = defs.importedTypes();

    if (values.empty() && types.empty())
        return {};

    QString ret("IMPORTS\n");

    ret += importedCollectionAsString<Data::Definitions::ImportedValues>(values);
    ret += importedCollectionAsString<Data::Definitions::ImportedTypes>(types);
    ret += QStringLiteral(";\n");

    return ret;
}

template<typename T>
QString NodeReconstructingVisitor::importedCollectionAsString(const T &types) const
{
    QString ret;
    for (const auto &type : types)
        ret += type.name() + QStringLiteral(" FROM ") + type.module() + QStringLiteral("\n");

    return ret;
}

QString NodeReconstructingVisitor::valuesAsString(const Data::Definitions &defs) const
{
    QString ret;
    for (const auto &value : defs.values()) {
        NodeReconstructingVisitor visitor;
        ret += visitor.valueFor(*value);
    }
    ret += QStringLiteral("\n");

    return ret;
}

QString NodeReconstructingVisitor::typesAsString(const Data::Definitions &defs) const
{
    QString ret;
    for (const auto &typeAssignment : defs.types()) {
        NodeReconstructingVisitor visitor;
        typeAssignment->accept(visitor);

        ret += visitor.value();
    }

    return ret;
}
