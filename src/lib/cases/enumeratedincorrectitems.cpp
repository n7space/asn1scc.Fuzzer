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
#include "enumeratedincorrectitems.h"

#include <algorithm>

#include <data/constraints/rangelistingvisitor.h>

#include "integerranges.h"

using namespace Fuzzer::Cases;
using namespace Fuzzer::Data;

namespace {
class EnumRangeListingVisitor : public Constraints::RangeListingVisitorBase<EnumValue, std::int64_t>
{
public:
    explicit EnumRangeListingVisitor(const Types::Enumerated &enumerated)
        : m_enumerated(enumerated)
    {}

    void visit(const Constraints::RangeConstraint<EnumValue> &constraint) override
    {
        m_result = {m_enumerated.mapToValue(constraint.range())};
    }

private:
    RangeList<std::int64_t> toRangeList(const Constraints::Constraint<EnumValue> &c) const override
    {
        EnumRangeListingVisitor v(m_enumerated);
        c.accept(v);
        return v.result();
    }

    const Types::Enumerated &m_enumerated;
};

RangeList<std::int64_t> itemsToRangeList(const Types::Enumerated &enumerated)
{
    RangeList<std::int64_t> result;
    for (const auto &i : enumerated.items())
        result.append(enumerated.mapToValue(i.name()));
    return result;
}

RangeList<std::int64_t> constraintsToRangeList(const Types::Enumerated &enumerated)
{
    EnumRangeListingVisitor v(enumerated);
    enumerated.constraints().accept(v);
    return v.result();
}

RangeList<std::int64_t> allowedItems(const Types::Enumerated &enumerated)
{
    auto items = itemsToRangeList(enumerated);
    const auto constraintRanges = constraintsToRangeList(enumerated);
    if (!constraintRanges.isEmpty())
        items.intersect(constraintRanges);
    return items;
}
} // namespace

EnumeratedIncorrectItems::EnumeratedIncorrectItems(const Types::Enumerated &enumerated)
{
    long index = enumerated.items().count();
    const auto items = allowedItems(enumerated);
    const auto maliciousRanges = difference(maxValueRangeFor(enumerated), items);

    for (const auto &r : maliciousRanges)
        for (long i = r.begin(); i <= r.end(); ++i)
            m_items.append({index++, "fuzzer-injected-" + QString::number(i), i});

    makeItemsUnique();
}

void EnumeratedIncorrectItems::makeItemsUnique()
{
    std::sort(m_items.begin(),
              m_items.end(),
              [](const Types::EnumeratedItem &a, const Types::EnumeratedItem &b) {
                  return a.index() < b.index();
              });
    m_items.erase(std::unique(m_items.begin(),
                              m_items.end(),
                              [](const Types::EnumeratedItem &a, const Types::EnumeratedItem &b) {
                                  return a.index() == b.index();
                              }),
                  m_items.end());
}
