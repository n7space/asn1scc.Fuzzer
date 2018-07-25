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
#include "inputparametersparser.h"

#include <QFileInfo>
#include <QString>
#include <QtDebug>

using namespace MalTester;

InputParametersParser::InputParametersParser()
    // clang-format off
    : m_mainStructure({"m", "main-structure"}, "Main structure for generation of malformed messages.", "main structure")
    , m_asn1sccPath  ({"a", "asn1scc-path"},   "Asn1scc compiler path.",                               "asn1scc path", "asn1.exe")
    , m_asn1sccFlags ({"f", "asn1scc-flags"},  "Asn1scc compiler flags.",                              "asn1scc flags", "--field-prefix AUTO --type-prefix T --acn-enc")
    , m_outputDir    ({"o", "output-dir"},     "Output directory for generated files.",                "output directory", "out")
    , m_wrapAsCcsds  ({"w", "wrap-as-ccsds"},  "Wrapping main structure inside CCSDS packets",         "[tc, tm]")
// clang-format on
{
    setupParser();
}

void InputParametersParser::process(const QStringList &args)
{
    m_parser.process(args);

    updateRunParams();
}

RunParameters InputParametersParser::parameters() const
{
    return m_params;
}

void InputParametersParser::setupParser()
{
    m_parser.addHelpOption();
    m_parser.addVersionOption();

    m_parser.addOption(m_asn1sccPath);
    m_parser.addOption(m_asn1sccFlags);
    m_parser.addOption(m_outputDir);
    m_parser.addOption(m_mainStructure);
    m_parser.addOption(m_wrapAsCcsds);
    m_parser.addPositionalArgument("<files>", "List of files to be passed to asn1scc.");
}

void InputParametersParser::updateRunParams()
{
    m_params.m_inputFiles = readFilesList();
    m_params.m_mainStructure = readMainStructure();
    m_params.m_asn1SccPath = readAsn1SccPath();
    m_params.m_asn1SccFlags = readAsn1SccFlags();
    m_params.m_outputDir = readOutputDir();
    m_params.m_ccsdsWrap = readCcsdsValue();
}

QStringList InputParametersParser::readFilesList()
{
    const auto args = m_parser.positionalArguments();

    if (args.empty())
        printUsageAndFail("No files provided");

    return args;
}

Data::TypeReference InputParametersParser::readMainStructure()
{
    if (!m_parser.isSet(m_mainStructure))
        printUsageAndFail("Main structure name must be set");

    const auto pair = m_parser.value(m_mainStructure).split('.', QString::SkipEmptyParts);
    if (pair.count() != 2 && pair.count() != 1) {
        printUsageAndFail("Main structure must be provided in format Module.Type or UniqueTypeName");
        return {};
    }

    if (pair.count() == 2)
        return {pair[1], pair[0]};
    return {pair[0], {}};
}

QString InputParametersParser::readAsn1SccPath()
{
    return m_parser.value(m_asn1sccPath);
}

QStringList InputParametersParser::readAsn1SccFlags()
{
    return m_parser.value(m_asn1sccFlags).split(QRegExp("\\s+"), QString::SkipEmptyParts);
}

QString InputParametersParser::readOutputDir()
{
    return m_parser.value(m_outputDir);
}

RunParameters::CcsdsWrap InputParametersParser::readCcsdsValue()
{
    if (!m_parser.isSet(m_wrapAsCcsds))
        return RunParameters::CcsdsWrap::none;

    const auto ccsds = m_parser.value(m_wrapAsCcsds);
    if (ccsds == "tc")
        return RunParameters::CcsdsWrap::tc;
    else if (ccsds == "tm")
        return RunParameters::CcsdsWrap::tm;

    printUsageAndFail("Unrecognised ccsds option: " + ccsds);
    return RunParameters::CcsdsWrap::none;
}

void InputParametersParser::printUsageAndFail(const QString &message)
{
    qCritical().noquote() << "ERROR:" << message;
    exit(1);
}
