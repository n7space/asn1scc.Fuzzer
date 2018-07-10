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
#pragma once

#include <functional>

#include <QTextStream>

#include <data/project.h>

namespace MalTester {

class Reconstructor
{
public:
    using StreamFactory = std::function<std::unique_ptr<QTextStream>(const QString &name)>;
    Reconstructor(const StreamFactory &streamFactory);

    void reconstruct(const Data::Project &project);

private:
    void reconstructAsn1File(const Data::File &file);
    void reconstructAcnFile(const Data::File &file);

    StreamFactory m_streamFactory;
};

} // namespace MalTester
