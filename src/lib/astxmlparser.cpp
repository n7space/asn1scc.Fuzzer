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

#include "astxmlparser.h"

#include <data/sourcelocation.h>

#include <data/types/builtintypes.h>
#include <data/types/userdefinedtype.h>

using namespace MalTester;

AstXmlParser::AstXmlParser(QXmlStreamReader &xmlReader)
    : m_xmlReader(xmlReader)
    , m_currentDefinitions(nullptr)
{}

bool AstXmlParser::parse()
{
    if (nextRequiredElementIs(QStringLiteral("AstRoot")))
        readAstRoot();
    return !m_xmlReader.hasError();
}

void AstXmlParser::readAstRoot()
{
    while (skipToChildElement(QStringLiteral("Asn1File")))
        readAsn1File();
}

void AstXmlParser::readAsn1File()
{
    updateCurrentFile();
    if (nextRequiredElementIs(QStringLiteral("Modules")))
        readModules();
    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readModules()
{
    while (nextRequiredElementIs(QStringLiteral("Module")))
        readModule();
}

void AstXmlParser::updateCurrentFile()
{
    m_currentFile = m_xmlReader.attributes().value(QStringLiteral("FileName")).toString();
    m_data.insert(std::make_pair(m_currentFile, std::make_unique<Data::File>(m_currentFile)));
}

void AstXmlParser::readModuleChildren()
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("ExportedTypes"))
            readExportedTypes();
        else if (m_xmlReader.name() == QStringLiteral("ExportedValues"))
            readExportedValues();
        else if (m_xmlReader.name() == QStringLiteral("ImportedModules"))
            readImportedModules();
        else if (m_xmlReader.name() == QStringLiteral("TypeAssignments"))
            readTypeAssignments();
        else if (m_xmlReader.name() == QStringLiteral("ValueAssignments"))
            readValueAssignments();
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::createNewModule()
{
    const auto location = readLocationFromAttributes();
    m_currentModule = readNameAttribute();
    auto module = std::make_unique<Data::Definitions>(m_currentModule, location);
    m_currentDefinitions = module.get();
    m_data[m_currentFile]->add(std::move(module));
}

void AstXmlParser::readModule()
{
    createNewModule();
    readModuleChildren();
}

QString AstXmlParser::readIdAttribute()
{
    return m_xmlReader.attributes().value("ID").toString();
}

void AstXmlParser::readValueAssignments()
{
    while (nextRequiredElementIs(QStringLiteral("ValueAssignment")))
        readValueAssignment();
}

void AstXmlParser::readTypeAssignments()
{
    while (nextRequiredElementIs(QStringLiteral("TypeAssignment")))
        readTypeAssignment();
}

void AstXmlParser::readTypeAssignment()
{
    Q_ASSERT(m_currentDefinitions != nullptr);
    const auto location = readLocationFromAttributes();
    const auto name = readNameAttribute();
    auto type = readType();
    m_xmlReader.skipCurrentElement();

    m_currentDefinitions->addType(
        std::make_unique<Data::TypeAssignment>(name, location, std::move(type)));
    m_data[m_currentFile]->addTypeReference(
        std::make_unique<Data::TypeReference>(name, m_currentDefinitions->name(), location));
}

void AstXmlParser::readValueAssignment()
{
    Q_ASSERT(m_currentDefinitions != nullptr);
    const auto location = readLocationFromAttributes();
    const auto name = readNameAttribute();
    auto type = readType();
    m_xmlReader.skipCurrentElement();

    m_currentDefinitions->addValue(
        std::make_unique<Data::ValueAssignment>(name, location, std::move(type)));
}

QString AstXmlParser::readTypeAssignmentAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("TypeAssignment")).toString();
}

QString AstXmlParser::readModuleAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("Module")).toString();
}

QString AstXmlParser::readNameAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("Name")).toString();
}

int AstXmlParser::readLineAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("Line")).toInt();
}

int AstXmlParser::readCharPossitionInLineAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("CharPositionInLine")).toInt();
}

void AstXmlParser::readImportedModules()
{
    while (nextRequiredElementIs(QStringLiteral("ImportedModule")))
        readImportedModule();
}

void AstXmlParser::readImportedModule()
{
    const auto moduleName = readIdAttribute();
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("ImportedTypes"))
            readImportedTypes(moduleName);
        else if (m_xmlReader.name() == QStringLiteral("ImportedValues"))
            readImportedValues(moduleName);
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readImportedValues(const QString &moduleName)
{
    while (nextRequiredElementIs(QStringLiteral("ImportedValue")))
        readImportedValue(moduleName);
}

