/****************************************************************************
**
** Copyright (C) 2018 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN MalTester - Tool for generating test cases
** based on ASN.1/ACN models and simulating malformed or malicious data.
**
** Tool was developed under a m_processogramme and funded by
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
** along with this m_processogram.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "astfilegenerator.h"

#include <QtDebug>

using namespace MalTester;

AstFileGenerator::AstFileGenerator(const RunParameters &params, const QString &outputPath)
    : m_params(params)
    , m_outputPath(outputPath)
    , m_process(nullptr)
{}

AstFileGenerator::~AstFileGenerator()
{
    m_process->close();
}

AstFileGenerator::Result AstFileGenerator::generate()
{
    m_process.reset(createProcess());
    m_process->start();
    if (!m_process->waitForStarted())
        return handleNotStarted();
    return m_process->waitForFinished() ? processFinished() : handleTimeout();
}

QProcess *AstFileGenerator::createProcess() const
{
    auto pr = new QProcess;

    pr->setProgram(m_params.m_asn1SccPath);
    pr->setArguments(createRunArgs());
    pr->setProcessChannelMode(QProcess::MergedChannels);

    return pr;
}

QStringList AstFileGenerator::createRunArgs() const
{
    return QStringList() << m_params.m_asn1SccFlags << astArg() << inputFilesArg();
}

QStringList AstFileGenerator::astArg() const
{
    return QStringList() << QLatin1String("--xml-ast") << m_outputPath;
}

QStringList AstFileGenerator::inputFilesArg() const
{
    return m_params.m_inputFiles;
}

AstFileGenerator::Result AstFileGenerator::processFinished() const
{
    switch (m_process->exitStatus()) {
    case QProcess::NormalExit:
        return handleNormalExit();
    case QProcess::CrashExit:
        return handleCrashExit();
    default:
        return Result::Unknown;
    }
}

AstFileGenerator::Result AstFileGenerator::handleNormalExit() const
{
    if (m_process->exitCode() != 0) {
        writeMessage("failed");
        return Result::BuildFailed;
    }

    return Result::BuildSuccess;
}

AstFileGenerator::Result AstFileGenerator::handleCrashExit() const
{
    writeMessage("crashed");
    return Result::ProcessCrashed;
}

AstFileGenerator::Result AstFileGenerator::handleNotStarted() const
{
    writeMessage("failed to start");
    return Result::ProcessCrashed;
}

AstFileGenerator::Result AstFileGenerator::handleTimeout() const
{
    writeMessage("timed out");
    return Result::ProcessTimeouted;
}

void AstFileGenerator::writeMessage(const QString &message) const
{
    qCritical().noquote().nospace() << "ERROR: asn1scc call " << message;

    const auto processMsg = m_process->readAll();
    if (!processMsg.isEmpty())
        qCritical().noquote() << processMsg.trimmed() << endl;
}
