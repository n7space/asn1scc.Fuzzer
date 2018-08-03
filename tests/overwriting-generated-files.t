# Keep no new line at end to make it work on Windows
  $ ${TESTDIR}/copy_test_resources.sh
  $ asn1scc-maltester -r EnumTests.MyEnum -o generated myenum.asn1 myenum.acn
  Generated 1 case(s).
  $ ${TESTDIR}/list_files.sh generated
  AllModels.acn
  AllModels.asn1
  test_main.c
  validate.h
  $ cp generated/AllModels.asn1 generated/validate.h
  $ asn1scc-maltester -r EnumTests.MyEnum -o generated myenum.asn1 myenum.acn
  Generated 1 case(s).
  $ cat generated/validate.h
  #ifndef ASN1SCC_MALTESTER_VALIDATE_H
  #define ASN1SCC_MALTESTER_VALIDATE_H
  
  #include <stdbool.h>
  
  #include "asn1crt.h"
  
  bool validate(const BitStream *stream);
  
  #endif
