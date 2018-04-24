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
#include "astxmlparser_tests.h"

#include <QtTest>

#include <data/types/choice.h>
#include <data/types/enumerated.h>
#include <data/types/integer.h>
#include <data/types/real.h>
#include <data/types/sequence.h>
#include <data/types/sequenceof.h>

using namespace MalTester::Tests;
using namespace MalTester;

AstXmlParserTests::AstXmlParserTests(QObject *parent)
    : QObject(parent)
{}

void AstXmlParserTests::setXmlData(const QString &str)
{
    m_xmlReader.clear();
    m_xmlReader.addData(str);
}

void AstXmlParserTests::test_emptyFile()
{
    parsingFails("");
}

void AstXmlParserTests::test_badXmlRoot()
{
    parsingFails(R"(<?xml version="1.0" encoding="utf-8"?>)"
                 R"(<BadXml></BadXml>)");
}

void AstXmlParserTests::test_emptyDefinitions()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="TestFile.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(  <AcnDependencies/>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["TestFile.asn"]->definitionsList().size(), std::size_t{1});
    const auto defs = m_parsedData["TestFile.asn"]->definitions("TestDefinitions");
    QCOMPARE(defs->name(), QStringLiteral("TestDefinitions"));
    QCOMPARE(defs->location().path(), QStringLiteral("TestFile.asn"));
    QCOMPARE(defs->location().line(), 13);
    QCOMPARE(defs->location().column(), 42);

    QCOMPARE(defs->parent(), m_parsedData["TestFile.asn"].get());
}

void AstXmlParserTests::test_singleTypeAssignment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10">)"
          R"(            <Asn1Type>)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->types().size(),
             std::size_t{1});
    const auto type = m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt");
    QCOMPARE(type->name(), QStringLiteral("MyInt"));
    QCOMPARE(type->location().path(), QStringLiteral("Test2File.asn"));
    QCOMPARE(type->location().line(), 4);
    QCOMPARE(type->location().column(), 10);

    QCOMPARE(type->parent(), m_parsedData["Test2File.asn"]->definitions("TestDefinitions"));
}

void AstXmlParserTests::test_builtinTypeReference()
{
    QFETCH(QString, xmlTypeName);
    QFETCH(QString, typeName);

    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="X.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10">)"
          R"(            <Asn1Type>)"
          R"(              <)"
          + xmlTypeName
          + "/>"
            R"(            </Asn1Type>)"
            R"(          </TypeAssignment>)"
            R"(        </TypeAssignments>)"
            R"(      </Module>)"
            R"(    </Modules>)"
            R"(  </Asn1File>)"
            R"(</AstRoot>)");

    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->type("MyInt")->type()->name(), typeName);
}

void AstXmlParserTests::test_builtinTypeReference_data()
{
    QTest::addColumn<QString>("xmlTypeName");
    QTest::addColumn<QString>("typeName");

    // clang-format off
    QTest::newRow("Boolean")       << "BOOLEAN"       << "BOOLEAN";
    QTest::newRow("Null")          << "NULL"          << "NULL";
    QTest::newRow("Integer")       << "INTEGER"       << "INTEGER";
    QTest::newRow("Real")          << "REAL"          << "REAL";
    QTest::newRow("BitString")     << "BIT_STRING"    << "BIT STRING";
    QTest::newRow("OctetString")   << "OCTET_STRING"  << "OCTET STRING";
    QTest::newRow("IA5String")     << "IA5String"     << "IA5String";
    QTest::newRow("NumericString") << "NumericString" << "NumericString";
    QTest::newRow("Enumerated")    << "Enumerated"    << "ENUMERATED";
    QTest::newRow("Choice")        << "CHOICE"        << "CHOICE";
    QTest::newRow("Sequence")      << "SEQUENCE"      << "SEQUENCE";
    QTest::newRow("SequenceOf")    << "SEQUENCE_OF"   << "SEQUENCE OF";
    // clang-format on
}

void AstXmlParserTests::test_userDefinedTypeReference()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="X.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="2" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="3" CharPositionInLine="19">)"
          R"(              <REFERENCE_TYPE Module="OtherModule" TypeAssignment="OrgInt"/>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->type("MyInt")->type()->name(),
             QStringLiteral("OrgInt"));

    const auto ref = m_parsedData["X.asn"]->referencesMap().find(3);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().column(), 19);
    QCOMPARE(ref->second->location().line(), 3);

    QCOMPARE(ref->second->name(), QStringLiteral("OrgInt"));
    QCOMPARE(ref->second->module(), QStringLiteral("OtherModule"));
}

void AstXmlParserTests::test_multipleTypeAssignments()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="X.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="2" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="3" CharPositionInLine="19">)"
          R"(              <INTEGER/>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(          <TypeAssignment Name="MyBool" Line="2" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="3" CharPositionInLine="19">)"
          R"(              <BOOLEAN/>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(          <TypeAssignment Name="MyNull" Line="2" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="3" CharPositionInLine="19">)"
          R"(              <NULL/>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->type("MyInt")->type()->name(),
             QStringLiteral("INTEGER"));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->type("MyBool")->type()->name(),
             QStringLiteral("BOOLEAN"));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->type("MyNull")->type()->name(),
             QStringLiteral("NULL"));
}

void AstXmlParserTests::test_importedType()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="X.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <ImportedModules>)"
          R"(            <ImportedModule ID="OtherDefinitions">)"
          R"(                <ImportedTypes>)"
          R"(                    <ImportedType Name="Imported" />)"
          R"(                </ImportedTypes>)"
          R"(            </ImportedModule>)"
          R"(        </ImportedModules>)"
          R"(        <TypeAssignments/>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().size(), std::size_t(1));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().at(0).module(),
             QStringLiteral("OtherDefinitions"));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().at(0).name(),
             QStringLiteral("Imported"));
}

