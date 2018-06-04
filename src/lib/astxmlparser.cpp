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

#include <data/asnsequencecomponent.h>
#include <data/ranges.h>
#include <data/sourcelocation.h>

#include <data/types/acnparameterizablecomposite.h>
#include <data/types/bitstring.h>
#include <data/types/boolean.h>
#include <data/types/choice.h>
#include <data/types/enumerated.h>
#include <data/types/ia5string.h>
#include <data/types/integer.h>
#include <data/types/integeracnparams.h>
#include <data/types/null.h>
#include <data/types/numericstring.h>
#include <data/types/octetstring.h>
#include <data/types/real.h>
#include <data/types/sequence.h>
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
        type.setTrueValue(m_attributes.value(QLatin1String("true-value")).toString());
        type.setFalseValue(m_attributes.value(QLatin1String("false-value")).toString());
    }

    void visit(Data::Types::Null &type) override
    {
        type.setPattern(m_attributes.value(QLatin1String("pattern")).toString());
    }

    void visit(Data::Types::BitString &type) override
    {
        type.setSize(m_attributes.value(QStringLiteral("size")).toString());
    }

    void visit(Data::Types::OctetString &type) override
    {
        type.setSize(m_attributes.value(QStringLiteral("size")).toString());
    }

    void visit(Data::Types::IA5String &type) override
    {
        using namespace Data::Types;

        type.setSize(m_attributes.value(QStringLiteral("size")).toString());
        type.setTerminationPattern(
            m_attributes.value(QStringLiteral("termination-pattern")).toString());
        type.setEncoding(IA5String::mapEncoding(m_attributes.value(QStringLiteral("encoding"))));
    }

    void visit(Data::Types::NumericString &type) override
    {
        using namespace Data::Types;

        type.setSize(m_attributes.value(QStringLiteral("size")).toString());
        type.setTerminationPattern(
            m_attributes.value(QStringLiteral("termination-pattern")).toString());
        type.setEncoding(NumericString::mapEncoding(m_attributes.value(QStringLiteral("encoding"))));
    }

    void visit(Data::Types::Enumerated &type) override
    {
        setIntegerAcnParameters(type);

        const auto encodeValues = m_attributes.value(QStringLiteral("encode-values"));
        type.setEncodeValues(encodeValues.toString().toLower() == "true");
    }

    void visit(Data::Types::Choice &type) override
    {
        type.setDeterminant(m_attributes.value(QLatin1String("determinant")).toString());
    }

    void visit(Data::Types::Sequence &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::SequenceOf &type) override
    {
        type.setSize(m_attributes.value(QLatin1String("size")).toString());
    }

    void visit(Data::Types::Real &type) override
    {
        using namespace Data::Types;

        type.setEndianness(Type::mapEndianess(m_attributes.value(QLatin1String("endianness"))));
        type.setEncoding(Real::mapEncoding(m_attributes.value(QLatin1String("encoding"))));
    }

    void visit(Data::Types::LabelType &type) override { Q_UNUSED(type); }

    void visit(Data::Types::Integer &type) override { setIntegerAcnParameters(type); }

    void visit(Data::Types::UserdefinedType &type) override { Q_UNUSED(type); }

private:
    void setIntegerAcnParameters(Data::Types::IntegerAcnParameters &type)
    {
        using namespace Data::Types;

        type.setSize(m_attributes.value(QLatin1String("size")).toInt());
        type.setEndianness(Type::mapEndianess(m_attributes.value(QLatin1String("endianness"))));
        type.setEncoding(Integer::mapEncoding(m_attributes.value(QLatin1String("encoding"))));
    }

    const QXmlStreamAttributes &m_attributes;
};

class OperatorAssigningVisitor : public Data::Types::TypeVisitor
{
public:
    OperatorAssigningVisitor(const QString &operatorType)
        : m_operatorType(operatorType)
    {}

    ~OperatorAssigningVisitor() override = default;

