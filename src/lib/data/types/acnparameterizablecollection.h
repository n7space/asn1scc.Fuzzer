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

#include <QString>

#include <data/acnparameter.h>

namespace MalTester {
namespace Data {
namespace Types {

template<typename T>
class AcnParametrizableCollection
{
public:
    AcnParametrizableCollection() = default;
    AcnParametrizableCollection(const AcnParametrizableCollection<T> &other);

    using Components = std::map<QString, T>;

    const Components &components() const { return m_components; }
    void addComponent(const QString &key, const T &component);

    const AcnParameterPtrs &acnParameters() const { return m_parameters; }
    void addParameter(AcnParameterPtr parameter);

protected:
    Components m_components;
    AcnParameterPtrs m_parameters;
};

template<typename T>
inline AcnParametrizableCollection<T>::AcnParametrizableCollection(
    const AcnParametrizableCollection<T> &other)
{
    for (const auto &parameter : other.m_parameters)
        addParameter(std::make_unique<AcnParameter>(*parameter));
}

template<typename T>
inline void AcnParametrizableCollection<T>::addComponent(const QString &key, const T &component)
{
    m_components.insert(std::pair<QString, T>(key, component));
}

template<typename T>
inline void AcnParametrizableCollection<T>::addParameter(AcnParameterPtr parameter)
{
    m_parameters.push_back(std::move(parameter));
}

} // namespace Types
} // namespace Data
} // namespace MalTester
