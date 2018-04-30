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

#include <data/acnparameter.h>
#include <data/definitions.h>
#include <data/file.h>

#include <data/types/type.h>

namespace MalTester {

class AstXmlParser
{
public:
    explicit AstXmlParser(QXmlStreamReader &xmlReader);

    bool parse();

    std::map<QString, std::unique_ptr<Data::File>> takeData() { return std::move(m_data); }

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
    QString readIdAttribute();
    QString readNameAttribute();
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

    std::unique_ptr<Data::Types::Type> findAndReadType();
    std::unique_ptr<Data::Types::Type> readType();
    std::unique_ptr<Data::Types::Type> readTypeDetails(const QStringRef &name,
                                                       const Data::SourceLocation &location,
                                                       const bool isParametrized,
                                                       const QStringRef &typeAlignment);
    std::unique_ptr<Data::Types::Type> buildTypeFromName(const QStringRef &name,
                                                         const Data::SourceLocation &location,
                                                         bool isParametrized);
    std::unique_ptr<Data::Types::Type> createReferenceType(const Data::SourceLocation &location);

    std::unique_ptr<MalTester::Data::AcnParameter> readAcnParameters();
    std::unique_ptr<MalTester::Data::AcnParameter> readAcnParameter();

    void readTypeContents(const QStringRef &name, Data::Types::Type &type);
    void readTypeAttributes(Data::Types::Type &type);

    void readSequence(Data::Types::Type &type);
    void readSequenceOf(Data::Types::Type &type);
    void readChoice(Data::Types::Type &type);
    void readReferenceType();

    void readInteger(Data::Types::Type &type);
    void readReal(Data::Types::Type &type);

    void readEnumerated(Data::Types::Type &type);
    void readEnumeratedItem(Data::Types::Type &type);

    void readConstraints(Data::Types::Type &type, const QString &valName);
    void readConstraint(std::unique_ptr<Data::Types::Type> &type, const QString &valName);

    void readRanges(Data::Types::Type &type, const QString &valName);
    void readRange(Data::Types::Type &type, const QString &valName);
    QString readValue(const QString &valName);

    QString readTypeAssignmentAttribute();
    QString readModuleAttribute();

    bool nextRequiredElementIs(const QString &name);
    bool skipToChildElement(const QString &name);

    QXmlStreamReader &m_xmlReader;
    std::map<QString, std::unique_ptr<Data::File>> m_data;
    QString m_currentFile;
    QString m_currentModule;
    Data::Definitions *m_currentDefinitions;
};

} // namespace MalTester
