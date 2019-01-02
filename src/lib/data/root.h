/****************************************************************************
**
** Copyright (C) 2018-2019 N7 Space sp. z o. o.
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
#pragma once

#include <map>
#include <memory>
#include <vector>

#include <QString>

#include "node.h"

namespace Fuzzer {
namespace Data {

class Project;

class Root : public Node
{
public:
    explicit Root();
    Root(const Root &other);
    ~Root() override;

    void accept(Visitor &visitor) const override;
    void accept(MutatingVisitor &visitor) override;

    void add(std::unique_ptr<Project> project);
    void remove(const QString &name);

    using Projects = std::vector<std::unique_ptr<Project>>;
    const Projects &projects() const { return m_projects; }

    Project *project(const QString &name) const;

private:
    Projects m_projects;
    std::map<QString, Project *> m_nameToProjectMap;
};

} // namespace Data
} // namespace Fuzzer