void AstXmlParserTests::test_multipleImportedType()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="X.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <ImportedModules>)"
          R"(            <ImportedModule ID="OtherDefinitions">)"
          R"(                <ImportedTypes>)"
          R"(                    <ImportedType Name="Imported1" />)"
          R"(                    <ImportedType Name="Imported2" />)"
          R"(                </ImportedTypes>)"
          R"(            </ImportedModule>)"
          R"(        </ImportedModules>)"
          R"(        <TypeAssignments/>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().size(), std::size_t(2));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().at(0).name(),
             QStringLiteral("Imported1"));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().at(1).name(),
             QStringLiteral("Imported2"));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().at(1).module(),
             QStringLiteral("OtherDefinitions"));
}

void AstXmlParserTests::test_assignmentsAreTypeReferences()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MySeq" Line="19" CharPositionInLine="4">)"
          R"(            <Asn1Type Line="3" CharPositionInLine="19">)"
          R"(              <INTEGER/>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["Test2.asn"]->referencesMap().size(), std::size_t{1});

    const auto ref = m_parsedData["Test2.asn"]->referencesMap().find(19);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().line(), 19);
    QCOMPARE(ref->second->location().column(), 4);

    QCOMPARE(ref->second->name(), QStringLiteral("MySeq"));
    QCOMPARE(ref->second->module(), QStringLiteral("TestDefinitions"));
}

void AstXmlParserTests::test_sequenceTypeAssingment()
{
    parse(
        R"(<?xml version="1.0" encoding="utf-8"?>)"
        R"(<AstRoot>)"
        R"(  <Asn1File FileName="Test2File.asn">)"
        R"(    <Modules>)"
        R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
        R"(        <TypeAssignments>)"
        R"(          <TypeAssignment Name="MySeq" Line="11" CharPositionInLine="9">)"
        R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
        R"(              <SEQUENCE>)"
        R"(                <Constraints/>)"
        R"(                <WithComponentConstraints/>)"
        R"(                <ACN_COMPONENT Id="MySequenceWithAcn.MySeq.n" Name="n" Type="INTEGER" size="8" encoding="BCD" />"
        R"(                <SEQUENCE_COMPONENT Name="a" Line="13" CharPositionInLine="3">)"
        R"(                  <Asn1Type Line="13" CharPositionInLine="8">)"
        R"(                    <INTEGER/>)"
        R"(                  </Asn1Type>)"
        R"(                </SEQUENCE_COMPONENT>)"
        R"(                <SEQUENCE_COMPONENT Name="b" Line="14" CharPositionInLine="2">)"
        R"(                  <Asn1Type Line="14" CharPositionInLine="4">)"
        R"(                    <REFERENCE_TYPE Module="Other" TypeAssignment="MyInt"/>")
        R"(                  </Asn1Type>)"
        R"(                </SEQUENCE_COMPONENT>)"
        R"(              </SEQUENCE>"
        R"(            </Asn1Type>)"
        R"(          </TypeAssignment>)"
        R"(        </TypeAssignments>)"
        R"(      </Module>)"
        R"(    </Modules>)"
        R"(  </Asn1File>)"
        R"(</AstRoot>)");

    const auto seq = m_parsedData["Test2File.asn"]->definitions("Defs")->type("MySeq");
    QCOMPARE(seq->type()->name(), QStringLiteral("SEQUENCE"));

    QCOMPARE(seq->location().line(), 11);
    QCOMPARE(seq->location().column(), 9);
    QCOMPARE(seq->location().path(), QLatin1String("Test2File.asn"));

    QCOMPARE(m_parsedData["Test2File.asn"]->referencesMap().size(), std::size_t{2});

    const auto ref = m_parsedData["Test2File.asn"]->referencesMap().find(14);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().line(), 14);
    QCOMPARE(ref->second->location().column(), 4);

    QCOMPARE(ref->second->name(), QStringLiteral("MyInt"));
    QCOMPARE(ref->second->module(), QStringLiteral("Other"));
}

void AstXmlParserTests::test_choiceTypeAssignment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyChoice" Line="11" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <CHOICE>)"
          R"(                <Constraints/>)"
          R"(                <WithComponentConstraints/>)"
          R"(                <CHOICE_ALTERNATIVE Name="a" Line="13" CharPositionInLine="3">)"
          R"(                  <Asn1Type Line="13" CharPositionInLine="5">)"
          R"(                    <INTEGER/>)"
          R"(                  </Asn1Type>)"
          R"(                </CHOICE_ALTERNATIVE>)"
          R"(                <CHOICE_ALTERNATIVE Name="b" Line="14" CharPositionInLine="2">)"
          R"(                  <Asn1Type Line="14" CharPositionInLine="4">)"
          R"(                    <REFERENCE_TYPE Module="Other" TypeAssignment="MyInt"/>")
          R"(                  </Asn1Type>)"
          R"(                </CHOICE_ALTERNATIVE>)"
          R"(              </CHOICE>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    const auto choice = m_parsedData["Test2File.asn"]->definitions("Defs")->type("MyChoice");
    QCOMPARE(choice->type()->name(), QStringLiteral("CHOICE"));

    QCOMPARE(choice->location().line(), 11);
    QCOMPARE(choice->location().column(), 9);
    QCOMPARE(choice->location().path(), QLatin1String("Test2File.asn"));

    QCOMPARE(m_parsedData["Test2File.asn"]->referencesMap().size(), std::size_t{2});

    const auto ref = m_parsedData["Test2File.asn"]->referencesMap().find(14);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().line(), 14);
    QCOMPARE(ref->second->location().column(), 4);

    QCOMPARE(ref->second->name(), QStringLiteral("MyInt"));
    QCOMPARE(ref->second->module(), QStringLiteral("Other"));
}