    void visit(Data::Types::Boolean &type) override { Q_UNUSED(type); }
    void visit(Data::Types::Null &type) override { Q_UNUSED(type); }

    void visit(Data::Types::BitString &type) override { addOperatorToStringType(type); }
    void visit(Data::Types::OctetString &type) override { addOperatorToStringType(type); }
    void visit(Data::Types::IA5String &type) override { addOperatorToStringType(type); }
    void visit(Data::Types::NumericString &type) override { addOperatorToStringType(type); }

    void visit(Data::Types::Enumerated &type) override
    {
        type.constraints().addOperator(m_operatorType);
    }

    void visit(Data::Types::Choice &type) override { Q_UNUSED(type); }
    void visit(Data::Types::Sequence &type) override { Q_UNUSED(type); }

    void visit(Data::Types::SequenceOf &type) override
    {
        type.constraints().addOperator(m_operatorType);
    }

    void visit(Data::Types::Real &type) override { type.constraints().addOperator(m_operatorType); }

    void visit(Data::Types::LabelType &type) override { Q_UNUSED(type); }

    void visit(Data::Types::Integer &type) override
    {
        type.constraints().addOperator(m_operatorType);
    }

    void visit(Data::Types::UserdefinedType &type) override { Q_UNUSED(type); }

private:
    void addOperatorToStringType(Data::Types::String &type)
    {
        type.constraints().addOperator(m_operatorType);
    }

    const QString m_operatorType;
};

class RangeConstraintAssigningVisitor : public Data::Types::TypeVisitor
{
public:
    RangeConstraintAssigningVisitor(QXmlStreamReader &xmlReader,
                                    const AstXmlParser::Constraint &begin,
                                    const AstXmlParser::Constraint &end)
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
        addRangeToStringType(QStringLiteral("IntegerValue"), QStringLiteral("BitStringValue"), type);
    }

    void visit(Data::Types::OctetString &type) override
    {
        addRangeToStringType(QStringLiteral("IntegerValue"),
                             QStringLiteral("OctetStringValue"),
                             type);
    }

    void visit(Data::Types::IA5String &type) override
    {
        addRangeToStringType(QStringLiteral("IntegerValue"), QStringLiteral("StringValue"), type);
    }

    void visit(Data::Types::NumericString &type) override
    {
        addRangeToStringType(QStringLiteral("IntegerValue"), QStringLiteral("StringValue"), type);
    }

    void visit(Data::Types::Enumerated &type) override
    {
        const auto &items = type.items();
        if (!items.contains(m_begin.value)) {
            m_xmlReader.raiseError("Incorrect ENUMERATED value: " + m_begin.value);
            return;
        }

        const auto val = items.value(m_begin.value).value();
        type.constraints().addRange(new Data::EnumeratedRange(val, val));
    }

    void visit(Data::Types::Choice &type) override { Q_UNUSED(type); }

    void visit(Data::Types::Sequence &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

    void visit(Data::Types::SequenceOf &type) override
    {
        addIntegerRange(type.constraints(), "Incorrect SIZE range for SEQUENCE OF");
    }

    void visit(Data::Types::Real &type) override
    {
        bool beginOk = false;
        bool endOk = false;
        auto range = new Data::RealRange(m_begin.value.toDouble(&beginOk),
                                         m_end.value.toDouble(&endOk));
        if (!beginOk || !endOk) {
            m_xmlReader.raiseError("Incorrect range for REAL: " + m_begin.value + " " + m_end.value);
            return;
        }

        type.constraints().addRange(range);
    }

    void visit(Data::Types::LabelType &type) override { Q_UNUSED(type); }

    void visit(Data::Types::Integer &type) override
    {
        addIntegerRange(type.constraints(), "Incorrect range for INTEGER");
    }

    void visit(Data::Types::UserdefinedType &type) override
    {
        Q_UNUSED(type);
        // TODO?
    }

private:
    void addIntegerRange(Data::Types::RangeConstraints &constraints, const QString &message)
    {
        bool beginOk = false;
        bool endOk = false;
        auto range = new Data::IntegerRange(m_begin.value.toInt(&beginOk),
                                            m_end.value.toInt(&endOk));
        if (!beginOk || !endOk) {
            m_xmlReader.raiseError(message + ": " + m_begin.value + " " + m_end.value);
            return;
        }

        constraints.addRange(range);
    }

    void addRangeToStringType(const QString &intConstraintName,
                              const QString &stringConstraintName,
                              Data::Types::String &type)
    {
        if (m_begin.type != m_end.type) {
            m_xmlReader.raiseError("Range types mismatch: " + m_begin.type + " " + m_end.type);
            return;
        }

        if (m_begin.type == intConstraintName)
            addIntegerRange(type.constraints(), "Incorrect range for string type");
        else if (m_begin.type == stringConstraintName)
            type.constraints().addRange(new Data::StringRange(m_begin.value, m_end.value));
    }

    QXmlStreamReader &m_xmlReader;
    const AstXmlParser::Constraint m_begin;
    const AstXmlParser::Constraint m_end;
};

