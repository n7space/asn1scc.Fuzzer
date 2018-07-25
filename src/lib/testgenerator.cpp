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
#include <cases/testcasebuilder.h>
#include <cases/testcaseprinter.h>
#include <cases/testcasesink.h>

#include <astfilegenerator.h>
#include <astfileprocessor.h>
#include <reconstructor.h>

using namespace MalTester;

TestGenerator::TestGenerator(const RunParameters &params)
    : m_params(params)
{}

bool TestGenerator::reportOnNotFoundStructure() const
{
    qCritical() << "Type" << m_params.m_mainStructure.name() << "from module:"
                << (m_params.m_mainStructure.module().isEmpty() ? QStringLiteral("*any*")
                                                                : m_params.m_mainStructure.module())
                << "not found."
                << "No cases were generated.";
    return false;
}

static bool reportOnNoCasesFound()
{
    qCritical() << "No cases were found for structure.";
    return false;
}

bool TestGenerator::run() const
{
    const auto ast = createDataTree();
    if (ast == nullptr)
        return false;
    return createOutputDirectory() && dumpRelaxedModelFrom(*ast) && dumpCases(buildCasesFor(*ast));
}

bool TestGenerator::createOutputDirectory() const
{
    if (QDir().mkpath(m_params.m_outputDir))
        return true;

    qCritical() << "Unable to create output directory: " << m_params.m_outputDir;
    return false;
}

std::unique_ptr<Cases::TestCaseSink> TestGenerator::buildCasesFor(const Data::Project &project) const
{
    Cases::TestCaseBuilder builder(m_params.m_mainStructure);
    project.accept(builder);
    return builder.takeResult();
}

bool TestGenerator::dumpCases(std::unique_ptr<Cases::TestCaseSink> cases) const
{
    if (cases == nullptr)
        return reportOnNotFoundStructure();
    if (cases->cases().isEmpty())
        return reportOnNoCasesFound();
    return dumpTestCases(*cases);
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
std::unique_ptr<QTextStream> openFile(QFile &file)
{
    if (!file.isOpen() && !file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qCritical() << "Unable to write to:" << file.fileName();
        return nullptr;
    }
    return std::make_unique<QTextStream>(&file);
}
} // namespace

bool TestGenerator::dumpRelaxedModelFrom(const Data::Project &project) const
{
    QFile asnFile(m_params.m_outputDir + "/AllModels.asn1");
    QFile acnFile(m_params.m_outputDir + "/AllModels.acn");
    Reconstructor r([this, &asnFile, &acnFile](const QString &name) {
        return openFile(name.endsWith(".acn") ? acnFile : asnFile);
    });
    return r.reconstruct(*createRelaxedCopyOf(project));
}

bool TestGenerator::dumpTestCases(const Cases::TestCaseSink &cases) const
{
    QFile out(m_params.m_outputDir + "/test_main.c");
    if (!out.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qCritical() << "Unable to write to: " << out.fileName();
        return false;
    }
    QTextStream stream(&out);
    Cases::TestCasePrinter printer(stream);
    printer.print(cases);
    qInfo() << "Generated" << cases.cases().count() << "cases.";
    return true;
}