void AstXmlParser::readImportedValue(const QString &moduleName)
{
    m_currentDefinitions->addImportedValue({moduleName, readNameAttribute()});
    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readImportedTypes(const QString &moduleName)
{
    while (nextRequiredElementIs(QStringLiteral("ImportedType")))
        readImportedType(moduleName);
}

void AstXmlParser::readImportedType(const QString &moduleName)
{
    m_currentDefinitions->addImportedType({moduleName, readNameAttribute()});
    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readExportedValues()
{
    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readExportedTypes()
{
    m_xmlReader.skipCurrentElement();
}

bool AstXmlParser::nextRequiredElementIs(const QString &name)
{
    if (!m_xmlReader.readNextStartElement())
        return false;
    if (m_xmlReader.name() == name)
        return true;
    m_xmlReader.raiseError(QString("XML does not contain expected <%1> element").arg(name));
    return false;
}

Data::SourceLocation AstXmlParser::readLocationFromAttributes()
{
    return {m_currentFile, readLineAttribute(), readCharPossitionInLineAttribute()};
}

QStringRef AstXmlParser::readIsAlignedToNext()
{
    return m_xmlReader.attributes().value(QLatin1String("align-to-next"));
}

std::unique_ptr<Data::Types::Type> AstXmlParser::readType()
{
    if (!skipToChildElement(QStringLiteral("Asn1Type")))
        return {};

    const auto location = readLocationFromAttributes();
    const auto isParametrized = isParametrizedTypeInstance();
    const auto alignToNext = readIsAlignedToNext();

    auto type = readTypeDetails(location, isParametrized, alignToNext);

    m_xmlReader.skipCurrentElement();

    return type;
}

bool AstXmlParser::isParametrizedTypeInstance() const
{
    const auto value = m_xmlReader.attributes().value(QLatin1String("ParameterizedTypeInstance"));
    return !value.isNull() && value == QStringLiteral("true");
}

std::unique_ptr<Data::Types::Type> AstXmlParser::readTypeDetails(
    const Data::SourceLocation &location, const bool isParametrized, const QStringRef &typeAlignment)
{
    auto type = buildTypeFromName(location, isParametrized);

    if (type && type->acnParams())
        type->acnParams()->setAlignToNext(Data::AcnParameters::mapAlignToNext(typeAlignment));

    return type;
}

std::unique_ptr<Data::Types::Type> AstXmlParser::buildTypeFromName(
    const Data::SourceLocation &location, bool isParametrized)
{
    if (!m_xmlReader.readNextStartElement())
        return nullptr;

    auto name = m_xmlReader.name();
    auto type = (name == QStringLiteral("REFERENCE_TYPE"))
                    ? createReferenceType(location)
                    : Data::Types::BuiltinType::createBuiltinType(name.toString());

    if (isParametrized) {
        m_xmlReader.skipCurrentElement();
        return type;
    }

    readTypeContents(name, type);

    return type;
}

std::unique_ptr<Data::Types::Type> AstXmlParser::createReferenceType(
    const Data::SourceLocation &location)
{
    const QString refName = readTypeAssignmentAttribute();
    const QString module = readModuleAttribute();

    auto ref = std::make_unique<Data::TypeReference>(refName, module, location);

    m_data[m_currentFile]->addTypeReference(std::move(ref));

    return std::make_unique<Data::Types::UserdefinedType>(refName, module);
}

void AstXmlParser::readTypeContents(const QStringRef &name, std::unique_ptr<Data::Types::Type> &type)
{
    if (name == QStringLiteral("SEQUENCE"))
        readSequence();
    else if (name == QStringLiteral("SEQUENCE_OF"))
        readSequenceOf();
    else if (name == QStringLiteral("CHOICE"))
        readChoice();
    else if (name == QStringLiteral("REFERENCE_TYPE"))
        readReferenceType(type);
    else if (name == QStringLiteral("INTEGER"))
        readInteger(type);
    else
        m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readSequence()
{
    while (skipToChildElement(QStringLiteral("SEQUENCE_COMPONENT"))) {
        readType();
        m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readSequenceOf()
{
    if (readType())
        m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readChoice()
{
    while (skipToChildElement(QStringLiteral("CHOICE_ALTERNATIVE"))) {
        readType();
        m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readInteger(std::unique_ptr<Data::Types::Type> &type)
{
    readIntegerAcnParams(type);
    readConstraint(type, "IntegerValue");
}

void AstXmlParser::readIntegerAcnParams(std::unique_ptr<Data::Types::Type> &type)
{
    auto acnParams = type->acnParams();
    if (!acnParams)
        return;

    acnParams->setSize(m_xmlReader.attributes().value(QLatin1String("size")).toInt());
    acnParams->setEndianness(Data::AcnParameters::mapEndianess(
        m_xmlReader.attributes().value(QLatin1String("endianness"))));
    acnParams->setEncoding(Data::AcnParameters::mapEncoding(
        m_xmlReader.attributes().value(QLatin1String("encoding"))));
    acnParams->setAlignToNext(Data::AcnParameters::mapAlignToNext(
        m_xmlReader.attributes().value(QLatin1String("align-to-next"))));
}

void AstXmlParser::readReferenceType(std::unique_ptr<Data::Types::Type> &type)
{
    Q_UNUSED(type);
    readType();
}

void AstXmlParser::readConstraint(std::unique_ptr<Data::Types::Type> &type, const QString &valName)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("Constraints"))
            readRanges(type, valName);
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readRanges(std::unique_ptr<Data::Types::Type> &type, const QString &valName)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("OR"))
            readRanges(type, valName);
        else if (m_xmlReader.name() == QStringLiteral("Range"))
            readRange(type, valName);
        else if (m_xmlReader.name() == valName) {
            QString val = m_xmlReader.readElementText();
            type->constraint()->addRange(Data::Constraint::StringPair(val, val));
        } else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readRange(std::unique_ptr<Data::Types::Type> &type, const QString &valName)
{
    QString a, b;
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("a"))
            a = readValue(valName);
        else if (m_xmlReader.name() == QStringLiteral("b"))
            b = readValue(valName);
        else
            m_xmlReader.skipCurrentElement();
    }

    type->constraint()->addRange(Data::Constraint::StringPair(a, b));
}

QString AstXmlParser::readValue(const QString &valName)
{
    QString val;
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == valName)
            val = m_xmlReader.readElementText();
        else
            m_xmlReader.skipCurrentElement();
    }

    return val;
}

bool AstXmlParser::skipToChildElement(const QString &name)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == name)
            return true;
        else
            m_xmlReader.skipCurrentElement();
    }

    return false;
}