class ChildItemAddingVisitor : public Data::Types::TypeVisitor
{
public:
    ChildItemAddingVisitor(const QXmlStreamAttributes &attributes,
                           const QString &currentFile,
                           std::unique_ptr<Data::Types::Type> childType = nullptr)
        : m_attributes(attributes)
        , m_currentFile(currentFile)
        , m_childType(std::move(childType))
    {}

    ~ChildItemAddingVisitor() override {}

    void visit(Data::Types::Boolean &type) override { Q_UNUSED(type); }
    void visit(Data::Types::Null &type) override { Q_UNUSED(type); }
    void visit(Data::Types::BitString &type) override { Q_UNUSED(type); }
    void visit(Data::Types::OctetString &type) override { Q_UNUSED(type); }
    void visit(Data::Types::IA5String &type) override { Q_UNUSED(type); }
    void visit(Data::Types::NumericString &type) override { Q_UNUSED(type); }

    void visit(Data::Types::Enumerated &type) override
    {
        const auto itemName = readStringAttribute(QStringLiteral("Name"));
        const auto value = readIntegerAttribute(QStringLiteral("Value"));
        Data::SourceLocation location(m_currentFile,
                                      readIntegerAttribute(QStringLiteral("Line")),
                                      readIntegerAttribute(QStringLiteral("CharPositionInLine")));
        type.addItem(itemName, {itemName, value, location});
    }

    void visit(Data::Types::Choice &type) override
    {
        const auto name = readStringAttribute(QStringLiteral("Name"));
        const auto presentWhenName = readStringAttribute(QStringLiteral("PresentWhenName"));
        const auto adaNameAttribute = readStringAttribute(QStringLiteral("AdaName"));
        const auto cNameAttribute = readStringAttribute(QStringLiteral("CName"));
        const auto presentWhen = readStringAttribute(QStringLiteral("present-when"));

        Data::SourceLocation location(m_currentFile,
                                      readIntegerAttribute(QStringLiteral("Line")),
                                      readIntegerAttribute(QStringLiteral("CharPositionInLine")));

        type.addComponent(std::make_unique<Data::Types::ChoiceAlternative>(name,
                                                                           presentWhenName,
                                                                           adaNameAttribute,
                                                                           cNameAttribute,
                                                                           presentWhen,
                                                                           location,
                                                                           std::move(m_childType)));
    }

    void visit(Data::Types::Sequence &type) override
    {
        const auto name = readStringAttribute(QStringLiteral("Name"));
        const auto presentWhen = readStringAttribute(QStringLiteral("present-when"));
        const auto optional = readStringAttribute(QStringLiteral("Optional")).toLower();
        Data::SourceLocation location(m_currentFile,
                                      readIntegerAttribute(QStringLiteral("Line")),
                                      readIntegerAttribute(QStringLiteral("CharPositionInLine")));

        type.addComponent(std::make_unique<Data::AsnSequenceComponent>(name,
                                                                       optional == "true",
                                                                       presentWhen,
                                                                       location,
                                                                       std::move(m_childType)));
    }

