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

#include <QCoreApplication>
#include <QTimer>

#include <maintask.h>

void initializeApplication(QCoreApplication &app, MalTester::MainTask &task)
{
    QCoreApplication::setApplicationName("asn1scc-MalTester");
    QCoreApplication::setApplicationVersion("0.2");
    QCoreApplication::setOrganizationDomain("n7space.com");
    QCoreApplication::setOrganizationName("N7 Space sp. z o.o.");

    QObject::connect(&task,
                     &MalTester::MainTask::finished,
                     &app,
                     &QCoreApplication::exit,
                     Qt::QueuedConnection);
    QTimer::singleShot(0, &task, &MalTester::MainTask::start);
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    MalTester::MainTask task(app);

    initializeApplication(app, task);

    return app.exec();
}
