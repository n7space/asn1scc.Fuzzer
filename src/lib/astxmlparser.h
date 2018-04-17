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

    std::unique_ptr<Data::Types::Type> readType();
    std::unique_ptr<Data::Types::Type> readTypeDetails(const Data::SourceLocation &location,
                                                       const bool isParametrized,
                                                       const QStringRef &typeAlignment);
    std::unique_ptr<Data::Types::Type> buildTypeFromName(const Data::SourceLocation &location,
                                                         bool isParametrized);
    std::unique_ptr<Data::Types::Type> createReferenceType(const Data::SourceLocation &location);

    void readTypeContents(const QStringRef &name, std::unique_ptr<Data::Types::Type> &type);
    void readTypeAttributes(std::unique_ptr<Data::Types::Type> &type);

    void readSequence();
    void readSequenceOf();
    void readChoice();
    void readReferenceType(std::unique_ptr<Data::Types::Type> &type);

    void readInteger(std::unique_ptr<Data::Types::Type> &type);
    void readReal(std::unique_ptr<Data::Types::Type> &type);

    void readConstraints(std::unique_ptr<Data::Types::Type> &type, const QString &valName);
    void readRanges(std::unique_ptr<Data::Types::Type> &type, const QString &valName);
    void readRange(std::unique_ptr<Data::Types::Type> &type, const QString &valName);
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
