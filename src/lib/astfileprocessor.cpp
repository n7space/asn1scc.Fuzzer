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

#include "astfileprocessor.h"

#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>

#include <astxmlparser.h>

using namespace MalTester;

AstFileProcessor::AstFileProcessor(const QString &astPath)
    : m_astPath(astPath)
{}

static std::unique_ptr<Data::Project> createProject(AstXmlParser &parser)
{
    auto project = std::make_unique<Data::Project>("project");

    for (auto &item : parser.takeData())
        project->add(std::move(item.second));

    return project;
}

std::unique_ptr<Data::Project> AstFileProcessor::process() const
{
    auto fileContent = readAstFileContent();
    if (fileContent.isEmpty())
        return nullptr;

    QXmlStreamReader streamReader;
    streamReader.addData(fileContent);

    AstXmlParser parser(streamReader);
    if (!parser.parse())
        return nullptr;

    return createProject(parser);
}

QString AstFileProcessor::readAstFileContent() const
{
    QFile f(m_astPath);
    if (!f.open(QFile::ReadOnly | QFile::Text))
        return {};

    QTextStream ts(&f);
    return ts.readAll();
}
