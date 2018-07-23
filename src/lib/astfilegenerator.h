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

#include <QProcess>

#include <runparameters.h>

namespace MalTester {

class AstFileGenerator
{
public:
    enum class Result {
        ProcessTimeouted,
        ProcessCrashed,

        BuildFailed,
        BuildSuccess,
        Unknown
    };

    AstFileGenerator(const RunParameters &params, const QString &outPath);
    ~AstFileGenerator();

    Result generate();

private:
    QProcess *createProcess() const;

    QStringList createRunArgs() const;
    QStringList astArg() const;
    QStringList inputFilesArg() const;

    Result processFinished() const;

    Result handleTimeout() const;
    Result handleNormalExit() const;
    Result handleCrashExit() const;

    void writeMessage(const QString &message) const;

    const RunParameters &m_params;
    const QString m_outputPath;

    std::unique_ptr<QProcess> m_process;
};

} // namespace MalTester
