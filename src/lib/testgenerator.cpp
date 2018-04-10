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

#include "testgenerator.h"

#include <astfilegenerator.h>
#include <astfileprocessor.h>

using namespace MalTester;

TestGenerator::TestGenerator(const RunParameters &params)
    : m_params(params)
{}

void TestGenerator::run() const
{
    auto ast = createDataTree();
    if (ast == nullptr)
        return;
}

std::unique_ptr<Data::Project> TestGenerator::createDataTree() const
{
    const QTemporaryDir dir;
    if (!dir.isValid())
        return nullptr;

    const QString outPath = dir.filePath(QLatin1String("ast.xml"));

    AstFileGenerator astGen(m_params, outPath);
    if (astGen.generate() != AstFileGenerator::State::BuildSuccess)
        return nullptr;

    AstFileProcessor astProc(outPath);
    return astProc.process();
}