void AstXmlParserTests::test_sequenceOfTypeAssingment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MySeqOf" Line="11" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <SEQUENCE_OF>)"
          R"(                <Constraints/>)"
          R"(                <WithComponentConstraints/>)"
          R"(                <Asn1Type Line="13" CharPositionInLine="4">)"
          R"(                  <REFERENCE_TYPE Module="Other" TypeAssignment="MyInt"/>")
          R"(                </Asn1Type>)"
          R"(              </SEQUENCE_OF>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    const auto seq = m_parsedData["Test2File.asn"]->definitions("Defs")->type("MySeqOf");
    QCOMPARE(seq->type()->name(), QStringLiteral("SEQUENCE OF"));

    QCOMPARE(seq->location().line(), 11);
    QCOMPARE(seq->location().column(), 9);
    QCOMPARE(seq->location().path(), QLatin1String("Test2File.asn"));

    QCOMPARE(m_parsedData["Test2File.asn"]->referencesMap().size(), std::size_t{2});

    const auto ref = m_parsedData["Test2File.asn"]->referencesMap().find(13);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().line(), 13);
    QCOMPARE(ref->second->location().column(), 4);

    QCOMPARE(ref->second->name(), QStringLiteral("MyInt"));
    QCOMPARE(ref->second->module(), QStringLiteral("Other"));
}

void AstXmlParserTests::test_valueAssignment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <ValueAssignments>)"
          R"(          <ValueAssignment Name="asnConstant" Line="2" CharPositionInLine="0">)"
          R"(            <Asn1Type Line="2" CharPositionInLine="6">)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </ValueAssignment>)"
          R"(        </ValueAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->values().size(), std::size_t{1});
    const auto var = m_parsedData["Test2File.asn"]->definitions("Defs")->value("asnConstant");
    QCOMPARE(var->name(), QStringLiteral("asnConstant"));
    QCOMPARE(var->location().line(), 2);
    QCOMPARE(var->type()->name(), QStringLiteral("INTEGER"));
}

void AstXmlParserTests::test_importedValues()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <ImportedModules>)"
          R"(            <ImportedModule ID="OtherDefinitions">)"
          R"(                <ImportedValues>)"
          R"(                    <ImportedValue Name="Imported" />)"
          R"(                </ImportedValues>)"
          R"(            </ImportedModule>)"
          R"(        </ImportedModules>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().size(),
             std::size_t(1));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().at(0).module(),
             QStringLiteral("OtherDefinitions"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().at(0).name(),
             QStringLiteral("Imported"));
}

void AstXmlParserTests::test_multipleImportedValues()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <ImportedModules>)"
          R"(            <ImportedModule ID="OtherDefinitions">)"
          R"(                <ImportedValues>)"
          R"(                    <ImportedValue Name="Imported" />)"
          R"(                    <ImportedValue Name="SecondImported" />)"
          R"(                </ImportedValues>)"
          R"(            </ImportedModule>)"
          R"(        </ImportedModules>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().size(),
             std::size_t(2));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().at(0).module(),
             QStringLiteral("OtherDefinitions"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().at(0).name(),
             QStringLiteral("Imported"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().at(1).name(),
             QStringLiteral("SecondImported"));
}

void AstXmlParserTests::test_multipleModules()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="11" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(      <Module Name="Defs2" Line="42" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt2" Line="99" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["File.asn"]->definitions("Defs")->type("MyInt")->location().line(), 11);
    QCOMPARE(m_parsedData["File.asn"]->definitions("Defs2")->type("MyInt2")->location().line(), 99);
}

void AstXmlParserTests::test_multipleFiles()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="11" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(  <Asn1File FileName="File2.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs2" Line="4" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt2" Line="9" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["File.asn"]->definitions("Defs")->type("MyInt")->location().line(), 11);
    QCOMPARE(m_parsedData["File2.asn"]->definitions("Defs2")->type("MyInt2")->location().line(), 9);
}

void AstXmlParserTests::test_parametrizedInstancesContentsAreIgnored()
{
    parse(
        R"(<?xml version="1.0" encoding="utf-8"?>)"
        R"(<AstRoot>)"
        R"(  <Asn1File FileName="Test2File.asn">)"
        R"(    <Modules>)"
        R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
        R"(        <TypeAssignments>)"
        R"(          <TypeAssignment Name="MySeq" Line="11" CharPositionInLine="9">)"
        R"(            <Asn1Type Line="12" CharPositionInLine="19" ParameterizedTypeInstance="true">)"
        R"(              <SEQUENCE>)"
        R"(                <Constraints/>)"
        R"(                <WithComponentConstraints/>)"
        R"(                <SEQUENCE_COMPONENT Name="b" Line="14" CharPositionInLine="2">)"
        R"(                  <Asn1Type Line="14" CharPositionInLine="4">)"
        R"(                    <REFERENCE_TYPE Module="Other" TypeAssignment="MyInt"/>")
        R"(                  </Asn1Type>)"
        R"(                </SEQUENCE_COMPONENT>)"
        R"(              </SEQUENCE>)"
        R"(            </Asn1Type>)"
        R"(          </TypeAssignment>)"
        R"(        </TypeAssignments>)"
        R"(      </Module>)"
        R"(    </Modules>)"
        R"(  </Asn1File>)"
        R"(</AstRoot>)");

    const auto seq = m_parsedData["Test2File.asn"]->definitions("Defs")->type("MySeq");
    QCOMPARE(seq->type()->name(), QStringLiteral("SEQUENCE"));

    QCOMPARE(seq->location().line(), 11);
    QCOMPARE(seq->location().column(), 9);
    QCOMPARE(seq->location().path(), QLatin1String("Test2File.asn"));

    QCOMPARE(m_parsedData["Test2File.asn"]->referencesMap().size(), std::size_t{1});

    const auto ref = m_parsedData["Test2File.asn"]->referencesMap().find(14);
    QVERIFY(ref == m_parsedData["Test2File.asn"]->referencesMap().end());
}

