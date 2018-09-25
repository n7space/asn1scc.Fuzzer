# asn1scc.Fuzzer
Tool for generating test cases based on ASN.1/ACN models and simulating malformed or malicious data.

[![Build Status](https://travis-ci.org/n7space/asn1scc.Fuzzer.svg?branch=master)](https://travis-ci.org/n7space/asn1scc.Fuzzer)
[![Build status](https://ci.appveyor.com/api/projects/status/8876k84l5edrhql3/branch/master?svg=true)](https://ci.appveyor.com/project/hcorg/asn1scc-fuzzer/branch/master)

Example usage
------------

Given simple ASN.1 model with ACN encoding:
#### a.asn1
```
Example DEFINITIONS ::= BEGIN

MySeq ::= SEQUENCE {
  a INTEGER(0..10),
  b INTEGER(5..126)
}

END
```
#### a.acn
```
Example DEFINITIONS ::= BEGIN

MySeq [] {
  a [encoding pos-int, size 6],
  b [encoding pos-int, size 7]
}

END
```

Such models describes message, which apart from accepted range of integers, can fit additional values. Software should properly react when such incorrect message is sent to it. That's where asn1scc-Fuzzer comes in:

```
asn1scc-fuzzer -r MySeq a.asn1 a.acn
Generated 7 case(s).
```

Generated cases build messages containg incorrect messages:
```
test_a_11
test_a_37
test_a_63
test_b_0
test_b_2
test_b_4
test_b_127
```
User is responsible for providing those messages to System Under Test and validate it proper behaviour.


Installation
------------
Download appropriate package from https://github.com/n7space/asn1scc.Fuzzer/releases and unpack. Software requires ASN1SCC compiler (https://github.com/ttsiodras/asn1scc).

Compilation
------------
Requires C++14 compatible compiler and Qt5.
 * clone repository
 * create build directory (outside cloned source)
 * in build directory execute `qmake -r SRCDIR/fuzzer.pro`
 * execute generated build system (e.g. `make` on Linux)

Usage
------------

```
asn1scc-fuzzer [options] <files>

Options:
  -h, --help                           Displays this help.
  -v, --version                        Displays version information.
  -a, --asn1scc-path <asn1scc path>    ASN1SCC compiler path.
  -f, --asn1scc-flags <asn1scc flags>  ASN1SCC compiler flags.
  -o, --output-dir <output directory>  Output directory for generated files.
  -r, --root-type <root type>          Root type representing message to be
                                       malformed.
  -w, --wrap-as-ccsds <[tc, tm]>       Wrapping root type inside CCSDS packets.

Arguments:
  <files>                              List of files to be processed.
```