    void visit(Data::Types::SequenceOf &type) override
    {
        type.setItemsType(std::move(m_childType));
    }

    void visit(Data::Types::Real &type) override { Q_UNUSED(type); }
    void visit(Data::Types::LabelType &type) override { Q_UNUSED(type); }
    void visit(Data::Types::Integer &type) override { Q_UNUSED(type); }
    void visit(Data::Types::UserdefinedType &type) override
    {
        type.setType(std::move(m_childType));
    }

private:
    int readIntegerAttribute(const QString &key) const { return m_attributes.value(key).toInt(); }
    QString readStringAttribute(const QString &key) const
    {
        return m_attributes.value(key).toString();
    }

    const QXmlStreamAttributes &m_attributes;
    const QString &m_currentFile;
    std::unique_ptr<Data::Types::Type> m_childType;
};

class AcnDefinedItemsAddingVisitor : public Data::Types::TypeVisitor
{
public:
    AcnDefinedItemsAddingVisitor(Data::AcnParameterPtrs acnParameters)
        : m_acnParameters(std::move(acnParameters))
    {}

    AcnDefinedItemsAddingVisitor(Data::AcnArgumentPtrs acnArguments)
        : m_acnArguments(std::move(acnArguments))
    {}

    AcnDefinedItemsAddingVisitor(Data::AcnSequenceComponentPtr acnComponent)
        : m_acnComponent(std::move(acnComponent))
    {}

    ~AcnDefinedItemsAddingVisitor() override {}

    void visit(Data::Types::Boolean &type) override { Q_UNUSED(type); }
    void visit(Data::Types::Null &type) override { Q_UNUSED(type); }
    void visit(Data::Types::BitString &type) override { Q_UNUSED(type); }
    void visit(Data::Types::OctetString &type) override { Q_UNUSED(type); }
    void visit(Data::Types::IA5String &type) override { Q_UNUSED(type); }
    void visit(Data::Types::NumericString &type) override { Q_UNUSED(type); }
    void visit(Data::Types::Enumerated &type) override { Q_UNUSED(type); }
    void visit(Data::Types::Choice &type) override
    {
        for (auto &param : m_acnParameters)
            type.addParameter(std::move(param));
    }

    void visit(Data::Types::Sequence &type) override
    {
        for (auto &param : m_acnParameters)
            type.addParameter(std::move(param));

        if (m_acnComponent)
            type.addComponent(std::move(m_acnComponent));
    }

