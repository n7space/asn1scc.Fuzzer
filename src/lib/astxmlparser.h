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

#include <QXmlStreamReader>

#include <data/acnargument.h>
#include <data/acnparameter.h>
#include <data/acnsequencecomponent.h>
#include <data/definitions.h>
#include <data/file.h>

#include <data/types/type.h>

namespace MalTester {

namespace Data {
namespace ExpressionTree {

class ExpressionNode;
class Range;

} // namespace ExpressionTree
} // namespace Data

class AstXmlParser
{
public:
    explicit AstXmlParser(QXmlStreamReader &xmlReader);

    bool parse();

    std::map<QString, std::unique_ptr<Data::File>> takeData() { return std::move(m_data); }

    using ConstraintType = QString;
    using Constraint = QString;

    using ExpressionNode = Data::ExpressionTree::ExpressionNode;
    using Range = Data::ExpressionTree::Range;

private:
    void readAstRoot();
    void readAsn1File();
    void readModules();
    void readModule();
    void readExportedTypes();
    void readExportedValues();
    void readImportedModules();
    void readTypeAssignments();
    void readValueAssignments();
    void readModuleChildren();
    void readTypeAssignment();
    void readValueAssignment();

    void updateCurrentFile();
    void createNewModule();
    QString readIdAttribute(const QString &id);
    QString readNameAttribute();
    QString readTypeAttribute();
    int readLineAttribute();
    int readCharPossitionInLineAttribute();
    bool isParametrizedTypeInstance() const;

    void readImportedModule();
    void readImportedValues(const QString &moduleName);
    void readImportedValue(const QString &moduleName);
    void readImportedTypes(const QString &moduleName);
    void readImportedType(const QString &moduleName);

    Data::SourceLocation readLocationFromAttributes();
    QStringRef readIsAlignedToNext();

    QString findAndReadValueAssignmentValue();

    std::unique_ptr<Data::Types::Type> findAndReadType();
    std::unique_ptr<Data::Types::Type> readType();
    std::unique_ptr<Data::Types::Type> readTypeDetails(const QString &name,
                                                       const Data::SourceLocation &location,
                                                       const bool isParametrized,
                                                       const QStringRef &typeAlignment);
    std::unique_ptr<Data::Types::Type> buildTypeFromName(const QString &name,
                                                         const Data::SourceLocation &location,
                                                         bool isParametrized);

    std::unique_ptr<Data::Types::Type> createReferenceType(const Data::SourceLocation &location);
    void readReferredTypeDetails(Data::Types::Type &type);
    void readAcnArguments(Data::Types::Type &type);

    Data::AcnParameterPtrs readAcnParameters();
    Data::AcnParameterPtr readAcnParameter();

    void readTypeContents(const QString &name, Data::Types::Type &type);
    void readTypeAttributes(Data::Types::Type &type);

    void readSequence(Data::Types::Type &type);
    void readSequenceComponent(Data::Types::Type &type);
    void readAcnComponent(Data::Types::Type &type);

    void readSequenceOf(Data::Types::Type &type);
    void readChoice(Data::Types::Type &type);
    void readReferenceType(Data::Types::Type &type);

    void readInteger(Data::Types::Type &type);
    void readReal(Data::Types::Type &type);

    void readEnumerated(Data::Types::Type &type);
    void readEnumeratedItem(Data::Types::Type &type);

    void readOctetString(Data::Types::Type &type);
    void readIA5String(Data::Types::Type &type);
    void readNumericString(Data::Types::Type &type);
    void readBitString(Data::Types::Type &type);

    void readConstraints(Data::Types::Type &type, const ConstraintType &constraintName);

    void readExpressionTree(Data::Types::Type &type, const ConstraintType &constraintName);
    const ExpressionNode *readExpressionSubtree(Data::Types::Type &type,
                                                const ConstraintType &constraintName);
    const ExpressionNode *readNodeNextChild(Data::Types::Type &type,
                                            const ConstraintType &constraintName);

    Constraint readValue(const ConstraintType &valName);
    Range *readRange(Data::Types::Type &type, const ConstraintType &constraintName);
    Range *createRangeNode(Data::Types::Type &type,
                           const Constraint &min,
                           const Constraint &max,
                           const ConstraintType &constraintName);

    QString readTypeAssignmentAttribute();
    QString readModuleAttribute();

    bool nextRequiredElementIs(const QString &name);
    bool skipToChildElement(const QString &name);

    QXmlStreamReader &m_xmlReader;
    std::map<QString, std::unique_ptr<Data::File>> m_data;
    QString m_currentFile;
    QString m_currentModule;

    Data::Definitions *m_currentDefinitions;
    bool m_inParametrizedBranch;
};

} // namespace MalTester
