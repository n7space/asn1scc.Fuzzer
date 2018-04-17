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

#include <data/types/enumerated.h>
#include <data/types/integer.h>
#include <data/types/real.h>
#include <data/types/sequenceof.h>
#include <data/types/typefactory.h>
#include <data/types/typevisitor.h>
#include <data/types/userdefinedtype.h>

using namespace MalTester;

namespace {
class TypeAttributesAssigningVisitor : public Data::Types::TypeVisitor
{
public:
    TypeAttributesAssigningVisitor(const QXmlStreamAttributes &attributes)
        : m_attributes(attributes)
    {}

    ~TypeAttributesAssigningVisitor() override {}

    void visit(Data::Types::Boolean &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::Null &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::BitString &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::OctetString &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::IA5String &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::NumericString &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::Enumerated &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::Choice &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::Sequence &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::SequenceOf &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::Real &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::LabelType &type) override { Q_UNUSED(type); }

    void visit(Data::Types::Integer &type) override
    {
        using namespace Data::Types;

        type.setSize(m_attributes.value(QLatin1String("size")).toInt());
        type.setEndianness(Integer::mapEndianess(m_attributes.value(QLatin1String("endianness"))));
        type.setEncoding(Integer::mapEncoding(m_attributes.value(QLatin1String("encoding"))));
    }

    void visit(Data::Types::UserdefinedType &type) override { Q_UNUSED(type); }

private:
    const QXmlStreamAttributes &m_attributes;
};

class RangeConstraintAssigningVisitor : public Data::Types::TypeVisitor
{
public:
    RangeConstraintAssigningVisitor(QXmlStreamReader &xmlReader,
                                    const QString &begin,
                                    const QString &end)
        : m_xmlReader(xmlReader)
        , m_begin(begin)
        , m_end(end)
    {}

    ~RangeConstraintAssigningVisitor() override {}

    void visit(Data::Types::Boolean &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::Null &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::BitString &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::OctetString &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::IA5String &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::NumericString &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::Enumerated &type) override
    {
        const auto &items = type.items();
        if (!items.contains(m_begin)) {
            m_xmlReader.raiseError("Incorrect ENUMERATED value: " + m_begin);
            return;
        }

        const auto val = items.value(m_begin).value();
        type.constraints().addRange(val, val);
    }

    void visit(Data::Types::Choice &type) override { Q_UNUSED(type); }

    void visit(Data::Types::Sequence &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::SequenceOf &type) override
    {
        addRange(type.constraints(), "Incorrect SIZE range for SEQUENCE OF");
    }

    void visit(Data::Types::Real &type) override
    {
        bool beginOk = false;
        bool endOk = false;
        type.constraints().addRange(m_begin.toDouble(&beginOk), m_end.toDouble(&endOk));
        if (!beginOk || !endOk)
            m_xmlReader.raiseError("Incorrect range for REAL: " + m_begin + " " + m_end);
    }

    void visit(Data::Types::LabelType &type) override { Q_UNUSED(type); }

    void visit(Data::Types::Integer &type) override
    {
        addRange(type.constraints(), "Incorrect range for INTEGER");
    }

    void visit(Data::Types::UserdefinedType &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

private:
    void addRange(Data::Types::IntegerConstraints &constraints, const QString &message)
    {
        bool beginOk = false;
        bool endOk = false;
        constraints.addRange(m_begin.toInt(&beginOk), m_end.toInt(&endOk));
        if (!beginOk || !endOk)
            m_xmlReader.raiseError(message + ": " + m_begin + " " + m_end);
    }

    QXmlStreamReader &m_xmlReader;
    const QString m_begin;
    const QString m_end;
};
} // namespace

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
    auto type = findAndReadType();
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
    auto type = findAndReadType();
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

std::unique_ptr<Data::Types::Type> AstXmlParser::findAndReadType()
{
    if (!skipToChildElement(QStringLiteral("Asn1Type")))
        return {};
    return readType();
}

std::unique_ptr<Data::Types::Type> AstXmlParser::readType()
{
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
    type->setAlignToNext(Data::Types::Type::mapAlignToNext(typeAlignment));

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
                    : Data::Types::TypeFactory::createBuiltinType(name.toString());

    if (isParametrized) {
        m_xmlReader.skipCurrentElement();
        return type;
    }

    readTypeAttributes(*type);
    readTypeContents(name, *type);

    return type;
}

void AstXmlParser::readTypeAttributes(Data::Types::Type &type)
{
    TypeAttributesAssigningVisitor visitor(m_xmlReader.attributes());
    type.accept(visitor);
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

void AstXmlParser::readTypeContents(const QStringRef &name, Data::Types::Type &type)
{
    if (name == QStringLiteral("SEQUENCE"))
        readSequence();
    else if (name == QStringLiteral("SEQUENCE_OF"))
        readSequenceOf(type);
    else if (name == QStringLiteral("CHOICE"))
        readChoice();
    else if (name == QStringLiteral("REFERENCE_TYPE"))
        readReferenceType();
    else if (name == QStringLiteral("INTEGER"))
        readInteger(type);
    else if (name == QStringLiteral("REAL"))
        readReal(type);
    else if (name == QStringLiteral("Enumerated"))
        readEnumerated(type);
    else
        m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readSequence()
{
    while (skipToChildElement(QStringLiteral("SEQUENCE_COMPONENT"))) {
        findAndReadType();
        m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readSequenceOf(Data::Types::Type &type)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("Constraints"))
            readRanges(type, "IntegerValue");
        else if (m_xmlReader.name() == QStringLiteral("Asn1Type"))
            readType();
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readChoice()
{
    while (skipToChildElement(QStringLiteral("CHOICE_ALTERNATIVE"))) {
        findAndReadType();
        m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readInteger(Data::Types::Type &type)
{
    readConstraints(type, "IntegerValue");
}

void AstXmlParser::readReal(Data::Types::Type &type)
{
    readConstraints(type, "RealValue");
}

void AstXmlParser::readEnumerated(Data::Types::Type &type)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("Items"))
            readEnumeratedItem(type);
        else if (m_xmlReader.name() == QStringLiteral("Constraints"))
            readRanges(type, "EnumValue");
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readEnumeratedItem(Data::Types::Type &type)
{
    auto &enumType = dynamic_cast<Data::Types::Enumerated &>(type);

    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("Item")) {
            const auto itemName = readNameAttribute();
            enumType.addItem(itemName,
                             {itemName, readValueFromAttributes(), readLocationFromAttributes()});
        }

        m_xmlReader.skipCurrentElement();
    }
}

int AstXmlParser::readValueFromAttributes()
{
    return m_xmlReader.attributes().value("Value").toInt();
}

void AstXmlParser::readReferenceType()
{
    findAndReadType();
}

void AstXmlParser::readConstraints(Data::Types::Type &type, const QString &valName)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("Constraints"))
            readRanges(type, valName);
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readRanges(Data::Types::Type &type, const QString &valName)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("OR")
            || m_xmlReader.name() == QStringLiteral("SIZE"))
            readRanges(type, valName);
        else if (m_xmlReader.name() == QStringLiteral("Range"))
            readRange(type, valName);
        else if (m_xmlReader.name() == valName) {
            QString val = m_xmlReader.readElementText();
            RangeConstraintAssigningVisitor visitor(m_xmlReader, val, val);
            type.accept(visitor);
        } else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readRange(Data::Types::Type &type, const QString &valName)
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

    RangeConstraintAssigningVisitor visitor(m_xmlReader, a, b);
    type.accept(visitor);
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
