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

#include <algorithm>
#include <initializer_list>

#include <QList>
#include <QString>

#include "range.h"

namespace MalTester {
namespace Data {

template<typename T>
class RangeList
{
public:
    RangeList() = default;
    RangeList(std::initializer_list<Range<T>> ranges)
        : m_ranges(ranges)
    {}

    using const_iterator = typename QList<Range<T>>::const_iterator;

    bool isEmpty() const { return m_ranges.isEmpty(); }
    const_iterator begin() const { return m_ranges.cbegin(); }
    const_iterator end() const { return m_ranges.cend(); }

    QString asString() const;

    void sort();
    void compact();

    void merge(const RangeList &other) { m_ranges.append(other.m_ranges); }
    void intersect(const RangeList &other);
    RangeList intersection(const Range<T> &range) const;

private:
    QList<Range<T>> m_ranges;
};

template<typename T>
QString RangeList<T>::asString() const
{
    if (isEmpty())
        return {};
    const auto rangeStr = [](const Range<T> &r) {
        return r.isSingleItem() ? QString("%1").arg(r.begin())
                                : QString("%1 .. %2").arg(r.begin()).arg(r.end());
    };
    return std::accumulate(begin() + 1,
                           end(),
                           rangeStr(m_ranges.first()),
                           [rangeStr](const QString &a, const Range<T> &b) {
                               return a + " | " + rangeStr(b);
                           });
}

template<typename T>
void RangeList<T>::compact()
{
    if (isEmpty())
        return;

    sort();

    decltype(m_ranges) list;

    list.append(m_ranges.first());
    std::for_each(begin() + 1, end(), [&list](const Range<T> &r) {
        if (list.last().canMerge(r))
            list.last() = list.last().merge(r);
        else
            list.append(r);
    });

    m_ranges = list;
}

template<typename T>
void RangeList<T>::sort()
{
    std::sort(m_ranges.begin(), m_ranges.end(), [](const Range<T> &a, const Range<T> &b) {
        return a.begin() < b.begin();
    });
}

template<typename T>
RangeList<T> RangeList<T>::intersection(const Range<T> &range) const
{
    RangeList<T> list;
    for (const auto &r : *this)
        if (r.intersects(range))
            list.m_ranges.append(r.intersection(range));
    return list;
}

template<typename T>
void RangeList<T>::intersect(const RangeList &other)
{
    decltype(m_ranges) list;

    for (const auto &r : other)
        list.append(intersection(r).m_ranges);

    m_ranges = list;
}

} // namespace Data
} // namespace MalTester
