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

#include <map>
#include <memory>
#include <vector>

#include "definitions.h"
#include "errormessage.h"
#include "node.h"
#include "typereference.h"

namespace MalTester {
namespace Data {

class File : public Node
{
public:
    explicit File(const QString &path);
    File(const File &other);
    ~File() override;

    void accept(Visitor &visitor) const override;
    void accept(MutatingVisitor &visitor) override;

    void add(std::unique_ptr<Definitions> defs);
    void addTypeReference(std::unique_ptr<TypeReference> ref);
    void addErrorMessage(const ErrorMessage &message);

    using DefinitionsList = std::vector<std::unique_ptr<Definitions>>;
    using References = std::vector<std::unique_ptr<TypeReference>>;
    using ReferencesMap = std::multimap<int, TypeReference *>;
    using ErrorList = std::vector<ErrorMessage>;

    const DefinitionsList &definitionsList() const { return m_definitionsList; }
    const Definitions *definitions(const QString &name) const;

    const ReferencesMap &referencesMap() const { return m_referencesMap; }
    const References &references() const { return m_references; }
    const ErrorList &errors() const { return m_errorList; }

    void setPolluted() { m_polluted = true; }
    bool isPolluted() const { return m_polluted; }

    void clearReferences();
    void clearErrors();

private:
    DefinitionsList m_definitionsList;
    ReferencesMap m_referencesMap;
    References m_references;
    ErrorList m_errorList;

    std::map<QString, Definitions *> m_definitionsByNameMap;

    bool m_polluted;
};

} // namespace Data
} // namespace MalTester
