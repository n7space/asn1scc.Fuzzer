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

#include <QDir>
#include <QFile>
#include <QTemporaryDir>
#include <QtDebug>

#include <cases/constraintsrelaxingvisitor.h>

#include <astfilegenerator.h>
#include <astfileprocessor.h>
#include <reconstructor.h>

using namespace MalTester;

TestGenerator::TestGenerator(const RunParameters &params)
    : m_params(params)
{}

void TestGenerator::run() const
{
    auto ast = createDataTree();
    if (ast == nullptr)
        return;

    if (!createOutputDirectory())
        return;

    dumpRelaxedModelFrom(*ast);
}

bool TestGenerator::createOutputDirectory() const
{
    if (QDir().mkpath(m_params.m_outputDir))
        return true;

    qCritical() << "Unable to create output directory: " << m_params.m_outputDir;
    return false;
}

std::unique_ptr<Data::Project> TestGenerator::createDataTree() const
{
    const QTemporaryDir dir;
    if (!dir.isValid())
        return nullptr;

    const QString outPath = dir.filePath(QLatin1String("ast.xml"));

    AstFileGenerator astGen(m_params, outPath);
    if (astGen.generate() != AstFileGenerator::Result::BuildSuccess)
        return nullptr;

    AstFileProcessor astProc(outPath);
    return astProc.process();
}

std::unique_ptr<Data::Project> TestGenerator::createRelaxedCopyOf(const Data::Project &project) const
{
    auto copy = std::make_unique<Data::Project>(project);
    Cases::ConstraintsRelaxingVisitor v;
    copy->accept(v);
    return copy;
}

namespace {
std::unique_ptr<QTextStream> openFile(QObject *fileOwner, const QString &path)
{
    auto outFile = new QFile(path, fileOwner);
    if (!outFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return nullptr;
    return std::make_unique<QTextStream>(outFile);
}
} // namespace

void TestGenerator::dumpRelaxedModelFrom(const Data::Project &project) const
{
    QObject fileOwner;
    Reconstructor r([this, &fileOwner](const QString &name) {
        return openFile(&fileOwner, m_params.m_outputDir + "/" + name);
    });
    r.reconstruct(*createRelaxedCopyOf(project));
}