    void visit(Data::Types::SequenceOf &type) override { Q_UNUSED(type); }
    void visit(Data::Types::Real &type) override { Q_UNUSED(type); }
    void visit(Data::Types::LabelType &type) override { Q_UNUSED(type); }
    void visit(Data::Types::Integer &type) override { Q_UNUSED(type); }
    void visit(Data::Types::UserdefinedType &type) override
    {
        for (auto &arg : m_acnArguments)
            type.addArgument(std::move(arg));
    }

private:
    Data::AcnParameterPtrs m_acnParameters;
    Data::AcnArgumentPtrs m_acnArguments;
    Data::AcnSequenceComponentPtr m_acnComponent;
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

QString AstXmlParser::readIdAttribute(const QString &id)
{
    return m_xmlReader.attributes().value(id).toString();
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

QString AstXmlParser::readTypeAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("Type")).toString();
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
    const auto moduleName = readIdAttribute(QStringLiteral("ID"));
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

static bool isTypeName(const QString &name)
{
    // clang-format off
    return name == QStringLiteral("BIT_STRING")
           || name == QStringLiteral("BOOLEAN")
           || name == QStringLiteral("CHOICE")
           || name == QStringLiteral("ENUMERATED")
           || name == QStringLiteral("NumericString")
           || name == QStringLiteral("IA5String")
           || name == QStringLiteral("INTEGER")
           || name == QStringLiteral("NULL")
           || name == QStringLiteral("NumericString")
           || name == QStringLiteral("OCTET_STRING")
           || name == QStringLiteral("REAL")
           || name == QStringLiteral("SEQUENCE")
           || name == QStringLiteral("SEQUENCE_OF")
           || name == QStringLiteral("REFERENCE_TYPE");
    // clang-format on
}

std::unique_ptr<Data::Types::Type> AstXmlParser::readType()
{
    const auto location = readLocationFromAttributes();
    const auto isParametrized = isParametrizedTypeInstance();
    const auto alignToNext = readIsAlignedToNext();
    std::unique_ptr<Data::Types::Type> type = nullptr;
    Data::AcnParameterPtrs acnParameters;

    while (m_xmlReader.readNextStartElement()) {
        const auto name = m_xmlReader.name().toString();

        if (name == QStringLiteral("AcnParameters"))
            acnParameters = readAcnParameters();
        else if (isTypeName(name))
            type = readTypeDetails(name, location, isParametrized, alignToNext);
        else
            m_xmlReader.skipCurrentElement();
    }

    AcnDefinedItemsAddingVisitor visitor(std::move(acnParameters));
    type->accept(visitor);

    return type;
}

Data::AcnParameterPtrs AstXmlParser::readAcnParameters()
{
    Data::AcnParameterPtrs parameters;

    while (skipToChildElement(QStringLiteral("AcnParameter")))
        parameters.push_back(readAcnParameter());

    return parameters;
}

Data::AcnParameterPtr AstXmlParser::readAcnParameter()
{
    const auto id = readIdAttribute(QStringLiteral("Id"));
    const auto name = readNameAttribute();
    const auto type = readTypeAttribute();

    m_xmlReader.skipCurrentElement();

    return std::make_unique<Data::AcnParameter>(id, name, type);
}

bool AstXmlParser::isParametrizedTypeInstance() const
{
    const auto value = m_xmlReader.attributes().value(QStringLiteral("ParameterizedTypeInstance"));
    return !value.isNull() && value == QStringLiteral("true");
}

std::unique_ptr<Data::Types::Type> AstXmlParser::readTypeDetails(const QString &name,
                                                                 const Data::SourceLocation &location,
                                                                 const bool isParametrized,
                                                                 const QStringRef &typeAlignment)
{
    auto type = buildTypeFromName(name, location, isParametrized);
    type->setAlignToNext(Data::Types::Type::mapAlignToNext(typeAlignment));

    return type;
}

std::unique_ptr<Data::Types::Type> AstXmlParser::buildTypeFromName(
    const QString &name, const Data::SourceLocation &location, bool isParametrized)
{
    auto type = (name == QStringLiteral("REFERENCE_TYPE"))
                    ? createReferenceType(location)
                    : Data::Types::TypeFactory::createBuiltinType(name);

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

    auto userDefinedType = std::make_unique<Data::Types::UserdefinedType>(refName, module);

    return userDefinedType;
}

void AstXmlParser::readReferredTypeDetails(Data::Types::Type &type)
{
    auto referedType = readType();
    ChildItemAddingVisitor visitor(m_xmlReader.attributes(), m_currentFile, std::move(referedType));
    type.accept(visitor);
}

void AstXmlParser::readAcnArguments(Data::Types::Type &type)
{
    Data::AcnArgumentPtrs arguments;
    while (skipToChildElement("argument"))
        arguments.push_back(std::make_unique<Data::AcnArgument>(m_xmlReader.readElementText()));

    AcnDefinedItemsAddingVisitor visitor(std::move(arguments));
    type.accept(visitor);
}

void AstXmlParser::readTypeContents(const QString &name, Data::Types::Type &type)
{
    if (name == QStringLiteral("SEQUENCE"))
        readSequence(type);
    else if (name == QStringLiteral("SEQUENCE_OF"))
        readSequenceOf(type);
    else if (name == QStringLiteral("CHOICE"))
        readChoice(type);
    else if (name == QStringLiteral("REFERENCE_TYPE"))
        readReferenceType(type);
    else if (name == QStringLiteral("INTEGER"))
        readInteger(type);
    else if (name == QStringLiteral("REAL"))
        readReal(type);
    else if (name == QStringLiteral("ENUMERATED"))
        readEnumerated(type);
    else if (name == QStringLiteral("OCTET_STRING"))
        readOctetString(type);
    else if (name == QStringLiteral("IA5String"))
        readIA5String(type);
    else if (name == QStringLiteral("NumericString"))
        readNumericString(type);
    else if (name == QStringLiteral("BIT_STRING"))
        readBitString(type);
    else
        m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readSequence(Data::Types::Type &type)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("SEQUENCE_COMPONENT"))
            readSequenceComponent(type);
        else if (m_xmlReader.name() == QStringLiteral("ACN_COMPONENT"))
            readAcnComponent(type);
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readSequenceComponent(Data::Types::Type &type)
{
    auto attributes = m_xmlReader.attributes();
    auto childType = findAndReadType();

    ChildItemAddingVisitor visitor(attributes, m_currentFile, std::move(childType));
    type.accept(visitor);
    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readAcnComponent(Data::Types::Type &type)
{
    auto alignToNext = readIsAlignedToNext();
    auto name = readNameAttribute();
    auto id = readIdAttribute(QStringLiteral("Id"));
    auto component = readTypeDetails(readTypeAttribute(), {}, false, alignToNext);

    AcnDefinedItemsAddingVisitor visitor(
        std::make_unique<Data::AcnSequenceComponent>(id, name, std::move(component)));
    type.accept(visitor);
}

void AstXmlParser::readSequenceOf(Data::Types::Type &type)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("Constraints")) {
            readExpressionTree(type, QStringLiteral("IntegerValue"));
        } else if (m_xmlReader.name() == QStringLiteral("Asn1Type")) {
            auto attributes = m_xmlReader.attributes();
            auto childType = readType();

            ChildItemAddingVisitor visitor(attributes, m_currentFile, std::move(childType));
            type.accept(visitor);
        } else {
            m_xmlReader.skipCurrentElement();
        }
    }
}