void AstXmlParserTests::test_enumeratedItems()
{
    parse(
        R"(<?xml version="1.0" encoding="utf-8"?>)"
        R"(<AstRoot>)"
        R"(  <Asn1File FileName="Test2File.asn">)"
        R"(    <Modules>)"
        R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
        R"(        <TypeAssignments>"
        R"(          <TypeAssignment Name="MyEnum" Line="9" CharPositionInLine="0">"
        R"(            <Asn1Type id="Constrained.MyEnum" Line="9" CharPositionInLine="21" ParameterizedTypeInstance="false">"
        R"(              <Enumerated>"
        R"(                <Items>"
        R"(                  <Item Name="ldev1" Value="0" Line="11" CharPositionInLine="4" />"
        R"(                  <Item Name="ldev2" Value="1" Line="12" CharPositionInLine="4" />"
        R"(                  <Item Name="ldev3" Value="2" Line="13" CharPositionInLine="4" />"
        R"(                </Items>"
        R"(                <Constraints />"
        R"(                <WithComponentConstraints />"
        R"(              </Enumerated>"
        R"(            </Asn1Type>"
        R"(          </TypeAssignment>"
        R"(        </TypeAssignments>"
        R"(      </Module>)"
        R"(    </Modules>)"
        R"(  </Asn1File>)"
        R"(</AstRoot>)");

    const auto myEnum = m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyEnum");
    const auto type = dynamic_cast<const Data::Types::Enumerated *>(myEnum->type());
    const auto items = type->items();

    QCOMPARE(items.size(), 3);

    QVERIFY(items.contains("ldev1"));
    auto item = items.value("ldev1");
    QCOMPARE(item.location(), Data::SourceLocation("Test2File.asn", 11, 4));
    QCOMPARE(item.value(), 0);

    QVERIFY(items.contains("ldev2"));
    item = items.value("ldev2");
    QCOMPARE(item.location(), Data::SourceLocation("Test2File.asn", 12, 4));
    QCOMPARE(item.value(), 1);

    QVERIFY(items.contains("ldev3"));
    item = items.value("ldev3");
    QCOMPARE(item.location(), Data::SourceLocation("Test2File.asn", 13, 4));
    QCOMPARE(item.value(), 2);
}

void AstXmlParserTests::test_enumeratedConstraints()
{
    parse(
        R"(<?xml version="1.0" encoding="utf-8"?>)"
        R"(<AstRoot>)"
        R"(  <Asn1File FileName="Test2File.asn">)"
        R"(    <Modules>)"
        R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
        R"(        <TypeAssignments>"
        R"(          <TypeAssignment Name="MyEnum" Line="9" CharPositionInLine="0">"
        R"(            <Asn1Type id="Constrained.MyEnum" Line="9" CharPositionInLine="21" ParameterizedTypeInstance="false">"
        R"(              <Enumerated>"
        R"(                <Items>"
        R"(                  <Item Name="ldev1" Value="0" Line="11" CharPositionInLine="4" />"
        R"(                  <Item Name="ldev2" Value="1" Line="12" CharPositionInLine="4" />"
        R"(                  <Item Name="ldev3" Value="2" Line="13" CharPositionInLine="4" />"
        R"(                </Items>"
        R"(                <Constraints>"
        R"(                  <OR>"
        R"(                    <OR>"
        R"(                      <EnumValue>ldev1</EnumValue>"
        R"(                      <EnumValue>ldev2</EnumValue>"
        R"(                    </OR>"
        R"(                    <EnumValue>ldev3</EnumValue>"
        R"(                  </OR>"
        R"(                </Constraints>"
        R"(                <WithComponentConstraints />"
        R"(              </Enumerated>"
        R"(            </Asn1Type>"
        R"(          </TypeAssignment>"
        R"(        </TypeAssignments>"
        R"(      </Module>)"
        R"(    </Modules>)"
        R"(  </Asn1File>)"
        R"(</AstRoot>)");

    const auto type = m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyEnum");
    const auto enumType = dynamic_cast<const Data::Types::Enumerated *>(type->type());
    const auto constraintRanges = enumType->constraints().ranges();

    QCOMPARE(constraintRanges.size(), 3);
    QVERIFY(constraintRanges.contains({0, 0}));
    QVERIFY(constraintRanges.contains({1, 1}));
    QVERIFY(constraintRanges.contains({2, 2}));
}

void AstXmlParserTests::test_enumeratedWithAcnParams()
{
    parse(
        R"(<?xml version="1.0" encoding="utf-8"?>)"
        R"(<AstRoot>)"
        R"(  <Asn1File FileName="Test2File.asn">)"
        R"(    <Modules>)"
        R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
        R"(        <TypeAssignments>"
        R"(          <TypeAssignment Name="MyEnum" Line="9" CharPositionInLine="0">"
        R"(            <Asn1Type id="Constrained.MyEnum" Line="9" CharPositionInLine="21" ParameterizedTypeInstance="false" align-to-next="dword">"
        R"(              <Enumerated endianness="big" size="8" encoding="BCD">"
        R"(                <Items>"
        R"(                  <Item Name="ldev1" Value="0" Line="11" CharPositionInLine="4" />"
        R"(                  <Item Name="ldev2" Value="1" Line="12" CharPositionInLine="4" />"
        R"(                  <Item Name="ldev3" Value="2" Line="13" CharPositionInLine="4" />"
        R"(                </Items>"
        R"(                <Constraints />"
        R"(                <WithComponentConstraints />"
        R"(              </Enumerated>"
        R"(            </Asn1Type>"
        R"(          </TypeAssignment>"
        R"(        </TypeAssignments>"
        R"(      </Module>)"
        R"(    </Modules>)"
        R"(  </Asn1File>)"
        R"(</AstRoot>)");

    const auto myEnum = m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyEnum");
    const auto type = dynamic_cast<const Data::Types::Enumerated *>(myEnum->type());

    QVERIFY(type != nullptr);

    QCOMPARE(type->size(), 8);
    QCOMPARE(type->encoding(), Data::Types::IntegerEncoding::BCD);
    QCOMPARE(type->endianness(), Data::Types::Endianness::big);
    QCOMPARE(type->alignToNext(), Data::Types::AlignToNext::dword);
    QCOMPARE(type->encodeValues(), true);
}

