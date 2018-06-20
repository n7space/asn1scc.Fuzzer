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

#include <QString>

namespace MalTester {
namespace Data {
namespace Constraints {

template<typename T>
class Range
{
public:
    explicit Range(const T &singleItem)
        : Range(singleItem, singleItem)
    {}
    Range(const T &begin, const T &end)
        : m_begin(qMin(begin, end))
        , m_end(qMax(begin, end))
    {}
    Range(const Range &other) = default;
    Range &operator=(const Range &other) = default;

    bool isSingleItem() const { return begin() == end(); }
    const T &begin() const { return m_begin; }
    const T &end() const { return m_end; }

    QString asString() const;

    bool intersects(const Range &other) const;
    Range intersection(const Range &other) const;

    bool canMerge(const Range &other) const;
    Range merge(const Range &other) const;

private:
    T m_begin;
    T m_end;
};

template<typename T>
inline bool operator==(const Range<T> &a, const Range<T> &b)
{
    return a.begin() == b.begin() && a.end() == b.end();
}

template<typename T>
inline bool operator!=(const Range<T> &a, const Range<T> &b)
{
    return !(a == b);
}

template<typename T>
QString Range<T>::asString() const
{
    if (isSingleItem())
        return QString::number(begin());
    return QString::number(begin()) + " .. " + QString::number(end());
}

template<>
inline QString Range<QString>::asString() const
{
    if (isSingleItem())
        return '"' + begin() + '"';
    return '"' + begin() + '"' + " .. " + '"' + end() + '"';
}

template<typename T>
bool Range<T>::intersects(const Range &other) const
{
    if (other.begin() < begin())
        return other.intersects(*this);
    return end() >= other.begin();
}

template<typename T>
Range<T> Range<T>::merge(const Range &other) const
{
    Q_ASSERT(canMerge(other));

    return {qMin(begin(), other.begin()), qMax(end(), other.end())};
}

template<typename T>
bool Range<T>::canMerge(const Range &other) const
{
    return intersects(other);
}

template<>
inline bool Range<int>::canMerge(const Range &other) const
{
    if (intersects(other))
        return true;
    return qAbs(begin() - other.begin()) == 1 || qAbs(end() - other.end()) == 1
           || qAbs(begin() - other.end()) == 1 || qAbs(end() - other.begin()) == 1;
}

template<typename T>
Range<T> Range<T>::intersection(const Range &other) const
{
    Q_ASSERT(intersects(other));

    return {qMax(begin(), other.begin()), qMin(end(), other.end())};
}

} // namespace Constraints
} // namespace Data
} // namespace MalTester