void AstXmlParser::readChoice(Data::Types::Type &type)
{
    while (skipToChildElement(QStringLiteral("CHOICE_ALTERNATIVE"))) {
        auto attributes = m_xmlReader.attributes();
        auto childType = findAndReadType();

        ChildItemAddingVisitor visitor(attributes, m_currentFile, std::move(childType));
        type.accept(visitor);

        m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readInteger(Data::Types::Type &type)
{
    readConstraints(type, QStringLiteral("IntegerValue"));
}

void AstXmlParser::readReal(Data::Types::Type &type)
{
    readConstraints(type, QStringLiteral("RealValue"));
}

void AstXmlParser::readEnumerated(Data::Types::Type &type)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("Items"))
            readEnumeratedItem(type);
        else if (m_xmlReader.name() == QStringLiteral("Constraints"))
            readExpressionTree(type, QStringLiteral("EnumValue"));
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readOctetString(Data::Types::Type &type)
{
    readConstraints(type, QStringLiteral("OctetStringValue"));
}

void AstXmlParser::readIA5String(Data::Types::Type &type)
{
    readConstraints(type, QStringLiteral("StringValue"));
}

void AstXmlParser::readNumericString(Data::Types::Type &type)
{
    readConstraints(type, QStringLiteral("StringValue"));
}

void AstXmlParser::readBitString(Data::Types::Type &type)
{
    readConstraints(type, QStringLiteral("BitStringValue"));
}

void AstXmlParser::readEnumeratedItem(Data::Types::Type &type)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("Item")) {
            ChildItemAddingVisitor visitor(m_xmlReader.attributes(), m_currentFile);
            type.accept(visitor);
        }

        m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readReferenceType(Data::Types::Type &type)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("AcnArguments"))
            readAcnArguments(type);
        else if (m_xmlReader.name() == QStringLiteral("Asn1Type"))
            readReferredTypeDetails(type);
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readConstraints(Data::Types::Type &type, const ConstraintType &constraintName)
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("Constraints"))
            readExpressionTree(type, constraintName);
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readExpressionTree(Data::Types::Type &type, const ConstraintType &constraintName)
{
    while (m_xmlReader.readNextStartElement()) {
        const auto &name = m_xmlReader.name().toString();
        if (name == QStringLiteral("SIZE")) {
            tryAddOperator(type, name);
            readAlphanumericalConstraints(type,
                                          QStringLiteral("IntegerValue"),
                                          QStringLiteral("SIZE"));
        } else if (name == QStringLiteral("ALPHA")) {
            tryAddOperator(type, name);
            readAlphanumericalConstraints(type,
                                          QStringLiteral("StringValue"),
                                          QStringLiteral("ALPHA"));
        } else if (name == QStringLiteral("OR") || name == QStringLiteral("AND")) {
            tryAddOperator(type, name);
            readExpressionTree(type, constraintName);
        } else if (name == QStringLiteral("Range")) {
            readRange(type, constraintName);
        } else if (constraintName == name) {
            Constraint val{m_xmlReader.readElementText(), name};
            RangeConstraintAssigningVisitor visitor(m_xmlReader, val, val);
            type.accept(visitor);
        } else {
            m_xmlReader.skipCurrentElement();
        }
    }
}

