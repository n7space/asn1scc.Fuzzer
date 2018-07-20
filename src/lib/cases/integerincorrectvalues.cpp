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
#include "integerincorrectvalues.h"

#include <algorithm>

#include <data/constraints/rangelistingvisitor.h>

#include "integerranges.h"

using namespace MalTester::Cases;
using namespace MalTester::Data;

namespace {
RangeList<int> definedRangesFor(const Types::Integer &integer)
{
    Constraints::RangeListingVisitor<IntegerValue> visitor;
    integer.constraints().accept(visitor);
    return visitor.result();
}
} // namespace

IntegerIncorrectValues::IntegerIncorrectValues(const Types::Integer &integer)
{
    const auto definedRanges = definedRangesFor(integer);
    if (definedRanges.isEmpty())
        return;
    const auto maliciousRanges = difference(maxValueRangeFor(integer), definedRanges);

    for (const auto r : maliciousRanges) {
        m_items.append(r.begin());
        if (!r.isSingleItem()) {
            m_items.append(r.end());
            m_items.append((r.begin() + r.end()) / 2);
        }
    }

    makeItemsUnique();
}

void IntegerIncorrectValues::makeItemsUnique()
{
    std::sort(m_items.begin(), m_items.end());
    m_items.erase(std::unique(m_items.begin(), m_items.end()), m_items.end());
}
