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

#include <QString>

using namespace MalTester;

InputParametersParser::InputParametersParser() // clang-format off
    : m_mainStructure({"m", "main-structure"}, "Main structure for generation of malformed messages.", "main structure")
    , m_asn1sccPath  ({"a", "asn1scc-path"},   "Asn1scc compiler path.",                               "asn1scc path", "asn1.exe")
    , m_outputDir    ({"o", "output-dir"},     "Output directory for generated files.",                "output directory")
    , m_wrapAsCcsds  ({"w", "wrap-as-ccsds"},  "Wrapping main structure inside CCSDS packets",         "[tc, tm]") // clang-format on
{
    setupParser();
}

void InputParametersParser::parse(int argc, char *argv[])
{
    if (argc <= 1)
        printUsageAndExit("Invalid arguments number: " + QString::number(argc));

    QStringList args;
    for (int i = 0; i < argc; i++)
        args << argv[i];

    auto help = m_parser.addHelpOption();
    if (!m_parser.parse(args) || m_parser.isSet(help))
        printUsageAndExit(m_parser.errorText());

    updateRunParams();
}

RunParameters InputParametersParser::parameters() const
{
    return m_params;
}

void InputParametersParser::setupParser()
{
    m_parser.addOption(m_asn1sccPath);
    m_parser.addOption(m_outputDir);
    m_parser.addOption(m_mainStructure);
    m_parser.addOption(m_wrapAsCcsds);
    m_parser.addPositionalArgument("<files>", "List of files to be passed to asn1scc.");
}

void InputParametersParser::updateRunParams()
{
    m_params.m_inputFiles = readFilesList();
    m_params.m_mainStructureName = readMainStructureName();
    m_params.m_asn1SccCommand = readAsn1SccPath();
    m_params.m_outputDir = readOutputDir();
    m_params.m_ccsdsWrap = readCcsdsValue();
}

QStringList InputParametersParser::readFilesList()
{
    const auto args = m_parser.positionalArguments();

    if (args.empty())
        printUsageAndExit("No files provided");

    return args;
}

QString InputParametersParser::readMainStructureName()
{
    if (!m_parser.isSet(m_mainStructure))
        printUsageAndExit("Main structure name must be set");

    return m_parser.value(m_mainStructure);
}

QString InputParametersParser::readAsn1SccPath()
{
    return m_parser.value(m_asn1sccPath);
}

QString InputParametersParser::readOutputDir()
{
    return m_parser.value(m_outputDir);
}

RunParameters::CcsdsWrap InputParametersParser::readCcsdsValue()
{
    RunParameters::CcsdsWrap val = RunParameters::CcsdsWrap::none;
    if (!m_parser.isSet(m_wrapAsCcsds))
        return val;

    const auto ccsds = m_parser.value(m_wrapAsCcsds);
    if (ccsds == "tc")
        val = RunParameters::CcsdsWrap::tc;
    else if (ccsds == "tm")
        val = RunParameters::CcsdsWrap::tm;
    else
        printUsageAndExit("Unrecognised ccsds option: " + ccsds);

    return val;
}

void InputParametersParser::printUsageAndExit(const QString &message)
{
    if (!message.isEmpty()) {
        fputs("\n", stderr);
        fputs(qPrintable(message), stderr);
        fputs("\n\n", stderr);
    }

    fputs(qPrintable(m_parser.helpText()), stderr);

    exit(1);
}