void AstXmlParser::readAlphanumericalConstraints(Data::Types::Type &type,
                                                 const ConstraintType &constraintName,
                                                 const QString &tagName)
{
    do {
        readAlphanumericalConstraint(type, constraintName);
    } while (m_xmlReader.isEndElement() && m_xmlReader.name() != tagName);
}

void AstXmlParser::readAlphanumericalConstraint(Data::Types::Type &type,
                                                const ConstraintType &constraintName)
{
    while (m_xmlReader.readNextStartElement()) {
        const auto &name = m_xmlReader.name().toString();
        if (name == QStringLiteral("OR") || name == QStringLiteral("AND")) {
            tryAddOperator(type, name);
        } else if (name == QStringLiteral("Range")) {
            readRange(type, constraintName);
        } else if (name == constraintName) {
            Constraint val{m_xmlReader.readElementText(), name};
            RangeConstraintAssigningVisitor visitor(m_xmlReader, val, val);
            type.accept(visitor);
        } else {
            m_xmlReader.skipCurrentElement();
        }
    }
}

void AstXmlParser::tryAddOperator(Data::Types::Type &type, const QString &operatorName)
{
    OperatorAssigningVisitor visitor(operatorName);
    type.accept(visitor);
}

void AstXmlParser::readRange(Data::Types::Type &type, const ConstraintType &constraintName)
{
    Constraint min, max;
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("Min"))
            min = readValue(constraintName);
        else if (m_xmlReader.name() == QStringLiteral("Max"))
            max = readValue(constraintName);
        else
            m_xmlReader.skipCurrentElement();
    }

    RangeConstraintAssigningVisitor visitor(m_xmlReader, min, max);
    type.accept(visitor);
}

AstXmlParser::Constraint AstXmlParser::readValue(const ConstraintType &constraintName)
{
    Constraint val;
    while (m_xmlReader.readNextStartElement()) {
        const auto &name = m_xmlReader.name().toString();
        if (constraintName == name)
            val = {m_xmlReader.readElementText(), name};
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
