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

#include <QObject>

#include <data/definitions.h>
#include <data/file.h>
#include <data/project.h>

namespace MalTester {
namespace Tests {

class ReconstructorTests : public QObject
{
    Q_OBJECT
public:
    explicit ReconstructorTests(QObject *parent = 0);

private slots:
    void test_emptyProject();

    void test_emptyFile();
    void test_singleFileWithSingleModule();
    void test_singleFileWithMultipleModules();
    void test_multipleFiles();

private:
    std::unique_ptr<Data::File> createFile(const QString &path) const;
    std::unique_ptr<Data::Project> createProject(const QString &name) const;
    std::unique_ptr<Data::Definitions> createDefinitions(const QString &name) const;
};

} // namespace Tests
} // namespace MalTester