void AstXmlParserTests::test_singleIntegerTypeAssignmentWithSimpleConstraint()
{
    parse(
        R"(<?xml version="1.0" encoding="utf-8"?>)"
        R"(<AstRoot>)"
        R"(  <Asn1File FileName="Test2File.asn">)"
        R"(    <Modules>)"
        R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
        R"(        <TypeAssignments>"
        R"(          <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10">)"
        R"(            <Asn1Type id="Constrained.MyInt" Line="3" CharPositionInLine="14" ParameterizedTypeInstance="false">"
        R"(              <INTEGER>"
        R"(                <Constraints>"
        R"(                  <IntegerValue>1</IntegerValue>"
        R"(                </Constraints>"
        R"(                <WithComponentConstraints />"
        R"(              </INTEGER>"
        R"(            </Asn1Type>"
        R"(          </TypeAssignment>"
        R"(        </TypeAssignments>"
        R"(      </Module>)"
        R"(    </Modules>)"
        R"(  </Asn1File>)"
        R"(</AstRoot>)");

    const auto type = m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt");
    const auto intType = dynamic_cast<const Data::Types::Integer *>(type->type());
    const auto constraintRanges = intType->constraints().ranges();

    QCOMPARE(constraintRanges.size(), 1);
    QVERIFY(constraintRanges.contains({1, 1}));
}

void AstXmlParserTests::test_singleIntegerTypeAssignmentWithRangedConstraints()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10">)"
          R"(            <Asn1Type>)"
          R"(              <INTEGER>
          R"(                <Constraints>
          R"(                  <OR>
          R"(                    <OR>
          R"(                      <OR>
          R"(                        <OR>
          R"(                          <OR>
          R"(                            <Range>
          R"(                              <a>
          R"(                                <IntegerValue>10</IntegerValue>
          R"(                              </a>
          R"(                              <b>
          R"(                                <IntegerValue>20</IntegerValue>
          R"(                              </b>
          R"(                            </Range>
          R"(                            <Range>
          R"(                              <a>
          R"(                                <IntegerValue>30</IntegerValue>
          R"(                              </a>
          R"(                              <b>
          R"(                                <IntegerValue>40</IntegerValue>
          R"(                              </b>
          R"(                            </Range>
          R"(                          </OR>
          R"(                          <IntegerValue>42</IntegerValue>
          R"(                        </OR>
          R"(                        <IntegerValue>44</IntegerValue>
          R"(                      </OR>
          R"(                      <IntegerValue>46</IntegerValue>
          R"(                    </OR>
          R"(                    <Range>
          R"(                      <a>
          R"(                        <IntegerValue>50</IntegerValue>
          R"(                      </a>
          R"(                      <b>
          R"(                        <IntegerValue>80</IntegerValue>
          R"(                      </b>
          R"(                    </Range>
          R"(                  </OR>
          R"(                </Constraints>
          R"(                <WithComponentConstraints />
          R"(              </INTEGER>
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    const auto type = m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt");
    const auto intType = dynamic_cast<const Data::Types::Integer *>(type->type());
    const auto constraintRanges = intType->constraints().ranges();

    QCOMPARE(constraintRanges.size(), 6);

    QVERIFY(constraintRanges.contains({10, 20}));
    QVERIFY(constraintRanges.contains({30, 40}));
    QVERIFY(constraintRanges.contains({42, 42}));
    QVERIFY(constraintRanges.contains({44, 44}));
    QVERIFY(constraintRanges.contains({46, 46}));
    QVERIFY(constraintRanges.contains({50, 80}));
}

void AstXmlParserTests::test_singleIntegerTypeAssignmentWithAcnParams()
{
    parse(
        R"(<?xml version="1.0" encoding="utf-8"?>)"
        R"(<AstRoot>)"
        R"(  <Asn1File FileName="Test2File.asn">)"
        R"(    <Modules>)"
        R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
        R"(        <TypeAssignments>"
        R"(          <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10">)"
        R"(            <Asn1Type id="Constrained.MyInt" Line="3" CharPositionInLine="14" ParameterizedTypeInstance="false" align-to-next="dword">"
        R"(              <INTEGER endianness="big" size="8" encoding="BCD">"
        R"(                <Constraints>"
        R"(                  <IntegerValue>1</IntegerValue>"
        R"(                </Constraints>"
        R"(                <WithComponentConstraints />"
        R"(              </INTEGER>"
        R"(            </Asn1Type>"
        R"(          </TypeAssignment>"
        R"(        </TypeAssignments>"
        R"(      </Module>)"
        R"(    </Modules>)"
        R"(  </Asn1File>)"
        R"(</AstRoot>)");

    const auto myInt = m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt");
    const auto type = dynamic_cast<const Data::Types::Integer *>(myInt->type());

    QVERIFY(type != nullptr);
    QCOMPARE(type->size(), 8);
    QCOMPARE(type->encoding(), Data::Types::IntegerEncoding::BCD);
    QCOMPARE(type->endianness(), Data::Types::Endianness::big);
    QCOMPARE(type->alignToNext(), Data::Types::AlignToNext::dword);
}

