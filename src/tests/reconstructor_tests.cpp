/****************************************************************************
**
** Copyright (C) 2018 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Fuzzer - Tool for generating test cases
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

#include "reconstructor_tests.h"

#include <QtTest>

using namespace Fuzzer;
using namespace Fuzzer::Tests;

ReconstructorTests::ReconstructorTests(QObject *parent)
    : QObject(parent)
{}

void ReconstructorTests::test_emptyProject()
{
    auto project = createProject(QStringLiteral("MyProject"));

    Reconstructor rec(createFactory());
    rec.reconstruct(*project);

    QVERIFY(m_files.empty());
}

void ReconstructorTests::test_emptyFile()
{
    auto project = createProject(QStringLiteral("MyProject"));
    project->add(createFile(QStringLiteral("MyFile_1.asn")));

    Reconstructor rec(createFactory());
    rec.reconstruct(*project);

    QCOMPARE(m_files.size(), std::size_t{2});
    QCOMPARE(m_files.at(QStringLiteral("MyFile_1.asn")), QStringLiteral(""));
    QCOMPARE(m_files.at(QStringLiteral("MyFile_1.acn")), QStringLiteral(""));
}

void ReconstructorTests::test_singleFileWithSingleModule()
{
    auto project = createProject(QStringLiteral("MyProject"));
    auto file = createFile(QStringLiteral("MyFile_1.asn1"));

    file->add(createDefinitions(QStringLiteral("MyModule_1")));
    project->add(std::move(file));

    Reconstructor rec(createFactory());
    rec.reconstruct(*project);

    QCOMPARE(m_files.size(), std::size_t{2});

    const QString expected = "MyModule_1 DEFINITIONS ::= BEGIN\n"
                             "END\n"
                             "\n";

    QCOMPARE(m_files.at(QStringLiteral("MyFile_1.asn1")), expected);
    QCOMPARE(m_files.at(QStringLiteral("MyFile_1.acn")), expected);
}

void ReconstructorTests::test_singleFileWithMultipleModules()
{
    auto project = createProject(QStringLiteral("MyProject"));
    auto file = createFile(QStringLiteral("MyFile_1"));

    file->add(createDefinitions(QStringLiteral("MyModule_1")));
    file->add(createDefinitions(QStringLiteral("MyModule_2")));
    project->add(std::move(file));

    Reconstructor rec(createFactory());
    rec.reconstruct(*project);

    QCOMPARE(m_files.size(), std::size_t{2});

    const QString expected = "MyModule_1 DEFINITIONS ::= BEGIN\n"
                             "END\n"
                             "\n"
                             "MyModule_2 DEFINITIONS ::= BEGIN\n"
                             "END\n"
                             "\n";

    QCOMPARE(m_files.at(QStringLiteral("MyFile_1")), expected);
    QCOMPARE(m_files.at(QStringLiteral("MyFile_1.acn")), expected);
}

void ReconstructorTests::test_multipleFiles()
{
    auto project = createProject(QStringLiteral("MyProject"));
    project->add(createFile(QStringLiteral("MyFile_1")));
    project->add(createFile(QStringLiteral("MyFile_2")));

    Reconstructor rec(createFactory());
    rec.reconstruct(*project);

    QCOMPARE(m_files.size(), std::size_t{4});
}

std::unique_ptr<Data::File> ReconstructorTests::createFile(const QString &path) const
{
    return std::make_unique<Data::File>(path);
}

std::unique_ptr<Data::Project> ReconstructorTests::createProject(const QString &name) const
{
    return std::make_unique<Data::Project>(name);
}

std::unique_ptr<Data::Definitions> ReconstructorTests::createDefinitions(const QString &name) const
{
    return std::make_unique<Data::Definitions>(name, Data::SourceLocation());
}

Reconstructor::StreamFactory ReconstructorTests::createFactory()
{
    return [this](const QString &name) { return std::make_unique<QTextStream>(&m_files[name]); };
}

void ReconstructorTests::init()
{
    m_files.clear();
}
