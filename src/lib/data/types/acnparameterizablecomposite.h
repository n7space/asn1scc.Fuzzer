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
#pragma once

#include <memory>
#include <vector>

#include <QString>

#include <data/acnparameter.h>

namespace Fuzzer {
namespace Data {
namespace Types {

template<typename T>
class AcnParameterizableCollection
{
public:
    AcnParameterizableCollection() = default;
    AcnParameterizableCollection(const AcnParameterizableCollection<T> &other);

    using Components = std::vector<std::unique_ptr<T>>;

    const Components &components() const { return m_components; }
    const T *component(const QString &name) const;
    void addComponent(std::unique_ptr<T> component);

    const AcnParameterPtrs &acnParameters() const { return m_parameters; }
    void addParameter(AcnParameterPtr parameter);

protected:
    Components m_components;
    AcnParameterPtrs m_parameters;
};

template<typename T>
inline const T *AcnParameterizableCollection<T>::component(const QString &name) const
{
    auto item = find_if(m_components.begin(), m_components.end(), [name](const auto &item) {
        return item->name() == name;
    });

    return item == m_components.end() ? nullptr : (*item).get();
}

template<typename T>
inline AcnParameterizableCollection<T>::AcnParameterizableCollection(
    const AcnParameterizableCollection<T> &other)
{
    for (const auto &parameter : other.m_parameters)
        addParameter(std::make_unique<AcnParameter>(*parameter));

    for (const auto &component : other.m_components)
        addComponent(component->clone());
}

template<typename T>
inline void AcnParameterizableCollection<T>::addComponent(std::unique_ptr<T> component)
{
    m_components.push_back(std::move(component));
}

template<typename T>
inline void AcnParameterizableCollection<T>::addParameter(AcnParameterPtr parameter)
{
    m_parameters.push_back(std::move(parameter));
}

} // namespace Types
} // namespace Data
} // namespace Fuzzer