void AstXmlParserTests::test_singleRealTypeAssignmentWithSimpleConstraint()
{
    parse(
        R"(<?xml version="1.0" encoding="utf-8"?>)"
        R"(<AstRoot>)"
        R"(  <Asn1File FileName="Test2File.asn">)"
        R"(    <Modules>)"
        R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
        R"(        <TypeAssignments>"
        R"(          <TypeAssignment Name="MyReal" Line="4" CharPositionInLine="10">)"
        R"(            <Asn1Type id="Constrained.MyReal" Line="3" CharPositionInLine="14" ParameterizedTypeInstance="false">"
        R"(              <REAL>"
        R"(                <Constraints>"
        R"(                  <RealValue>1.1</RealValue>"
        R"(                </Constraints>"
        R"(                <WithComponentConstraints />"
        R"(              </REAL>"
        R"(            </Asn1Type>"
        R"(          </TypeAssignment>"
        R"(        </TypeAssignments>"
        R"(      </Module>)"
        R"(    </Modules>)"
        R"(  </Asn1File>)"
        R"(</AstRoot>)");

    const auto type = m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyReal");
    const auto realType = dynamic_cast<const Data::Types::Real *>(type->type());
    const auto constraintRanges = realType->constraints().ranges();

    QCOMPARE(constraintRanges.size(), 1);
    QVERIFY(qFuzzyCompare(constraintRanges.at(0).first, 1.1));
    QVERIFY(qFuzzyCompare(constraintRanges.at(0).second, 1.1));
}

void AstXmlParserTests::test_singleRealTypeAssignmentWithRangedConstraints()
{
    parse(
        R"(<?xml version="1.0" encoding="utf-8"?>)"
        R"(<AstRoot>)"
        R"(  <Asn1File FileName="Test2File.asn">)"
        R"(    <Modules>)"
        R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
        R"(        <TypeAssignments>"
        R"(          <TypeAssignment Name="MyReal" Line="4" CharPositionInLine="10">)"
        R"(            <Asn1Type id="Constrained.MyReal" Line="3" CharPositionInLine="14" ParameterizedTypeInstance="false">"
        R"(              <REAL>"
        R"(                <Constraints>"
        R"(                  <OR>"
        R"(                    <Range>"
        R"(                      <a>"
        R"(                        <RealValue>1.1</RealValue>"
        R"(                      </a>"
        R"(                      <b>"
        R"(                        <RealValue>2.5</RealValue>"
        R"(                      </b>"
        R"(                    </Range>"
        R"(                    <Range>"
        R"(                      <a>"
        R"(                        <RealValue>4.5</RealValue>"
        R"(                      </a>"
        R"(                      <b>"
        R"(                        <RealValue>5.5</RealValue>"
        R"(                      </b>"
        R"(                    </Range>"
        R"(                  </OR>"
        R"(                </Constraints>"
        R"(                <WithComponentConstraints />"
        R"(              </REAL>"
        R"(            </Asn1Type>"
        R"(          </TypeAssignment>"
        R"(        </TypeAssignments>"
        R"(      </Module>)"
        R"(    </Modules>)"
        R"(  </Asn1File>)"
        R"(</AstRoot>)");

    const auto type = m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyReal");
    const auto realType = dynamic_cast<const Data::Types::Real *>(type->type());
    const auto constraintRanges = realType->constraints().ranges();

    QCOMPARE(constraintRanges.size(), 2);

    QVERIFY(qFuzzyCompare(constraintRanges.at(0).first, 1.1));
    QVERIFY(qFuzzyCompare(constraintRanges.at(0).second, 2.5));

    QVERIFY(qFuzzyCompare(constraintRanges.at(1).first, 4.5));
    QVERIFY(qFuzzyCompare(constraintRanges.at(1).second, 5.5));
}

void AstXmlParserTests::test_singleRealTypeAssignmentWithAcnParams()
{
    parse(
        R"(<?xml version="1.0" encoding="utf-8"?>)"
        R"(<AstRoot>)"
        R"(  <Asn1File FileName="Test2File.asn">)"
        R"(    <Modules>)"
        R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
        R"(        <TypeAssignments>"
        R"(          <TypeAssignment Name="MyReal" Line="4" CharPositionInLine="10">)"
        R"(            <Asn1Type id="Constrained.MyReal" Line="20" CharPositionInLine="11" ParameterizedTypeInstance="false" align-to-next="dword">
        R"(              <REAL endianness="little" encoding="IEEE754-1985-32">
        R"(                <Constraints />
        R"(                <WithComponentConstraints />
        R"(              </REAL>
        R"(            </Asn1Type>
        R"(          </TypeAssignment>"
        R"(        </TypeAssignments>"
        R"(      </Module>)"
        R"(    </Modules>)"
        R"(  </Asn1File>)"
        R"(</AstRoot>)");

    const auto myReal = m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyReal");
    const auto type = dynamic_cast<const Data::Types::Real *>(myReal->type());

    QVERIFY(type != nullptr);

    QCOMPARE(type->encoding(), Data::Types::RealEncoding::IEEE754_1985_32);
    QCOMPARE(type->endianness(), Data::Types::Endianness::little);
    QCOMPARE(type->alignToNext(), Data::Types::AlignToNext::dword);
}

void AstXmlParserTests::test_sequenceOfAssignmentWithSimpleConstraint()
{
    parse(
        R"(<?xml version="1.0" encoding="utf-8"?>)"
        R"(<AstRoot>)"
        R"(  <Asn1File FileName="Test2File.asn">)"
        R"(    <Modules>)"
        R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
        R"(        <TypeAssignments>)"
        R"(          <TypeAssignment Name="MySeq0" Line="6" CharPositionInLine="0">)"
        R"(            <Asn1Type id="Test.MySeq0" Line="6" CharPositionInLine="11" ParameterizedTypeInstance="false">)"
        R"(              <SEQUENCE_OF>)"
        R"(                <Constraints>)"
        R"(                  <SIZE>)"
        R"(                    <IntegerValue>10</IntegerValue>)"
        R"(                  </SIZE>)"
        R"(               </Constraints>)"
        R"(               <WithComponentConstraints />)"
        R"(               <Asn1Type id="Test.MySeq0.#" Line="6" CharPositionInLine="34" ParameterizedTypeInstance="false">)"
        R"(                 <INTEGER>)"
        R"(                   <Constraints />)"
        R"(                   <WithComponentConstraints />)"
        R"(                 </INTEGER>)"
        R"(               </Asn1Type>)"
        R"(             </SEQUENCE_OF>)"
        R"(           </Asn1Type>)"
        R"(         </TypeAssignment>)"
        R"(       </TypeAssignments>)"
        R"(      </Module>)"
        R"(    </Modules>)"
        R"(  </Asn1File>)"
        R"(</AstRoot>)");

    const auto type = m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MySeq0");
    const auto realType = dynamic_cast<const Data::Types::SequenceOf *>(type->type());
    const auto constraintRanges = realType->constraints().ranges();

    QCOMPARE(constraintRanges.size(), 1);
    QCOMPARE(constraintRanges.at(0).first, 10);
    QCOMPARE(constraintRanges.at(0).second, 10);
}

void AstXmlParserTests::test_sequenceOfAssignmentWithMultipleRangeConstraints()
{
    parse(
        R"(<?xml version="1.0" encoding="utf-8"?>)"
        R"(<AstRoot>)"
        R"(  <Asn1File FileName="Test2File.asn">)"
        R"(    <Modules>)"
        R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
        R"(        <TypeAssignments>)"
        R"(          <TypeAssignment Name="MySeq0" Line="6" CharPositionInLine="0">)"
        R"(            <Asn1Type id="Test.MySeq0" Line="6" CharPositionInLine="11" ParameterizedTypeInstance="false">)"
        R"(              <SEQUENCE_OF>)"
        R"(                <Constraints>)"
        R"(                  <OR>)"
        R"(                    <SIZE>)"
        R"(                      <Range>)"
        R"(                        <a>)"
        R"(                          <IntegerValue>0</IntegerValue>)"
        R"(                        </a>)"
        R"(                        <b>)"
        R"(                          <IntegerValue>10</IntegerValue>)"
        R"(                        </b>)"
        R"(                     </Range>)"
        R"(                   </SIZE>)"
        R"(                   <SIZE>)"
        R"(                     <Range>)"
        R"(                       <a>)"
        R"(                         <IntegerValue>20</IntegerValue>)"
        R"(                       </a>)"
        R"(                       <b>)"
        R"(                         <IntegerValue>22</IntegerValue>)"
        R"(                       </b>)"
        R"(                     </Range>)"
        R"(                   </SIZE>)"
        R"(                 </OR>)"
        R"(               </Constraints>)"
        R"(               <WithComponentConstraints />)"
        R"(               <Asn1Type id="Test.MySeq0.#" Line="6" CharPositionInLine="34" ParameterizedTypeInstance="false">)"
        R"(                 <INTEGER>)"
        R"(                   <Constraints />)"
        R"(                   <WithComponentConstraints />)"
        R"(                 </INTEGER>)"
        R"(               </Asn1Type>)"
        R"(             </SEQUENCE_OF>)"
        R"(           </Asn1Type>)"
        R"(         </TypeAssignment>)"
        R"(       </TypeAssignments>)"
        R"(      </Module>)"
        R"(    </Modules>)"
        R"(  </Asn1File>)"
        R"(</AstRoot>)");

    const auto type = m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MySeq0");
    const auto realType = dynamic_cast<const Data::Types::SequenceOf *>(type->type());
    const auto constraintRanges = realType->constraints().ranges();

    QCOMPARE(constraintRanges.size(), 2);
    QVERIFY(constraintRanges.contains({0, 10}));
    QVERIFY(constraintRanges.contains({20, 22}));
}

void AstXmlParserTests::test_sequenceOfAssignmentWithRangeConstraintInsideSizeConstraint()
{
    parse(
        R"(<?xml version="1.0" encoding="utf-8"?>)"
        R"(<AstRoot>)"
        R"(  <Asn1File FileName="Test2File.asn">)"
        R"(    <Modules>)"
        R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
        R"(        <TypeAssignments>)"
        R"(          <TypeAssignment Name="MySeq0" Line="6" CharPositionInLine="0">)"
        R"(            <Asn1Type id="Test.MySeq0" Line="6" CharPositionInLine="11" ParameterizedTypeInstance="false">)"
        R"(              <SEQUENCE_OF>)"
        R"(                <Constraints>)"
        R"(                  <SIZE>)"
        R"(                    <OR>)"
        R"(                      <Range>)"
        R"(                        <a>)"
        R"(                          <IntegerValue>0</IntegerValue>)"
        R"(                        </a>)"
        R"(                        <b>)"
        R"(                          <IntegerValue>10</IntegerValue>)"
        R"(                        </b>)"
        R"(                     </Range>)"
        R"(                     <Range>)"
        R"(                       <a>)"
        R"(                         <IntegerValue>20</IntegerValue>)"
        R"(                       </a>)"
        R"(                       <b>)"
        R"(                         <IntegerValue>22</IntegerValue>)"
        R"(                       </b>)"
        R"(                     </Range>)"
        R"(                   </OR>)"
        R"(                 </SIZE>)"
        R"(               </Constraints>)"
        R"(               <WithComponentConstraints />)"
        R"(               <Asn1Type id="Test.MySeq0.#" Line="6" CharPositionInLine="34" ParameterizedTypeInstance="false">)"
        R"(                 <INTEGER>)"
        R"(                   <Constraints />)"
        R"(                   <WithComponentConstraints />)"
        R"(                 </INTEGER>)"
        R"(               </Asn1Type>)"
        R"(             </SEQUENCE_OF>)"
        R"(           </Asn1Type>)"
        R"(         </TypeAssignment>)"
        R"(       </TypeAssignments>)"
        R"(      </Module>)"
        R"(    </Modules>)"
        R"(  </Asn1File>)"
        R"(</AstRoot>)");

    const auto type = m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MySeq0");
    const auto realType = dynamic_cast<const Data::Types::SequenceOf *>(type->type());
    const auto constraintRanges = realType->constraints().ranges();

    QCOMPARE(constraintRanges.size(), 2);
    QVERIFY(constraintRanges.contains({0, 10}));
    QVERIFY(constraintRanges.contains({20, 22}));
}

void AstXmlParserTests::test_sequenceOfAssignmentWithAcnParams()
{
    parse(
        R"(<?xml version="1.0" encoding="utf-8"?>)"
        R"(<AstRoot>)"
        R"(  <Asn1File FileName="Test2File.asn">)"
        R"(    <Modules>)"
        R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
        R"(        <TypeAssignments>)"
        R"(          <TypeAssignment Name="MySeq0" Line="6" CharPositionInLine="0">)"
        R"(            <Asn1Type id="Test.MySeq0" Line="6" CharPositionInLine="11" ParameterizedTypeInstance="false">)"
        R"(              <SEQUENCE_OF size="n">)"
        R"(                <Constraints>)"
        R"(                  <SIZE>)"
        R"(                    <IntegerValue>10</IntegerValue>)"
        R"(                  </SIZE>)"
        R"(               </Constraints>)"
        R"(               <WithComponentConstraints />)"
        R"(               <Asn1Type id="Test.MySeq0.#" Line="6" CharPositionInLine="34" ParameterizedTypeInstance="false">)"
        R"(                 <INTEGER>)"
        R"(                   <Constraints />)"
        R"(                   <WithComponentConstraints />)"
        R"(                 </INTEGER>)"
        R"(               </Asn1Type>)"
        R"(             </SEQUENCE_OF>)"
        R"(           </Asn1Type>)"
        R"(         </TypeAssignment>)"
        R"(       </TypeAssignments>)"
        R"(      </Module>)"
        R"(    </Modules>)"
        R"(  </Asn1File>)"
        R"(</AstRoot>)");

    const auto type = m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MySeq0");
    const auto sequenceOfType = dynamic_cast<const Data::Types::SequenceOf *>(type->type());

    QVERIFY(sequenceOfType != nullptr);
    QCOMPARE(sequenceOfType->size(), QStringLiteral("n"));
}

void AstXmlParserTests::test_choiceAlternatives()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyChoice" Line="5" CharPositionInLine="0">"
          R"(            <Asn1Type id="MyChoiceModel.MyChoice" Line="5" CharPositionInLine="13" ParameterizedTypeInstance="false">"
          R"(              <CHOICE>"
          R"(                <CHOICE_ALTERNATIVE Name="i1" Line="7" CharPositionInLine="4" PresentWhenName="i1" AdaName="i1" CName="i1">"
          R"(                  <Asn1Type id="MyChoiceModel.MyChoice.i1" Line="7" CharPositionInLine="7" ParameterizedTypeInstance="false">"
          R"(                    <INTEGER>"
          R"(                      <Constraints />"
          R"(                      <WithComponentConstraints />"
          R"(                    </INTEGER>"
          R"(                  </Asn1Type>"
          R"(                </CHOICE_ALTERNATIVE>"
          R"(                <Constraints />"
          R"(                <WithComponentConstraints />"
          R"(              </CHOICE>"
          R"(            </Asn1Type>"
          R"(          </TypeAssignment>"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    const auto type = m_parsedData["Test2File.asn"]->definitions("Defs")->type("MyChoice");
    const auto choiceType = dynamic_cast<const Data::Types::Choice *>(type->type());

    QVERIFY(choiceType != nullptr);
    auto alternatives = choiceType->alternatives();

    QCOMPARE(alternatives.size(), static_cast<unsigned int>(1));
    auto alternative = alternatives.find(QStringLiteral("i1"));

    QVERIFY(alternative != alternatives.end());
    QCOMPARE(alternative->second.presentWhenName(), QStringLiteral("i1"));
    QCOMPARE(alternative->second.adaName(), QStringLiteral("i1"));
    QCOMPARE(alternative->second.cName(), QStringLiteral("i1"));

    const auto &location = alternative->second.location();
    QCOMPARE(location.path(), QStringLiteral("Test2File.asn"));
    QCOMPARE(location.line(), 7);
    QCOMPARE(location.column(), 4);

    QCOMPARE(alternative->second.type().name(), QStringLiteral("INTEGER"));
}

void AstXmlParserTests::test_choiceAlternativesWithAcnParams()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyChoice" Line="5" CharPositionInLine="0">"
          R"(            <Asn1Type id="MyChoiceModel.MyChoice" Line="5" CharPositionInLine="13" ParameterizedTypeInstance="false">"
          R"(              <CHOICE determinant="deter">"
          R"(                <CHOICE_ALTERNATIVE Name="i1" Line="7" CharPositionInLine="4" PresentWhenName="i1" AdaName="i1" CName="i1" present-when="type = 1">"
          R"(                  <Asn1Type id="MyChoiceModel.MyChoice.i1" Line="7" CharPositionInLine="7" ParameterizedTypeInstance="false">"
          R"(                    <INTEGER>"
          R"(                      <Constraints />"
          R"(                      <WithComponentConstraints />"
          R"(                    </INTEGER>"
          R"(                  </Asn1Type>"
          R"(                </CHOICE_ALTERNATIVE>"
          R"(                <Constraints />"
          R"(                <WithComponentConstraints />"
          R"(              </CHOICE>"
          R"(            </Asn1Type>"
          R"(          </TypeAssignment>"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    const auto type = m_parsedData["Test2File.asn"]->definitions("Defs")->type("MyChoice");
    const auto choiceType = dynamic_cast<const Data::Types::Choice *>(type->type());

    QCOMPARE(choiceType->determinant(), QStringLiteral("deter"));

    auto alternatives = choiceType->alternatives();
    auto alternative = alternatives.find(QStringLiteral("i1"));
    QCOMPARE(alternative->second.presentWhen(), QStringLiteral("type = 1"));
}

void AstXmlParserTests::parsingFails(const QString &xmlData)
{
    setXmlData(xmlData);
    AstXmlParser parser(m_xmlReader);
    QVERIFY(!parser.parse());
}

void AstXmlParserTests::parse(const QString &xmlData)
{
    setXmlData(xmlData);
    AstXmlParser parser(m_xmlReader);
    QVERIFY(parser.parse());
    m_parsedData = parser.takeData();
}
